#ifndef FINAL_CPP_PHYSICS_H
#define FINAL_CPP_PHYSICS_H

#include <optional>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Physics {

    // camera
    extern double g_x;
    extern double g_y;
    extern double g_z;
    extern double g_yaw;
    extern double g_pitch;

    extern double lookX;
    extern double lookY;
    extern double lookZ;

    extern int holding_block;

    extern float g_fov;

    void init();

    void update();

    void update_acceleration_speed();

    void update_position();

    void set_boundary(double dx, double dy, double dz);

    bool collision_XZ(double next_x, double next_z, int block_x, int block_y, int block_z);

    bool collision_Y(double next_y, int block_x, int block_y, int block_z);

    void look_at(void);

    void place_block();

    void break_block();

    std::optional<glm::vec3> ray_trace_blocks(glm::vec3 start, glm::vec3 end);

    void ray_trace_face(void);
}

#endif //FINAL_CPP_PHYSICS_H
