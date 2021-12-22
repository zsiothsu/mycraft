#ifndef FINAL_CPP_WORLD_H
#define FINAL_CPP_WORLD_H

#include <cstdint>
#include <cstring>
#include "main.h"

#define WORLD_SIZE 16
#define WORLD_HEIGHT 64

namespace World {

    struct chunk {
        int x;
        int z;
        uint8_t block[WORLD_SIZE][WORLD_HEIGHT][WORLD_SIZE];
        uint8_t light[WORLD_SIZE][WORLD_HEIGHT][WORLD_SIZE];
        chunk() {
            x = 0;
            z = 0;
            memset(block, 0, sizeof(block));
            memset(light, 0, sizeof(light));
        }
    };

    extern chunk *world_map[16][16];

    void generate_map(void);

    int get_light(int x, int y, int z);

    void update_light(int x, int y, int z);

    int have_block(int x, int y, int z);

    void save_world(void);

    void load_world(void);
}

#endif //FINAL_CPP_WORLD_H
