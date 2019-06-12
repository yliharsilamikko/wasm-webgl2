#pragma once

#include "../core_math.h"
#include <math.h>
#include <stdio.h> //printf debug
class camera
{
public:
    math::vec3 position = {-10.0f, 10.0f, 10.0f};
    math::vec3 focus = {0.0f, 0.0f, 0.0f};
    math::vec3 up = {0.0f, 1.0f, 0.0f};

    // vertical fov
    float fov = 0.6f;
    float near = 0.1f;
    float far = 100.0f;

    int width = 0;
    int height = 0;

    // height / width
    float get_aspect()
    {
        return height / (float)width;
    }

    math::mat4 get_view_matrix()
    {
        return math::mat4_look_at(position, focus, up);
    }

    math::mat4 get_projection_matrix()
    {
        return math::mat4_perspective(fov, width / (float)height, near, far);
    }

    math::vec3 get_sight()
    {
        return math::normalize(math::sub(focus, position));
    }

    math::vec3 get_right()
    {
        auto sight = get_sight();
        return math::normalize(math::cross(sight, up));
    }

    // fix up direction
    void make_up_ortho()
    {
        auto right = get_right();
        auto sight = get_sight();
        up = math::normalize(math::cross(right, sight));
    }

    float get_focus_distance()
    {
        auto eye_to_focus = math::sub(focus, position);
        return math::magnitude(eye_to_focus);
    }

    void set_focus_distance(float distance)
    {
        auto sight = get_sight();
        focus = math::add(position, math::scale(sight, distance));
    }

    void rotate_around_focus(float pitch, float yaw, float roll)
    {
        auto right = get_right();
        auto sight = get_sight();

        auto rotation_pitch = math::mat4_rot_around_axis(right, pitch);
        auto rotation_yaw = math::mat4_rot_around_axis(up, -yaw);
        auto rotation_roll = math::mat4_rot_around_axis(sight, roll);

        auto translate_to_origin = math::mat4_translate(math::inverse(focus));
        auto rotation = math::mul(rotation_pitch, math::mul(rotation_yaw, rotation_roll));
        auto translate_back = math::mat4_translate(focus);

        auto transformation = math::mul(translate_back, math::mul(rotation, translate_to_origin));

        position = math::transform_point(transformation, position);
        up = math::normalize(math::transform_direction(transformation, up));
        make_up_ortho();
    }

    void pan(float x, float y)
    {
        float focus_dist = get_focus_distance();

        auto aspect = get_aspect();
        float fov_h = fov / aspect;
        float fov_v = fov;

        float angle_h = fov_h * x / (float)width;
        float angle_v = fov_v * y / (float)height;

        float delta_right = tan(angle_h) * focus_dist;
        float delta_up = tan(angle_v) * focus_dist;

        auto right = get_right();

        auto eye_translation = math::add(math::scale(right, delta_right), math::scale(up, delta_up));

        position = math::add(position, eye_translation);
        focus = math::add(focus, eye_translation);
        make_up_ortho();
    }

    // zoom into screen position
    void zoom(float x, float y, float delta)
    {
        auto focus_dist = get_focus_distance();

        auto right = get_right();
        auto sight = get_sight();

        float zoom_factor = 1;
        if (delta > 0)
        {
            zoom_factor = 1.25;
        }
        else
        {
            zoom_factor = 0.8;
        }

        float new_focus_dist = zoom_factor * focus_dist;
        auto aspect = get_aspect();
        float fov_h = fov / aspect;
        float fov_v = fov;

        float angle_h = fov_h * x / (float)width;
        float angle_v = fov_v * y / (float)height;

        float delta_right = tan(angle_h) * focus_dist * (1.0f - zoom_factor);
        float delta_up = tan(angle_v) * focus_dist * (1.0f - zoom_factor);
        float delta_sight = focus_dist - new_focus_dist;
        math::vec3 cam_translation;
        cam_translation = math::add(math::scale(right, delta_right), math::add(math::scale(up, delta_up), math::scale(sight, delta_sight)));

        position = math::add(position, cam_translation);
        focus = math::add(position, math::scale(sight, new_focus_dist));
        make_up_ortho();
    }
};