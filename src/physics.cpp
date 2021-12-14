#include "physics.h"

#include <iostream>
#include <cmath>
#include <chrono>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "world.h"

extern bool key_state[400];

namespace Physics {
    const static float MAX_SPEED = 0.15;
    const static float MU = 0.7;
    const static float GRAVITY_ACCELERATION = -2.8;
    const static float JUMP_INITIAL_ACCELERATION = 20.0;
    const static float RUN_INITAIL_ACCELERATION = 1;
    const static float STEVE_HEIGHT_FEET_TO_EYES = 1.7;

    const static float BOX_DX = 0.35;
    const static float BOX_DY = 0.1;
    const static float BOX_DZ = 0.35;

    float g_x = 10;
    float g_y = 10;
    float g_z = 10;
    float g_yaw = -45;
    float g_pitch = 0;

    float lookX;
    float lookY;
    float lookZ;

    // boundary
    float pos_x, pos_y, pos_z;
    float neg_x, neg_y, neg_z;

    int detect_order_xz[4][2] = {
            {-1, 0},
            {1,  0},
            {0,  1},
            {0,  -1}
    };

    // gravity
    glm::vec3 velocity;
    glm::vec3 gravity;
    glm::vec3 acceleration;

    // time
    auto last_time = std::chrono::system_clock::now();
    float DELTA_T;

    void init() {
        last_time = std::chrono::system_clock::now();
        velocity = glm::vec3();
        gravity = glm::vec3(0.0, GRAVITY_ACCELERATION, 0.0);
        acceleration = glm::vec3();
    }

    void update() {
        auto time_now = std::chrono::system_clock::now();
        DELTA_T = std::chrono::duration_cast<std::chrono::milliseconds>(time_now - last_time).count();
        DELTA_T /= 1000000.0;
        if(DELTA_T > 0.05) DELTA_T = 0.05;

        update_acceleration_speed();
        update_position();
    }

    void update_acceleration_speed() {
        float _yaw = g_yaw / 180 * M_PI;

        int r_x = round(g_x);
        int r_y_eye = round(g_y);
        int r_y_feet = round(g_y - STEVE_HEIGHT_FEET_TO_EYES);
        int r_z = round(g_z);

        // step1: set acceleration
        glm::vec3 acc;
        glm::vec3 F;

        bool collision[6];
        memset(collision, 0, sizeof(collision));
        set_boundary(g_x, g_y, g_z);
        for (int i = 0; i < 4; i++) {
            if (collision_XZ(g_x, g_z, r_x + detect_order_xz[i][0], r_y_eye, r_z + detect_order_xz[i][1])
                || collision_XZ(g_x, g_z, r_x + detect_order_xz[i][0], r_y_feet, r_z + detect_order_xz[i][1])) {
                collision[i + 2] = true;
            }
        }

        if (collision_Y(g_y, r_x, r_y_eye + 1, r_z)) collision[0] = true;
        if (collision_Y(g_y - STEVE_HEIGHT_FEET_TO_EYES, r_x, r_y_feet, r_z)) collision[1] = true;
        if (collision_Y(g_y - STEVE_HEIGHT_FEET_TO_EYES, r_x, r_y_feet - 1, r_z)) collision[1] = true;

        if (!collision[1]) acc.y = GRAVITY_ACCELERATION;

        if (key_state[GLFW_KEY_W]) {
            F.x += sin(_yaw);
            F.z += -cos(_yaw);
        }
        if (key_state[GLFW_KEY_S]) {
            F.x -= sin(_yaw);
            F.z -= -cos(_yaw);
        }
        if (key_state[GLFW_KEY_A]) {
            F.x += -sin(_yaw + M_PI / 2);
            F.z += +cos(_yaw + M_PI / 2);
        }
        if (key_state[GLFW_KEY_D]) {
            F.x -= -sin(_yaw + M_PI / 2);
            F.z -= +cos(_yaw + M_PI / 2);
        }
        if (F.x != 0 && F.y != 0 && F.z != 0) F = glm::normalize(F);
        acc += F * RUN_INITAIL_ACCELERATION;

        acceleration.x = acc.x;
        acceleration.y = acc.y;
        acceleration.z = acc.z;

        //step2: speed
        if(collision[1] && key_state[GLFW_KEY_SPACE]) {
            velocity.y = JUMP_INITIAL_ACCELERATION * DELTA_T;
        }

        velocity += acceleration * DELTA_T;

        if (collision[0] && velocity.y > 0) {
            velocity.y = 0;
        }
        if (collision[1] && velocity.y < 0) {
            velocity.y = 0;
        }
        if (collision[2] && velocity.x < 0) {
            velocity.x = 0;
        }
        if (collision[3] && velocity.x > 0) {
            velocity.x = 0;
        }
        if (collision[4] && velocity.z > 0) {
            velocity.z = 0;
        }
        if (collision[5] && velocity.z < 0) {
            velocity.z = 0;
        }

        if (acc.x == 0 && acc.z == 0) {
            velocity.x *= MU;
            velocity.z *= MU;
        }


        if (velocity.y < -0.5) velocity.y = -0.5;
        glm::vec3 vh = velocity;
        vh.y = 0;
        float len;
        len = glm::length(vh);
        if (len > 0.2) {
            vh *= 0.2 / len;
            velocity.x = vh.x;
            velocity.z = vh.z;
        }
    }

