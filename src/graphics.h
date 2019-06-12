#pragma once
#include <emscripten.h>
#include <emscripten/bind.h>
#include <emscripten/html5.h>

#include <GLES3/gl3.h>

//std
#include <vector>
#include <stdio.h>
#include <map>
#include <string>
#include <chrono>
#include <memory>

#include "graphics/camera.h"
#include "input_handler.h"
#include "graphics_gl.h"
#include "core_math.h"

#include "import_obj.h"

namespace graphics
{
class window;
}

namespace
{
int next_window_id = 0;
std::map<int, graphics::window *> windows;
std::map<std::string, int> window_name_id;
} // namespace

static void output_error(int error, const char *msg)
{
    printf("Error: %s\n", msg);
}

namespace graphics
{

class window
{

private:
    int window_id_;
    std::string window_name_;
    std::vector<float> vertices_;
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context_;

    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLuint normal_buffer;
    GLuint uv_buffer;
    std::map<std::string, GLint> uniform_locations_;
    std::map<std::string, GLint> attribute_locations_;

    std::chrono::system_clock::time_point last_time_point_;
    std::chrono::system_clock::time_point start_time_point_;

    vec3 camera_position_;
    std::unique_ptr<camera> camera_;
    std::unique_ptr<input_handler> input_handler_;

    obj_data obj_;

    float avg_fps_ = 0.0;

public:
    window(const std::string &window_name)
    {
        window_id_ = next_window_id++;
        window_name_ = window_name;
        windows[window_id_] = this;
        window_name_id[window_name] = window_id_;

        camera_ = std::make_unique<camera>();
        input_handler_ = std::make_unique<input_handler>(camera_.get());
        printf("window created %d\n", window_id_);
        init_gl();
        init_js_callbacks();
        last_time_point_ = std::chrono::system_clock::now();
        start_time_point_ = last_time_point_;
    }

    ~window()
    {
        emscripten_webgl_make_context_current(0);
        emscripten_webgl_destroy_context(context_);
    }

    input_handler *get_input_handler()
    {
        return input_handler_.get();
    }

    void display_fps(float fps)
    {
        float n_samples = 10.0f;
        avg_fps_ -= avg_fps_ / n_samples;
        avg_fps_ += fps / n_samples;
        EM_ASM(
            var fps_div = document.getElementById("fps");
            fps_div.innerHTML = "FPS: " + $0;
            , (int)avg_fps_);
    }

    //Resize canvas drawing size to display size if size has changed
    void resize()
    {
        // Set Javascript callbacks
        EM_ASM(
            var canvas_id = UTF8ToString($0);
            var canvas = document.getElementById(canvas_id);
            // Lookup the size the browser is displaying the canvas.
            var displayWidth = canvas.clientWidth;
            var displayHeight = canvas.clientHeight;

            // Check if the canvas is not the same size.
            if (canvas.width != displayWidth ||
                canvas.height != displayHeight) {
                // Make the canvas the same size
                canvas.width = displayWidth;
                canvas.height = displayHeight;
                //canvas.style.width = displayWidth;
                //canvas.style.height = displayHeight;
            },
            window_name_.c_str());
    }

    void init_gl()
    {

        EmscriptenWebGLContextAttributes attrs;
        emscripten_webgl_init_context_attributes(&attrs);
        attrs.depth = 1;
        attrs.stencil = 1;
        attrs.antialias = 1;
        attrs.majorVersion = 3;
        attrs.minorVersion = 0;

        ////assert(emscripten_webgl_get_current_context() == 0);
        context_ = emscripten_webgl_create_context("canvas", &attrs);
        printf("Context = %d\n", context_);
        emscripten_webgl_make_context_current(context_);

        int w = 0;
        int h = 0;
        emscripten_webgl_get_drawing_buffer_size(context_, &w, &h);
        printf("context width = %d, height = %d\n", w, h);

        printf("OpenGL version supported by this platform : %s\n", glGetString(GL_VERSION));

        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS);
        // Cull triangles which normal is not towards the camera
        glEnable(GL_CULL_FACE);

        obj_ = import_test_cube();

        init_shaders();

        uniform_locations_["model"] = glGetUniformLocation(program, "model");
        uniform_locations_["view"] = glGetUniformLocation(program, "view");
        uniform_locations_["projection"] = glGetUniformLocation(program, "projection");

        uniform_locations_["camera_pos"] = glGetUniformLocation(program, "camera_pos");
        uniform_locations_["light_pos"] = glGetUniformLocation(program, "light_pos");

        attribute_locations_["v_pos"] = glGetAttribLocation(program, "v_pos");
        attribute_locations_["v_norm"] = glGetAttribLocation(program, "v_norm");
        attribute_locations_["v_uv"] = glGetAttribLocation(program, "v_uv");

