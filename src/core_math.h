#pragma once

#include <math.h>
#include <vector>
#include <array>

namespace math
{

const float pi = 3.14159265358979323846;

typedef std::array<float, 3> vec3;
typedef std::array<float, 4> vec4;
typedef std::array<float, 9> mat3;
typedef std::array<float, 16> mat4;

static void print(vec3 v)
{
    printf("[%f, %f, %f]\n", v[0], v[1], v[2]);
}

static void print(vec4 v)
{
    printf("[%f, %f, %f, %f]\n", v[0], v[1], v[2], v[3]);
}

static void print(mat3 m)
{
    printf("[%f, %f, %f]\n", m[0], m[1], m[2]);
    printf("[%f, %f, %f]\n", m[3], m[4], m[5]);
    printf("[%f, %f, %f]\n", m[6], m[7], m[8]);
}

static void print(mat4 m)
{
    printf("[%f, %f, %f, %f]\n", m[0], m[1], m[2], m[3]);
    printf("[%f, %f, %f, %f]\n", m[4], m[5], m[6], m[7]);
    printf("[%f, %f, %f, %f]\n", m[8], m[9], m[10], m[11]);
    printf("[%f, %f, %f, %f]\n", m[12], m[13], m[14], m[15]);
}

static bool equal(const vec3 &a, const vec3 &b, float max_error = 1.0e-6)
{
    for (int i = 0; i < 3; i++)
    {
        if (fabs(a[i] - b[i]) > max_error)
        {
            return false;
        }
    }
    return true;
}

static bool equal(const vec4 &a, const vec4 &b, float max_error = 1.0e-6)
{
    for (int i = 0; i < 4; i++)
    {
        if (fabs(a[i] - b[i]) > max_error)
        {
            return false;
        }
    }
    return true;
}

static bool equal(const mat3 &a, const mat3 &b, float max_error = 1.0e-6)
{
    for (int i = 0; i < 9; i++)
    {
        if (fabs(a[i] - b[i]) > max_error)
        {
            return false;
        }
    }
    return true;
}

static bool equal(const mat4 &a, const mat4 &b, float max_error = 1.0e-6)
{
    for (int i = 0; i < 16; i++)
    {
        if (fabs(a[i] - b[i]) > max_error)
        {
            return false;
        }
    }
    return true;
}

static mat3 mat3_identity()
{
    mat3 m{};
    m[0] = 1.0f;
    m[4] = 1.0f;
    m[8] = 1.0f;
    return m;
}

static mat4 mat4_identity()
{
    mat4 m{};
    m[0] = 1.0f;
    m[5] = 1.0f;
    m[10] = 1.0f;
    m[15] = 1.0f;
    return m;
}

static vec4 vec4_from_vec3(const vec3 &v, float added_value = 1.0f)
{
    return vec4{v[0], v[1], v[2], added_value};
}

static vec3 add(const vec3 &a, const vec3 &b)
{
    vec3 v;
    for (int i = 0; i < 3; i++)
    {
        v[i] = a[i] + b[i];
    }
    return v;
}

static vec4 add(const vec4 &a, const vec4 &b)
{
    vec4 v;
    for (int i = 0; i < 4; i++)
    {
        v[i] = a[i] + b[i];
    }
    return v;
}

static vec3 sub(const vec3 &a, const vec3 &b)
{
    vec3 v;
    for (int i = 0; i < 3; i++)
    {
        v[i] = a[i] - b[i];
    }
    return v;
}

static vec4 sub(const vec4 &a, const vec4 &b)
{
    vec4 v;
    for (int i = 0; i < 4; i++)
    {
        v[i] = a[i] - b[i];
    }
    return v;
}

static vec3 scale(const vec3 &v, float scalar)
{
    auto r = vec3{};
    for (int i = 0; i < 3; i++)
    {
        r[i] = v[i] * scalar;
    }
    return r;
}

static vec4 scale(const vec4 &v, float scalar)
{
    auto r = vec4{};
    for (int i = 0; i < 4; i++)
    {
        r[i] = v[i] * scalar;
    }
    return r;
}

static float dot(const vec3 &a, const vec3 &b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static float dot(const vec4 &a, const vec4 &b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

static float magnitude(const vec3 &v)
{
    return sqrt(dot(v, v));
}

static float magnitude(const vec4 &v)
{
    return sqrt(dot(v, v));
}

static vec3 normalize(const vec3 &v)
{
    auto mag = magnitude(v);
    return scale(v, 1.0f / mag);
}

static vec4 normalize(const vec4 &v)
{
    auto mag = magnitude(v);
    return scale(v, 1.0f / mag);
}

static vec3 inverse(const vec3 &v)
{
    return scale(v, -1.0f);
}
static vec4 inverse(const vec4 &v)
{
    return scale(v, -1.0f);
}

static vec3 cross(const vec3 &a, const vec3 &b)
{
    vec3 r;
    r[0] = a[1] * b[2] - a[2] * b[1];
    r[1] = a[2] * b[0] - a[0] * b[2];
    r[2] = a[0] * b[1] - a[1] * b[0];
    return r;
}

static mat3 sub(const mat3 &a, const mat3 &b)
{
    mat3 m;
    for (int i = 0; i < 9; i++)
    {
        m[i] = a[i] - b[i];
    }
    return m;
}

static mat4 sub(const mat4 &a, const mat4 &b)
{
    mat4 v;
    for (int i = 0; i < 16; i++)
    {
        v[i] = a[i] - b[i];
    }
    return v;
}

static vec3 row_vector(const mat3 &mat, const int row)
{
    return vec3{mat[3 * row], mat[3 * row + 1], mat[3 * row + 2]};
}
static vec4 row_vector(const mat4 &mat, const int row)
{
    return vec4{mat[4 * row], mat[4 * row + 1], mat[4 * row + 2], mat[4 * row + 3]};
}

static vec3 column_vector(const mat3 &mat, const int col)
{
    return vec3{mat[col], mat[col + 3], mat[col + 6]};
}
static vec4 column_vector(const mat4 &mat, const int col)
{
    return vec4{mat[col], mat[col + 4], mat[col + 8], mat[col + 12]};
}

static vec3 transform_point(const mat4 &m, const vec3 &v)
{
    vec3 r{};

    for (int i = 0; i < 3; i++)
    {
        r[i] = dot(row_vector(m, i), vec4_from_vec3(v));
    }
    return r;
}

static vec3 transform_direction(const mat4 &m, const vec3 &v)
{
    vec3 r{};
    auto rot_transform = m;
    rot_transform[3] = 0.0f;
    rot_transform[7] = 0.0f;
    rot_transform[11] = 0.0f;

    r = transform_point(rot_transform, v);

    return r;
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

static mat4 mul(const mat4 &m_a, const mat4 &m_b)
{
    auto m = mat4{};
    for (size_t row = 0; row < 4; row++)
    {
        auto row_a = row_vector(m_a, row);
        for (size_t col = 0; col < 4; col++)
        {
            auto col_b = column_vector(m_b, col);
            auto d = dot(row_a, col_b);
            m[4 * row + col] = d;
        }
    }
    return m;
}

mat3 transpose(const mat3 &m)
{
    mat3 t;
    for (int row = 0; row < 3; row++)
        for (int col = 0; col < 3; col++)
            t[3 * row + col] = m[3 * col + row];
    return t;
}
mat4 transpose(const mat4 &m)
{
    mat4 t;
    for (int row = 0; row < 4; row++)
        for (int col = 0; col < 4; col++)
            t[4 * row + col] = m[4 * col + row];
    return t;
}

mat4 mat4_translate(const vec3 &translation)
{
    mat4 m = mat4_identity();
    m[3] = translation[0];
    m[7] = translation[1];
    m[11] = translation[2];
    return m;
}

//transformations

static mat4 mat4_rot_around_axis(const vec3 &axis, float angle)
{
    // http://en.wikipedia.org/wiki/Rotation_matrix
    // Rotation matrix from axis and angle

    double cosO = cos(angle);
    double sinO = sin(angle);

    double ux = axis[0];
    double uy = axis[1];
    double uz = axis[2];

    mat4 m{};

    m[0] = cosO + ux * ux * (1 - cosO);
    m[1] = ux * uy * (1 - cosO) - uz * sinO;
    m[2] = ux * uz * (1 - cosO) + uy * sinO;
    m[3] = 0;

    m[4] = ux * uy * (1 - cosO) + uz * sinO;
    m[5] = cosO + uy * uy * (1 - cosO);
    m[6] = uy * uz * (1 - cosO) - ux * sinO;
    m[7] = 0;

    m[8] = uz * ux * (1 - cosO) - uy * sinO;
    m[9] = uz * uy * (1 - cosO) + ux * sinO;
    m[10] = cosO + uz * uz * (1 - cosO);
    m[11] = 0;

    m[12] = 0;
    m[13] = 0;
    m[14] = 0;
    m[15] = 1;

    return m;
}

// opengl related

// aspect = witdth / height
static mat4 mat4_perspective(float fov_v, float aspect, float near, float far)
{
    float const a = 1.f / tan(fov_v / 2.f);
    mat4 m{};

    m[0] = a / aspect;
    m[5] = a;
    m[10] = -((far + near) / (far - near));
    m[11] = -((2.f * far * near) / (far - near));
    m[14] = -1.f;

    return m;
}

static mat4 mat4_look_at(vec3 eye, vec3 center, vec3 up)
{
    mat4 m{};
    vec3 f = normalize(sub(center, eye));
    vec3 s = normalize(cross(f, up));
    vec3 t = cross(s, f);

    m[0] = s[0];
    m[1] = s[1];
    m[2] = s[2];
    m[3] = 0.f;

    m[4] = t[0];
    m[5] = t[1];
    m[6] = t[2];
    m[7] = 0.f;

    m[8] = -f[0];
    m[9] = -f[1];
    m[10] = -f[2];
    m[11] = 0.f;

    m[12] = 0.f;
    m[13] = 0.f;
    m[14] = 0.f;
    m[15] = 1.f;

    auto translation = mat4_translate(inverse(eye));
    m = mul(m, translation);

    return m;
}

static mat4 mat4_rotation(float pitch, float yaw, float roll)
{
    mat4 m{};

    float a = pitch;
    float b = yaw;
    float c = roll;

    m[0] = cos(a) * cos(b);
    m[1] = cos(a) * sin(b) * sin(c) - sin(a) * cos(c);
    m[2] = cos(a) * sin(b) * cos(c) + sin(a) * sin(c);
    m[3] = 0.f;

    m[4] = sin(a) * cos(b);
    m[5] = sin(a) * sin(b) * sin(c) + cos(a) * cos(c);
    m[6] = sin(a) * sin(b) * cos(c) - cos(a) * sin(c);
    m[7] = 0.f;

    m[8] = -sin(b);
    m[9] = cos(b) * sin(c);
    m[10] = cos(b) * cos(c);
    m[11] = 0.f;

    m[12] = 0.f;
    m[13] = 0.f;
    m[14] = 0.f;
    m[15] = 1.f;
    return m;
}

// smooth s-shape curve between 0 and 1
// inputs 0 and 1 do not map exactly to outputs 0 and 1
static float sigmoid_smooth(float t, float k = 20.0f)
{
    return 1.0f / (1 + exp(-k * (t - 0.5f)));
}

} // namespace math