    void update_position() {
        float next_x = g_x + velocity.x;
        float next_y = g_y + velocity.y;
        float next_z = g_z + velocity.z;

        int r_x = round(next_x);
        int r_y_eye = round(next_y);
        int r_y_feet = round(next_y - STEVE_HEIGHT_FEET_TO_EYES);
        int r_z = round(next_z);

        bool collision[6];
        memset(collision, 0, sizeof(collision));
        set_boundary(next_x, next_y, next_z);
        for (int i = 0; i < 4; i++) {
            if (collision_XZ(g_x, g_z, r_x + detect_order_xz[i][0], r_y_eye, r_z + detect_order_xz[i][1])
                || collision_XZ(g_x, g_z, r_x + detect_order_xz[i][0], r_y_feet, r_z + detect_order_xz[i][1])) {
                collision[i + 2] = true;
            }
        }

        if(collision_Y(next_y, r_x, r_y_eye + 1, r_z) && velocity.y > 0) {
            next_y = r_y_eye + 1 -0.5 - BOX_DY;
        } else if (collision_Y(next_y, r_x, r_y_eye, r_z) && velocity.y > 0) {
            next_y = r_y_eye -0.5 - BOX_DY;
        }
        if (collision_Y(next_y - STEVE_HEIGHT_FEET_TO_EYES, r_x, r_y_feet - 1, r_z) && velocity.y < 0) {
            next_y = r_y_feet - 1 + 0.5 + BOX_DY + STEVE_HEIGHT_FEET_TO_EYES;
        } else if (collision_Y(next_y - STEVE_HEIGHT_FEET_TO_EYES, r_x, r_y_feet, r_z) && velocity.y < 0) {
            next_y = r_y_feet + 0.5 + BOX_DY + STEVE_HEIGHT_FEET_TO_EYES;
        }

        if ((collision[2] && velocity.x < 0) || (collision[3] && velocity.x > 0)) {
            next_x = g_x;
            velocity.x = 0;
        }

        if ((collision[4] && velocity.z > 0) || (collision[3] && velocity.z < 0)) {
            next_z = g_z;
            velocity.z = 0;
        }

        g_x = next_x;
        g_y = next_y;
        g_z = next_z;
    }

    void set_boundary(float x, float y, float z) {
        pos_x = x + BOX_DX;
        neg_x = x - BOX_DX;
        pos_y = y + BOX_DY;
        neg_y = y - BOX_DY - STEVE_HEIGHT_FEET_TO_EYES;
        pos_z = z + BOX_DZ;
        neg_z = z - BOX_DZ;
    }

    bool collision_XZ(float next_x, float next_z, int block_x, int block_y, int block_z) {
        if (World::have_block(block_x, block_y, block_z)) {
            if (((block_x - 0.5 <= pos_x) && (pos_x <= block_x + 0.5)) ||
                ((block_x - 0.5 <= neg_x) && (neg_x <= block_x + 0.5)) &&
                ((block_z - 0.5 <= pos_z) && (pos_z <= block_z + 0.5)) ||
                ((block_z - 0.5 <= neg_z) && (neg_z <= block_z + 0.5))) {
                return true;
            }
        }
        return false;
    }

    bool collision_Y(float next_y, int block_x, int block_y, int block_z) {
        if (World::have_block(block_x, block_y, block_z) != 0) {
            if (((block_y - 0.5) <= pos_y && (pos_y <= block_y + 0.5)) ||
                ((block_y - 0.5 <= neg_y) && (neg_y <= block_y + 0.5))) {
                return true;
            }
        }

        return false;
    }
}