        printf("Bind vertices\n");
        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, obj_.vertices.size() * sizeof(float), &obj_.vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(attribute_locations_["v_pos"]);
        glVertexAttribPointer(attribute_locations_["v_pos"], 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        printf("Bind normals\n");
        glGenBuffers(1, &normal_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
        glBufferData(GL_ARRAY_BUFFER, obj_.normals.size() * sizeof(float), &obj_.normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(attribute_locations_["v_norm"]);
        glVertexAttribPointer(attribute_locations_["v_norm"], 3, GL_FLOAT, GL_FALSE, 0, (void *)0);

        printf("Bind uvs\n");
        glGenBuffers(1, &uv_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
        glBufferData(GL_ARRAY_BUFFER, obj_.uvs.size() * sizeof(float), &obj_.normals[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(attribute_locations_["v_uv"]);
        glVertexAttribPointer(attribute_locations_["v_uv"], 2, GL_FLOAT, GL_FALSE, 0, (void *)0);
    }

    void init_js_callbacks()
    {

        // Set Javascript callbacks
        EM_ASM(
            var window_name = UTF8ToString($0);
            var window_id = $1;
            var canv = document.getElementById(window_name);
            //console.log("window_name: " + window_name + "\n");

            canv.onpointerdown = function(e) {
                var rect = e.target.getBoundingClientRect();
                var x = e.clientX - rect.left; //x position within the element.
                var y = e.clientY - rect.top;  //y position within the element.
                var capture = _on_pointer_down(window_id, e.button, 0, x, y);
                if(capture == 1){
                    canv.setPointerCapture(e.pointerId);
                } };
            canv.onpointerup = function(e) {
                var rect = e.target.getBoundingClientRect();
                var x = e.clientX - rect.left; //x position within the element.
                var y = e.clientY - rect.top;  //y position within the element.
                var capture = _on_pointer_up(window_id, e.button, 0, x, y);
                if(capture == 0){
                    canv.releasePointerCapture(e.pointerId);
                } };
            canv.oncontextmenu = function(e){e.preventDefault()};

            canv.onpointermove = function(e) {
                var rect = e.target.getBoundingClientRect();
                var x = e.clientX - rect.left; //x position within the element.
                var y = e.clientY - rect.top;  //y position within the element.
                var capture = _on_pointer_move(window_id, e.buttons, 0, x, y);
                if(capture == 0){
                    canv.releasePointerCapture(e.pointerId);
                } };

            canv.onwheel = function(e) { _on_wheel(window_id, e.deltaY); };

            canv.ondrop = function(e) { dropHandler(event); };

            canv.onresize = function() {
            canv.width = canv.clientWidth;
            canv.height = canv.clientHeight; };, window_name_.c_str(), window_id_);
    }

    void init_shaders()
    {
        printf("init_shaders()\n");
        printf("Create vertex_shader\n");
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
        glCompileShader(vertex_shader);
        check_compiled(vertex_shader);

        printf("Create fragment_shader\n");
        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
        glCompileShader(fragment_shader);
        check_compiled(fragment_shader);

        printf("Create program\n");
        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);
        check_linked(program);
        printf("init_shaders() return\n");
    }

    void draw()
    {
        resize();

        auto current_time_point = std::chrono::system_clock::now();

        auto elapsed = std::chrono::duration_cast<
                           std::chrono::duration<float>>(current_time_point - last_time_point_)
                           .count();

        last_time_point_ = current_time_point;

        auto fps = 1.0f / elapsed;
        display_fps((int)fps);
        float elapsed_since_start = std::chrono::duration_cast<
                                        std::chrono::duration<float>>(current_time_point - start_time_point_)
                                        .count();
        float angle = elapsed_since_start * (2 * 3.14f) / 2.0f;

        //printf("fps: %f\n", fps);

        int w = 0;
        int h = 0;
        emscripten_webgl_get_drawing_buffer_size(context_, &w, &h);

        camera_->width = w;
        camera_->height = h;

        //printf("context width = %d, height = %d\n", w, h);
        float ratio = w / (float)h;
        glViewport(0, 0, w, h);

        //glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        auto model = math::mat4_identity();
        auto rot_around_z = math::mat4_rot_around_axis({0, 0, 1}, angle);
        //model = math::mul(rot_around_z, model);
        auto view = camera_->get_view_matrix();
        auto projection = camera_->get_projection_matrix();
        model = math::transpose(model);
        view = math::transpose(view);
        projection = math::transpose(projection);

        math::vec3 light_pos{2.0f, 2.0f, 2.0f};
        glUseProgram(program);
        glUniformMatrix4fv(uniform_locations_["model"], 1, GL_FALSE, (const GLfloat *)model.data());
        glUniformMatrix4fv(uniform_locations_["view"], 1, GL_FALSE, (const GLfloat *)view.data());
        glUniformMatrix4fv(uniform_locations_["projection"], 1, GL_FALSE, (const GLfloat *)projection.data());
        glUniform3fv(uniform_locations_["camera_pos"], 1, (const GLfloat *)camera_->position.data());
        glUniform3fv(uniform_locations_["light_pos"], 1, (const GLfloat *)light_pos.data());
        glDrawArrays(GL_TRIANGLES, 0, obj_.vertices.size() / 3);
        emscripten_webgl_commit_frame();
    }
}; // namespace graphics

} // namespace graphics

extern "C" EMSCRIPTEN_KEEPALIVE int
on_pointer_down(int window_id, int button, int key_state, int x, int y)
{
    return windows[window_id]->get_input_handler()->on_pointer_down(button, key_state, x, y);
}

extern "C" EMSCRIPTEN_KEEPALIVE int on_pointer_up(int window_id, int button, int key_state, int x, int y)
{
    return windows[window_id]->get_input_handler()->on_pointer_up(button, key_state, x, y);
}

extern "C" EMSCRIPTEN_KEEPALIVE int on_pointer_move(int window_id, int button, int key_state, int x, int y)
{
    return windows[window_id]->get_input_handler()->on_pointer_move(button, key_state, x, y);
}

extern "C" EMSCRIPTEN_KEEPALIVE void on_wheel(int window_id, int delta)
{
    windows[window_id]->get_input_handler()->on_wheel(delta);
}