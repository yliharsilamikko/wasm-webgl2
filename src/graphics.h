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

#include "graphics_gl.h"
#include "linmath.h"

namespace graphics
{
class window;
}

namespace
{
std::map<std::string, graphics::window *> windows;
}

static void output_error(int error, const char *msg)
{
    printf("Error: %s\n", msg);
}

namespace graphics
{

struct vertex
{
    float x, y;
    float r, g, b;
};

class window
{

private:
    std::string window_id_;
    std::vector<float> vertices_;
    EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context_;

    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    std::chrono::system_clock::time_point last_time_point_;
    std::chrono::system_clock::time_point start_time_point_;

    vertex vertices[3] = {
        {-0.6f, -0.4f, 1.f, 0.f, 0.f},
        {0.6f, -0.4f, 0.f, 1.f, 0.f},
        {0.f, 0.6f, 0.f, 0.f, 1.f}};

    float avg_fps_ = 0.0;

public:
    window(const std::string &canvas_id)
    {
        window_id_ = canvas_id;
        windows[canvas_id] = this;
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

    void display_fps(float fps)
    {
        float n_samples = 10.0f;
        avg_fps_ -= avg_fps_ / n_samples;
        avg_fps_ += fps / n_samples;
        EM_ASM(
            var fps_div = document.getElementById("fps");
            fps_div.innerHTML = "FPS: " + $0;, (int)avg_fps_);
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
            },
            window_id_.c_str());
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

        glGenBuffers(1, &vertex_buffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        init_shaders();
        mvp_location = glGetUniformLocation(program, "MVP");
        vpos_location = glGetAttribLocation(program, "vPos");
        vcol_location = glGetAttribLocation(program, "vCol");
        glEnableVertexAttribArray(vpos_location);
        glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                              sizeof(float) * 5, (void *)0);
        glEnableVertexAttribArray(vcol_location);
        glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                              sizeof(float) * 5, (void *)(sizeof(float) * 2));
    }

    void init_js_callbacks()
    {

        // Set Javascript callbacks
        EM_ASM(
            var canvas_id = UTF8ToString($0);

            var canv = document.getElementById(canvas_id);

            console.log("canvas id: " + canvas_id + "\n");

            canv.onpointerdown = function(e) {
            canv.setPointerCapture(e.pointerId);
            console.log("onpointerdown: " + e.buttons + "\n"); };

            canv.onpointerup = function(e) {
            canv.releasePointerCapture(e.pointerId);
            console.log("onpointerup: " + e.buttons + "\n"); };

            canv.oncontextmenu = function(e){e.preventDefault()};

            canv.onpointermove = function(e) {
            //e.ctrlKey
            //e.shiftKey
            //e.altKey
            //e.metaKey
            console.log("onpointermove: " + e.buttons + "\n");
            console.log("X:" + e.clientX + ", Y:" + e.clientY + "\n"); };

            canv.onresize = function() {
            canv.width = canv.clientWidth;
            canv.height = canv.clientHeight; };
            ,
            window_id_.c_str());
    }

    void init_shaders()
    {
        vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
        glCompileShader(vertex_shader);
        check_compiled(vertex_shader);

        fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
        glCompileShader(fragment_shader);
        check_compiled(fragment_shader);

        program = glCreateProgram();
        glAttachShader(program, vertex_shader);
        glAttachShader(program, fragment_shader);
        glLinkProgram(program);
        check_linked(program);
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

        printf("fps: %f\n", fps);

        float ratio;
        mat4x4 m, p, mvp;
        int w = 0;
        int h = 0;
        emscripten_webgl_get_drawing_buffer_size(context_, &w, &h);
        printf("context width = %d, height = %d\n", w, h);
        ratio = w / (float)h;
        glViewport(0, 0, w, h);
        //glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        mat4x4_identity(m);
        mat4x4_rotate_Z(m, m, angle);
        mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mat4x4_mul(mvp, p, m);
        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat *)mvp);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        emscripten_webgl_commit_frame();
    }
}; // namespace graphics

} // namespace graphics