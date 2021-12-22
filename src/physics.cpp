#include "physics.h"

#include <iostream>
#include <cmath>
#include <chrono>

#include "world.h"

extern bool key_state[400];

namespace Physics {
    const static double MAX_SPEED = 0.15;
    const static double MU = 0.7;
    const static double GRAVITY_ACCELERATION = -2.8;
    const static double JUMP_INITIAL_ACCELERATION = 31.0;
    const static double RUN_INITAIL_ACCELERATION = 1;
    const static double STEVE_HEIGHT_FEET_TO_EYES = 1.7;

    const static double BOX_DX = 0.3;
    const static double BOX_DY = 0.1;
    const static double BOX_DZ = 0.3;

    double g_x = 10;
    double g_y = 60;
    double g_z = 10;
    double g_yaw = -45;
    double g_pitch = 0;

    double lookX;
    double lookY;
    double lookZ;

    int holding_block = id_grass;

    // boundary
    double pos_x, pos_y, pos_z;
    double neg_x, neg_y, neg_z;

    int look_at_x;
    int look_at_y;
    int look_at_z;
    int look_at_face;

    float g_fov = 60.0;

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
    double DELTA_T;

    void init() {
        last_time = std::chrono::system_clock::now();
        velocity = glm::vec3();
        gravity = glm::vec3(0.0, GRAVITY_ACCELERATION, 0.0);
        acceleration = glm::vec3();
    }

    void update() {
        DELTA_T = 0.01;

        update_acceleration_speed();
        update_position();
    }

    void update_acceleration_speed() {
        double _yaw = g_yaw / 180 * M_PI;

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
        acc += F * (float) RUN_INITAIL_ACCELERATION;

        acceleration.x = acc.x;
        acceleration.y = acc.y;
        acceleration.z = acc.z;

        //step2: speed
        if (collision[1] && key_state[GLFW_KEY_SPACE]) {
            velocity.y = JUMP_INITIAL_ACCELERATION * DELTA_T;
        }

        velocity += acceleration * (float) DELTA_T;

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
        double len;
        len = glm::length(vh);
        if (len > 0.2) {
            vh *= 0.2 / len;
            velocity.x = vh.x;
            velocity.z = vh.z;
        }
    }

