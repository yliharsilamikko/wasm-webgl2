#pragma once

#include <emscripten.h>
//#define GLFW_INCLUDE_ES3
//#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>

#include <stdio.h> //printf
namespace graphics
{

static const char *vertex_shader_text =
    "#version 300 es\n"
    "uniform lowp mat4 model;\n"
    "uniform lowp mat4 view;\n"
    "uniform lowp mat4 projection;\n"
    "in lowp vec3 v_pos;\n"
    "in lowp vec3 v_norm;\n"
    "in lowp vec2 v_uv;\n"
    "out lowp vec3 f_pos;\n"
    "out lowp vec3 f_norm;\n"
    "out lowp vec2 f_uv;\n"
    "void main()\n"
    "{\n"
    "   f_pos = vec3(model * vec4(v_pos, 1.0));\n"
    "   f_norm = mat3(transpose(inverse(model))) * v_norm;\n"
    "   f_uv = v_uv;\n"
    "   gl_Position = projection * view * vec4(f_pos, 1.0);\n"
    "}\n";

static const char *fragment_shader_text =
    "#version 300 es\n"
    "uniform lowp vec3 camera_pos;\n"
    "uniform lowp vec3 light_pos;\n"
    "in lowp vec3 f_pos;\n"
    "in lowp vec3 f_norm;\n"
    "in lowp vec2 f_uv;\n"
    "out lowp vec4 color;\n"
    "void main()\n"
    "{\n"
    "   lowp vec3 light_color = vec3(1.0, 1.0, 1.0);\n"
    "   lowp vec3 object_color = vec3(0.4, 0.5, 0.6);\n"
    "   // ambient\n"
    "   lowp float ambient_strength = 0.6;\n"
    "   lowp vec3 ambient = ambient_strength * light_color;\n"
    "   \n"
    "   // diffuse\n"
    "   lowp vec3 norm = normalize(f_norm);\n"
    "   lowp vec3 light_dir = normalize(light_pos - f_pos);\n"
    "   lowp float diff = max(dot(norm, light_dir), 0.0);\n"
    "   lowp vec3 diffuse = diff * light_color;\n"
    "   \n"
    "   // specular\n"
    "   lowp float specular_strength = 1.5;\n"
    "   lowp vec3 view_dir = normalize(camera_pos - f_pos);\n"
    "   lowp vec3 reflect_dir = reflect(-light_dir, norm);\n"
    "   lowp float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 4.0);\n"
    "   lowp vec3 specular = specular_strength * spec * light_color;\n"
    "   \n"
    "   lowp vec3 result = (ambient + diffuse + specular) * object_color;\n"
    "   color = vec4(result, 1.0);\n"
    "}\n";

// static const char *fragment_shader_text =
//     "#version 300 es\n"
//     "uniform lowp vec3 light_world;\n"
//     "in lowp vec3 position_world;\n"
//     "in lowp vec3 normal_camera;\n"
//     "in lowp vec3 camera_direction_camera;\n"
//     "in lowp vec3 light_direction_camera;\n"
//     "in lowp vec2 fragment_uv;\n"
//     "out lowp vec4 color;\n"
//     "void main()\n"
//     "{\n"
//     "   lowp vec3 light_color = vec3(1.0, 1.0, 1.0);\n"
//     "   lowp float light_power = 10.0f;\n"
//     "   lowp vec3 diffuse_color = vec3(0.5,0.5,0.5);\n"
//     "   lowp vec3 ambient_color = vec3(0.7,0.8,0.7) * diffuse_color;\n"
//     "   lowp vec3 specular_color = vec3(0.7,0.7,0.5);\n"
//     "   lowp float distance = length( light_world - position_world );\n"
//     "   lowp vec3 n = normalize( normal_camera );\n"
//     "   lowp vec3 l = normalize( light_direction_camera );\n"
//     "   lowp float cos_theta = clamp( dot( n,l ), 0.0, 1.0 );\n"
//     "   lowp vec3 e = normalize(camera_direction_camera);\n"
//     "   lowp vec3 r = reflect(-l,n);\n"
//     "   lowp float cos_alpha = clamp( dot( e,r ), 0.0, 1.0 );\n"
//     "   color = \n"
//     "   	vec4(ambient_color +\n"
//     "   	diffuse_color * light_color * light_power * cos_theta / (distance*distance) +\n"
//     "   	specular_color * light_color * light_power * pow(cos_alpha,5.0) / (distance*distance), 1.0);\n"
//     "}\n";

static int check_compiled(GLuint shader)
{
    GLint success = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (success == GL_FALSE)
    {
        GLint max_len = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &max_len);

        GLchar err_log[max_len];
        glGetShaderInfoLog(shader, max_len, &max_len, &err_log[0]);
        glDeleteShader(shader);

        printf("Shader compilation failed: %s\n", err_log);
    }

    return success;
}

static int check_linked(GLuint program)
{
    GLint success = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &success);

    if (success == GL_FALSE)
    {
        GLint max_len = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &max_len);

        GLchar err_log[max_len];
        glGetProgramInfoLog(program, max_len, &max_len, &err_log[0]);

        printf("Program linking failed: %s\n", err_log);
    }

    return success;
}
} // namespace graphics