#include "core_math.h"
#include <algorithm>
#include "graphics/camera.h"

#include <stdio.h> //printf debug

class input_handler
{
private:
    math::vec3 last_position_{};
    math::vec3 down_position_{};

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
        math::vec3 position{view_x, view_y, 0};
        last_position_ = position;
        down_position_ = position;
        return 1; //set capture
    }

    int on_pointer_up(int button, int key_state, int x, int y)
    {
        float view_x = (float)x - camera_->width / 2.0f;
        float view_y = camera_->height / 2.0f - (float)y;
        math::vec3 position{view_x, view_y, 0};
        last_position_ = position;
        return 0; //set capture
    }

    int on_pointer_move(int buttons, int key_state, int x, int y)
    {

        float view_x = (float)x - camera_->width / 2.0f;
        float view_y = camera_->height / 2.0f - (float)y;
        math::vec3 position{view_x, view_y, 0};
        math::vec3 move = math::sub(position, last_position_);
        last_position_ = position;

        float move_len = math::magnitude(move);
        if (move_len < 0.5f)
        {
            return -1;
        }
        if (buttons == 2)
        {
            // moving mouse right moves camera left
            camera_->pan(-move[0], -move[1]);
        }
        if (buttons == 4)
        {
            float pitch = move[1] / 100.0f;
            float yaw = move[0] / 100.0f;

            math::vec3 z{0, 0, 1};
            auto roll_tan = math::normalize(math::cross(position, z));
            auto roll_len = math::dot(roll_tan, move);
            auto pos_len = math::magnitude(position);
            // smooth roll effect near to the center off screen
            float view_size = (float)std::max(camera_->width, camera_->height);
            float smooth = math::sigmoid_smooth(pos_len / (view_size / 2.0f));
            auto roll_ratio = smooth * abs(roll_len / move_len);
            auto roll = -roll_ratio * atan(roll_len / pos_len);

            pitch = (1.0f - roll_ratio) * pitch;
            yaw = (1.0f - roll_ratio) * yaw;

            camera_->rotate_around_focus(pitch, yaw, roll);
        }
        return -1;
    }

    void on_wheel(int delta)
    {
        camera_->zoom(last_position_[0], last_position_[1], delta);
    }
};