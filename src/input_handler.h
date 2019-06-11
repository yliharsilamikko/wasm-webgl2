#include "linmath.h"
#include <algorithm>
#include "graphics/camera.h"

#include <stdio.h> //printf debug

class input_handler
{
private:
    vec2 last_position_ = {0.0f, 0.0f};
    vec2 down_position_;

    int button_state_ = 0;
    int modifier_state_ = 0;

    camera *camera_;

public:
    input_handler(camera *cam)
    {
        camera_ = cam;
    }

    int on_pointer_down(int button, int key_state, int x, int y)
    {
        float view_x = (float)x - camera_->width / 2.0f;
        float view_y = camera_->height / 2.0f - (float)y;
        vec2 position{view_x, view_y};
        vec2_set(last_position_, position);
        vec2_set(down_position_, position);
        return 1; //set capture
    }

    int on_pointer_up(int button, int key_state, int x, int y)
    {
        float view_x = (float)x - camera_->width / 2.0f;
        float view_y = camera_->height / 2.0f - (float)y;
        vec2 position{view_x, view_y};
        vec2_set(last_position_, position);
        return 0; //set capture
    }

    int on_pointer_move(int buttons, int key_state, int x, int y)
    {

        float view_x = (float)x - camera_->width / 2.0f;
        float view_y = camera_->height / 2.0f - (float)y;
        vec2 position{view_x, view_y};
        vec2 translation;
        vec2_sub(translation, position, last_position_);
        vec2_set(last_position_, position);
        //printf("input_handler::on_pointer_move %d %d %f %f\n", buttons, key_state, view_x, view_y);

        float movement_length = vec2_len(translation);
        if (movement_length < 0.5f)
        {
            return -1;
        }
        if (buttons == 2)
        {
            vec2_scale(translation, translation, -1.0f);
            camera_->pan(translation);
        }
        if (buttons == 4)
        {
            vec2_scale(translation, translation, -1.0f);
            camera_->rotate_around_focus(translation);
        }
        return -1;
    }
};
