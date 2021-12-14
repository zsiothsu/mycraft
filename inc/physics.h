//
// Created by chipen on 2021/12/12.
//

#ifndef FINAL_CPP_PHYSICS_H
#define FINAL_CPP_PHYSICS_H

namespace Physics {

    // camera
    extern float g_x;
    extern float g_y;
    extern float g_z;
    extern float g_yaw;
    extern float g_pitch;

    extern float lookX;
    extern float lookY;
    extern float lookZ;

    void init();
    void update();
    void update_acceleration_speed();
    void update_position();
    void set_boundary(float dx, float dy, float dz);
    bool collision_XZ(float next_x, float next_z, int block_x, int block_y, int block_z);
    bool collision_Y(float next_y, int block_x, int block_y, int block_z);
}

#endif //FINAL_CPP_PHYSICS_H
