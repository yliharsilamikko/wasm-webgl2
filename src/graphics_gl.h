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
    "uniform mat4 MVP;\n"
    "in lowp vec3 vCol;\n"
    "in lowp vec2 vPos;\n"
    "out lowp vec3 i_color;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
    "    i_color = vCol;\n"
    "}\n";
static const char *fragment_shader_text =
    "#version 300 es\n"
    "in lowp vec3 i_color;\n"
    "out lowp vec4 o_color;\n"
    "void main()\n"
    "{\n"
    "    o_color = vec4(i_color, 1.0);\n"
    "}\n";

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