    void update_position() {
        if (key_state[GLFW_KEY_R]) {
            g_x = 10;
            g_y = 60;
            g_z = 10;
        }


        double next_x = g_x + velocity.x;
        double next_y = g_y + velocity.y;
        double next_z = g_z + velocity.z;

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

        if (collision_Y(next_y, r_x, r_y_eye + 1, r_z) && velocity.y > 0) {
            next_y = r_y_eye + 1 - 0.5 - BOX_DY;
        } else if (collision_Y(next_y, r_x, r_y_eye, r_z) && velocity.y > 0) {
            next_y = r_y_eye - 0.5 - BOX_DY;
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

    void set_boundary(double x, double y, double z) {
        pos_x = x + BOX_DX;
        neg_x = x - BOX_DX;
        pos_y = y + BOX_DY;
        neg_y = y - BOX_DY - STEVE_HEIGHT_FEET_TO_EYES;
        pos_z = z + BOX_DZ;
        neg_z = z - BOX_DZ;
    }

    bool collision_XZ(double next_x, double next_z, int block_x, int block_y, int block_z) {
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

    bool collision_Y(double next_y, int block_x, int block_y, int block_z) {
        if (World::have_block(block_x, block_y, block_z) != 0) {
            if (((block_y - 0.5) <= pos_y && (pos_y <= block_y + 0.5)) ||
                ((block_y - 0.5 <= neg_y) && (neg_y <= block_y + 0.5))) {
                return true;
            }
        }

        return false;
    }


    void look_at(void) {
        double len = 7.0;
        double yaw = Physics::g_yaw / 180.0 * M_PI;
        double pitch = Physics::g_pitch / 180 * M_PI;

        glm::vec3 start = glm::vec3(g_x, g_y, g_z);
        glm::vec3 end = glm::vec3(
                g_x + len * cos(pitch) * sin(yaw),
                g_y + len * sin(pitch),
                g_z - len * cos(pitch) * cos(yaw)
        );

        std::optional<glm::vec3> at = ray_trace_blocks(start, end);

        if (at != std::nullopt) {
            look_at_x = at.value().x;
            look_at_y = at.value().y;
            look_at_z = at.value().z;
            ray_trace_face();
        } else {
            look_at_face = -1;
        }
    }

    void place_block() {
        if (look_at_face != -1) {
            int x = look_at_x;
            int y = look_at_y;
            int z = look_at_z;

            if (look_at_face == 0) y += 1;
            else if (look_at_face == 1) y -= 1;
            else if (look_at_face == 2) x -= 1;
            else if (look_at_face == 3) x += 1;
            else if (look_at_face == 4) z += 1;
            else if (look_at_face == 5) z -= 1;


            if (!World::have_block(x, y, z)) {
                if (x >= 0 && z >= 0 && y >= 0 && y < 64) {
                    int chunk_x = x / 16;
                    int chunk_z = z / 16;
                    int in_x = x % 16;
                    int in_z = z % 16;

                    World::chunk *c = World::world_map[chunk_x][chunk_z];

                    c->block[in_x][y][in_z] = holding_block;

                    World::update_light(x, y, z);
                    std::cout<< "light: " << World::get_light(x, y, z) << std::endl;
                }
            }
        }
    }

    void break_block() {
        if (World::have_block(look_at_x, look_at_y, look_at_z)) {
            int chunk_x = look_at_x / 16;
            int chunk_z = look_at_z / 16;
            int in_x = look_at_x % 16;
            int in_z = look_at_z % 16;

            World::chunk *c = World::world_map[chunk_x][chunk_z];

            c->block[in_x][look_at_y][in_z] = 0;

            World::update_light(look_at_x, look_at_y, look_at_z);
        }
    }

    std::optional<glm::vec3> ray_trace_blocks(glm::vec3 start, glm::vec3 end) {
        if (!glm::isnan(start).r && !glm::isnan(end).r) {
            int intX1 = round(start.x);
            int intY1 = round(start.y);
            int intZ1 = round(start.z);

            int intX2 = round(end.x);
            int intY2 = round(end.y);
            int intZ2 = round(end.z);

            // if player's head is in block
            if (World::have_block(intX1, intY1, intZ1)) {
                return glm::vec3(intX1, intY1, intZ1);
            }

            // the max detection number
            int count = 100;

            while (count-- >= 0) {

                if (intX1 < 0 || intY1 < 0 || intZ1 < 0 || intY1 >= 64) {
                    return std::nullopt;
                }

                if (glm::isnan(start).r || glm::isnan(end).r) {
                    return std::nullopt;
                }

                if (intX1 == intX2 && intY1 == intY2 && intZ1 == intZ2) {
                    return std::nullopt;
                }

                bool Xchanged = true;
                bool Ychanged = true;
                bool Zchanged = true;

                double newX;
                double newY;
                double newZ;

                if (intX2 > intX1) {
                    newX = (double) intX1 + 1.0f;
                } else if (intX2 < intX1) {
                    newX = (double) intX1 - 1.0f;
                } else {
                    Xchanged = false;
                }

                if (intY2 > intY1) {
                    newY = (double) intY1 + 1.0f;
                } else if (intY2 < intY1) {
                    newY = (double) intY1 - 1.0f;
                } else {
                    Ychanged = false;
                }

                if (intZ2 > intZ1) {
                    newZ = (double) intZ1 + 1.0f;
                } else if (intZ2 < intZ1) {
                    newZ = (double) intZ1 - 1.0f;
                } else {
                    Zchanged = false;
                }

                double Xt = 999.0;
                double Yt = 999.0;
                double Zt = 999.0;
                double dX = end.x - start.x;
                double dY = end.y - start.y;
                double dZ = end.z - start.z;


//                if(Xchanged) {
//                    Xt = (newX - start.x) / dX;
//                }
//                if(Ychanged) {
//                    Yt = (newY - start.y) / dY;
//                }
//                if(Zchanged) {
//                    Zt = (newZ - start.z) / dZ;
//                }
                if (Xchanged) {
                    Xt = (newX - start.x) / dX;
                }
                if (Ychanged) {
                    Yt = (newY - start.y) / dY;
                }
                if (Zchanged) {
                    Zt = (newZ - start.z) / dZ;
                }

                unsigned char direction;

                if (Xt < Yt && Xt < Zt) {
                    if (intX2 > intX1) {
                        direction = 4;
                    } else {
                        direction = 5;
                    }

                    start.x = newX;
                    start.y += dY * Xt;
                    start.z += dZ * Xt;
                } else if (Yt < Zt) {
                    if (intY2 > intY1) {
                        direction = 0;
                    } else {
                        direction = 1;
                    }

                    start.x += dX * Yt;
                    start.y = newY;
                    start.z += dZ * Yt;
                } else {
                    if (intZ2 > intZ1) {
                        direction = 2;
                    } else {
                        direction = 3;
                    }

                    start.x += dX * Zt;
                    start.y += dY * Zt;
                    start.z = newZ;
                }

                intX1 = round(start.x);
                intY1 = round(start.y);
                intZ1 = round(start.z);

                if (World::have_block(intX1, intY1, intZ1)) {
                    return glm::vec3(intX1, intY1, intZ1);
                }
            }

            return std::nullopt;
        }

        return std::nullopt;
    }

    void ray_trace_face(void) {
        double yaw = Physics::g_yaw / 180.0 * M_PI;
        double pitch = Physics::g_pitch / 180 * M_PI;

//        glm::vec3 start = glm::vec3(g_x, g_y, g_z);
//        glm::vec3 end = glm::vec3(
//            g_x + len * cos(pitch) * sin(yaw),
//            g_y + len * sin(pitch),
//            g_z - len * cos(pitch) * cos(yaw)
//        );

        bool face[6] = {};

        // which face is looked
        if (g_x < look_at_x) face[2] = true;
        else if (g_x > look_at_x) face[3] = true;
        if (g_y > look_at_y) face[0] = true;
        else if (g_y < look_at_y) face[1] = true;
        if (g_z > look_at_z) face[4] = true;
        else if (g_z < look_at_z) face[5] = true;


        // basic cube module
        //         -1,1,-1                1,1,-1
        //                 4 ───────────┐5
        //                  /          /│
        //        -1,1,1  0┌──────────┐1│ 1,1,1
        //                 │          │ │
        //                 │          │ /
        //       -1,-1,-1  │ 7        │/ 6 1,-1,-1
        //                 └──────────┘
        //                3        2
        //        -1,-1,1             1,-1,1
        double nx = look_at_x - 0.5;
        double px = look_at_x + 0.5;
        double ny = look_at_y - 0.5;
        double py = look_at_y + 0.5;
        double nz = look_at_z - 0.5;
        double pz = look_at_z + 0.5;

        /*
         * x = x_0 + ta;
         * y = y_0 + tb;
         * z = z_0 + tc;
         *
         * t = (x - x_0) / a;
         * t = (y - y_0) / b;
         * t = (z - z_0) / c;
         */

        double t;

        double a = cos(pitch) * sin(yaw);
        double b = sin(pitch);
        double c = -cos(pitch) * cos(yaw);

        look_at_face = -1;

        if (face[0]) {
            t = (py - g_y) / b;
            double x1 = g_x + t * a;
            double z1 = g_z + t * c;

            if (nx <= x1 && x1 <= px && nz <= z1 && z1 <= pz) {
                look_at_face = 0;
                return;
            }
        }
        if (face[1]) {
            t = (ny - g_y) / b;
            double x1 = g_x + t * a;
            double z1 = g_z + t * c;

            if (nx <= x1 && x1 <= px && nz <= z1 && z1 <= pz) {
                look_at_face = 1;
                return;
            }
        }
        if (face[2]) {
            t = (nx - g_x) / a;
            double y1 = g_y + t * b;
            double z1 = g_z + t * c;

            if (ny <= y1 && y1 <= py && nz <= z1 && z1 <= pz) {
                look_at_face = 2;
                return;
            }
        }
        if (face[3]) {
            t = (px - g_x) / a;
            double y1 = g_y + t * b;
            double z1 = g_z + t * c;

            if (ny <= y1 && y1 <= py && nz <= z1 && z1 <= pz) {
                look_at_face = 3;
                return;
            }
        }
        if (face[4]) {
            t = (pz - g_z) / c;
            double y1 = g_y + t * b;
            double x1 = g_x + t * a;

            if (ny <= y1 && y1 <= py && nx <= x1 && x1 <= px) {
                look_at_face = 4;
                return;
            }
        }
        if (face[5]) {
            t = (nz - g_z) / c;
            double y1 = g_y + t * b;
            double x1 = g_x + t * a;

            if (ny <= y1 && y1 <= py && nx <= x1 && x1 <= px) {
                look_at_face = 5;
//                return;
            }
        }
    }
}