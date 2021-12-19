#ifndef FINAL_CPP_WORLD_H
#define FINAL_CPP_WORLD_H

#include <cstdint>
#include <cstring>
#include "main.h"

#define WORLD_SIZE 16

namespace World {

    struct chunk {
        int x;
        int z;
        uint8_t block[WORLD_SIZE][64][WORLD_SIZE];

        chunk() {
            x = 0;
            z = 0;
            memset(block, 0, sizeof(block));
        }
    };

    extern chunk *world_map[16][16];

    void generate_map(void);

    void build_house(void);

    int have_block(int x, int y, int z);

    void save_world(void);

    void load_world(void);
}

#endif //FINAL_CPP_WORLD_H
