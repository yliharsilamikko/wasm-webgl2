#pragma once

#include <vector>
#include <array>

namespace math
{

const float pi = 3.14159265358979323846;

typedef std::array<float, 3> vec3;
typedef std::array<float, 4> vec4;
typedef std::array<float, 9> mat3;
typedef std::array<float, 16> mat4;

static float dot(const vec3 &a, const vec3 &b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static float dot(const vec4 &a, const vec4 &b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

static vec3 row_vector(const mat3 &mat, const int row)
{
    return vec3{mat[3 * row], mat[3 * row + 1], mat[3 * row + 2]};
}
static vec4 row_vector(const mat4 &mat, const int row)
{
    return vec4{mat[3 * row], mat[3 * row + 1], mat[3 * row + 2], mat[3 * row + 3]};
}

static vec3 column_vector(const mat3 &mat, const int col)
{
    return vec3{mat[col], mat[col + 3], mat[col + 6]};
}
static vec4 column_vector(const mat4 &mat, const int col)
{
    return vec4{mat[col], mat[col + 4], mat[col + 8], mat[col + 12]};
}

static mat3 mul(const mat3 &mat, float scalar)
{
    auto m = mat3{};
    for (int i = 0; i < 9; i++)
    {
        m[i] = mat[i] * scalar;
    }
    return m;
}

static mat4 mul(const mat4 &mat, float scalar)
{
    auto m = mat4{};
    for (int i = 0; i < 16; i++)
    {
        m[i] = mat[i] * scalar;
    }
    return m;
}

static mat3 div(const mat3 &mat, float scalar)
{
    auto m = mat3{};
    for (int i = 0; i < 9; i++)
    {
        m[i] = mat[i] / scalar;
    }
    return m;
}

static mat4 div(const mat4 &mat, float scalar)
{
    auto m = mat4{};
    for (int i = 0; i < 16; i++)
    {
        m[i] = mat[i] / scalar;
    }
    return m;
}

static mat3 mul(const mat3 &mat_a, const mat3 &mat_b)
{
    auto m = mat3{};
    for (size_t row = 0; row < 3; row++)
    {
        auto row_vec = row_vector(m, row);
        for (size_t col = 0; col < 3; col++)
        {
            auto col_vec = row_vector(m, row);
            auto d = dot(row_vec, col_vec);
            m[3 * row + col] = d;
        }
    }
    return m;
}

static mat4 mul(const mat4 &mat_a, const mat4 &mat_b)
{
    auto m = mat4{};
    for (size_t row = 0; row < 4; row++)
    {
        auto row_vec = row_vector(m, row);
        for (size_t col = 0; col < 4; col++)
        {
            auto col_vec = row_vector(m, row);
            auto d = dot(row_vec, col_vec);
            m[4 * row + col] = d;
        }
    }
    return m;
}

} // namespace math