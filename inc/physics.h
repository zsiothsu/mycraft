#ifndef FINAL_CPP_PHYSICS_H
#define FINAL_CPP_PHYSICS_H

#include <optional>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

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

    void look_at(void);

    std::optional<glm::vec3> ray_trace_blocks(glm::vec3 start, glm::vec3 end);
}

#endif //FINAL_CPP_PHYSICS_H
