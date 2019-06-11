#pragma once

#include "../linmath.h"
#include "../core_math.h"
#include <math.h>
#include <stdio.h> //printf debug
class camera
{
public:
    //math::vec3 position = {0.0f, 0.0f, 10.0f};
    vec3 position = {0.0f, 0.0f, 10.0f};
    vec3 focus = {0.0f, 0.0f, 0.0f};
    vec3 up = {0.0f, 1.0f, 0.0f};

    float fov = 0.5f;
    float near = 0.1f;
    float far = 25.0f;

    int width = 0;
    int height = 0;

    void get_view_matrix(mat4x4 view)
    {
        mat4x4_look_at(view, position, focus, up);
    }

    void get_projection_matrix(mat4x4 projection)
    {
        mat4x4_perspective(projection, fov, width / (float)height, near, far);
    }

    void get_sight(vec3 sight)
    {
        vec3_sub(sight, focus, position);
        vec3_norm(sight, sight);
    }

    void get_right(vec3 right)
    {
        vec3 sight;
        get_sight(sight);
        vec3_mul_cross(right, sight, up);
    }

    float get_focus_distance()
    {
        vec3 eye_to_focus;
        vec3_sub(eye_to_focus, focus, position);
        return vec3_len(eye_to_focus);
    }

    void rotate_around_focus(vec2 translation)
    {
        printf("rotate_around_focus\n");

        float xangle = translation[1] / 100.0f;
        float yangle = translation[0] / 100.0f;
        float xctr = (float)width / 2.0f;
        float roll = (translation[0] - xctr) * xangle / xctr;

        printf("xangle: %f\n", xangle);
        printf("yangle: %f\n", yangle);
        printf("xctr: %f\n", xctr);
        printf("roll: %f\n", roll);

        vec3 sight;
        get_sight(sight);
        vec3 right;
        get_right(right);

        mat4x4 transformation;
        mat4x4_rotate_around_coordsys(
            transformation,
            position,
            right,
            up,
            sight,
            xangle,
            yangle,
            roll);

        vec4 old_pos;
        old_pos[0] = position[0];
        old_pos[1] = position[1];
        old_pos[2] = position[2];
        old_pos[3] = 1.0f;
        vec4 new_pos;
        mat4x4_mul_vec4(new_pos, transformation, old_pos);
        printf("transforamtion:\n%f %f %f")

            position[0] = new_pos[0];
        position[1] = new_pos[1];
        position[2] = new_pos[2];
    }

    void pan(vec2 translation)
    {
        printf("pan\n");

        float focus_distance = get_focus_distance();
        printf("focus_distance: %f\n", focus_distance);

        float ratio = height / (float)width;
        printf("ratio: %f\n", ratio);

        float tfov = tan(fov / 2.0f);
        printf("tfov: %f\n", tfov);

        float dist = 2.0f * tfov * focus_distance;
        printf("dist: %f\n", dist);

        float tup = dist * ratio * translation[1] / (float)height;
        printf("tup: %f\n", tup);

        float tright = dist * translation[0] / (float)width;
        printf("tright: %f\n", tright);

        vec3 right;
        get_right(right);
        vec3 eye_translation;
        eye_translation[0] = tright * right[0] + tup * up[0];
        eye_translation[1] = tright * right[1] + tup * up[1];
        eye_translation[2] = tright * right[2] + tup * up[2];

        printf("pan translation: %f %f\n", translation[0], translation[1]);
        printf("pan eye_translation: %f %f %f\n", eye_translation[0], eye_translation[1], eye_translation[2]);
        vec3_add(position, position, eye_translation);
        vec3_add(focus, focus, eye_translation);
    }
};