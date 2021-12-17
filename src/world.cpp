#include "world.h"

namespace World {
    chunk *world_map[16][16];

    void generate_map(void) {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                world_map[i][j] = nullptr;
            }
        }

        for (int _x = 0; _x <= 16; _x++) {
            for (int _z = 0; _z <= 16; _z++) {
                chunk *new_chunk = new chunk;
                world_map[_x][_z] = new_chunk;
                new_chunk->x = _x;
                new_chunk->z = _z;

                for (int x = 0; x < 16; x++) {
                    for (int z = 0; z < 16; z++) {
                        new_chunk->block[x][0][z] = id_grass;
                    }
                }
            }
        }
    }

    void build_house(void) {
        chunk *c = world_map[1][1];

        int house_index[34][3] = {
                {2,  1, 5},
                {4,  1, 2},
                {8,  1, 1},
                {12, 1, 2},
                {14, 1, 5},
                {2,  2, 5},
                {4,  2, 2},
                {8,  2, 1},
                {12, 2, 2},
                {14, 2, 5},
                {2,  3, 5},
                {4,  3, 2},
                {8,  3, 1},
                {12, 3, 2},
                {14, 3, 5},
                {2,  3, 4},
                {2,  3, 3},
                {3,  3, 3},
                {4,  3, 2},
                {5,  3, 2},
                {6,  3, 2},
                {6,  3, 1},
                {7,  3, 1},
                {8,  3, 1},
                {9,  3, 1},
                {10, 3, 1},
                {10, 3, 2},
                {11, 3, 2},
                {12, 3, 2},
                {12, 3, 3},
                {13, 3, 3},
                {14, 3, 3},
                {14, 3, 4},
                {14, 3, 5},
        };

        for (auto i: house_index) {
            c->block[i[0]][i[1]][i[2]] = id_brick;
        }

        int log_index[4][3] = {
                {8,  1, 6},
                {8,  1, 10},
                {6,  1, 8},
                {10, 1, 8},
        };

        for (auto i: log_index) {
            c->block[i[0]][i[1]][i[2]] = id_log_oak;
        }

        c->block[8][0][8] = id_stone;
    }

    int have_block(int x, int y, int z) {
        if (x >= 0 && z >= 0 && y >= 0 && y < 64) {
            int chunk_x = x / 16;
            int chunk_z = z / 16;
            int in_x = x % 16;
            int in_z = z % 16;

            chunk *c = world_map[chunk_x][chunk_z];
            if (c == nullptr) return 0;

            return c->block[in_x][y][in_z];
        } else {
            return 0;
        }
    }
}