#include "world.h"
#include "perlin_noise.h"

#include <string>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <queue>
#include <set>
#include <map>
#include <tuple>

namespace World {
    using std::tuple;
    using std::queue;
    using std::set;
    using std::map;
    using std::get;

    chunk *world_map[16][16];

    void generate_map(void) {
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                if (world_map[i][j] != nullptr) {
                    delete world_map[i][j];
                }
                world_map[i][j] = nullptr;
            }
        }

        Perlin perlin;

        for (int _x = 0; _x < 16; _x++) {
            for (int _z = 0; _z < 16; _z++) {
                chunk *new_chunk = new chunk;
                world_map[_x][_z] = new_chunk;
                new_chunk->x = _x;
                new_chunk->z = _z;

                for (int x = 0; x < 16; x++) {
                    for (int z = 0; z < 16; z++) {
                        int world_x = _x * 16 + x;
                        int world_z = _z * 16 + z;

                        int inth = 0;
                        for (int i = -2; i < 2; i++) {
                            for (int j = -2; j < 2; j++) {
                                double height = perlin.PerlinNoise((float) (world_x + i) / 10,
                                                                   (float) (world_z + j) / 10);
                                inth += (int) (fabs(height + 1) * 10);
                            }
                        }
                        inth /= 25;

                        for (int h = 0; h < inth; h++) {
                            new_chunk->block[x][h][z] = id_stone;
                        }
                        if (inth > 0) {
                            new_chunk->block[x][inth - 1][z] = id_dirt;
                            new_chunk->block[x][inth][z] = id_grass;

                        } else {
                            new_chunk->block[x][inth][z] = id_grass;
                        }

                        // for each air block's light which have no blocks above is 7
                        for (int h = inth + 1; h < WORLD_HEIGHT; h++) {
                            new_chunk->light[x][h][z] = 7;
                        }
                        for (int h = 0; h < inth + 1; h++) {
                            new_chunk->light[x][h][z] = 0;
                        }
                    }
                }
            }
        }
    }

    void set_light(int x, int y, int z, int val) {
        if (x >= 0 && z >= 0 && y >= 0 && y < 64) {
            int chunk_x = x / 16;
            int chunk_z = z / 16;
            int in_x = x % 16;
            int in_z = z % 16;

            chunk *c = world_map[chunk_x][chunk_z];
            if (c == nullptr) return;

            c->light[in_x][y][in_z] = val;
        }
    }

    int get_light(int x, int y, int z) {
        if (x >= 0 && z >= 0 && y >= 0 && y < 64) {
            int chunk_x = x / 16;
            int chunk_z = z / 16;
            int in_x = x % 16;
            int in_z = z % 16;

            chunk *c = world_map[chunk_x][chunk_z];
            if (c == nullptr) return 7;

            return c->light[in_x][y][in_z];
        } else {
            return 7;
        }
    }

    void update_light(int x, int y, int z) {
        static const int search_list[6][3] = {
                {0,  1,  0},
                {0,  -1, 0},
                {-1, 0,  0},
                {1,  0,  0},
                {0,  0,  1},
                {0,  0,  -1},
        };
        if (have_block(x, y, z) && (have_block(x, y, z) != id_sea_lantern)) {
            set_light(x, y, z, 0);
        } else {
            set_light(x, y, z, 7);
        }

        typedef tuple<int, int, int> Index;
        set<Index> close_list;
        queue<Index> q;
        q.push(Index(x, y, z));

        map<Index, int> changed;

        while (!q.empty()) {
            Index index = q.front();
            q.pop();

            int nx = get<0>(index);
            int ny = get<1>(index);
            int nz = get<2>(index);


            bool closed = false;
            for (auto i: close_list) {
                if (get<0>(i) == nx && get<1>(i) == ny && get<2>(i) == nz) {
                    closed = true;
                }
            }
            if (closed) continue;

            close_list.insert(index);

            int next_block_id = have_block(nx, ny, nz);

            if (((next_block_id != 0) || (next_block_id != id_sea_lantern)) && (nx != x || ny != y || nz != z)) continue;

            if ((abs(nx - x) + abs(ny - y) + abs(nz - z)) > 7) {
                continue;
            }

            // sky light
            int flag_have_blocks_above = false;
            if (have_block(nx, ny + 1, nz) || (get_light(nx, ny + 1, nz) < 7)) {
                flag_have_blocks_above = true;
            }


            if (!flag_have_blocks_above) {
                set_light(nx, ny, nz, 7);
            }

            {
                int max_light = 0;
                for (auto face: search_list) {
                    int to_x = nx + face[0];
                    int to_y = ny + face[1];
                    int to_z = nz + face[2];
                    int to_block_id = have_block(to_x, to_y, to_z);
                    if (!to_block_id || (to_block_id == id_sea_lantern)) {
                        bool flag_closed = false;
                        for (auto i: close_list) {
                            if (get<0>(i) == to_x && get<1>(i) == to_y && get<2>(i) == to_z) {
                                flag_closed = true;
                            }
                        }
                        if (!flag_closed) {
                            q.push(Index(to_x, to_y, to_z));
                        }

                        int l = get_light(to_x, to_y, to_z);
                        max_light = l > max_light ? l : max_light;
                    }
                }


                // block or sea_lantern
                int block_id = have_block(nx, ny, nz);
                if ((block_id != 0) && (block_id != id_sea_lantern)) {
                    set_light(nx, ny, nz, 0);
                } else if (block_id == id_sea_lantern) {
                    set_light(nx, ny, nz, 15);
                }

                // select lightest
                else if (flag_have_blocks_above) {
                    Index idx = Index(nx, ny, nz);
                    if(changed.find(idx) == changed.end()) {
                        set_light(nx, ny, nz, max_light - 1 >= 0 ? max_light - 1 : 0);
                        changed[idx] = max_light - 1;
                    } else {
                        int light = changed[idx];
                        if(light < max_light - 1) {
                            set_light(nx, ny, nz, max_light - 1 >= 0 ? max_light - 1 : 0);
                            changed[idx] = max_light - 1;
                        }
                    }
                }
            }
        }
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

    void save_world(void) {
        std::string pwd;
        char cwd[100];
        getcwd(cwd, sizeof(cwd));
        pwd = std::string(cwd) + "/";

        std::ofstream outFile(pwd + "saves/region.mcr", std::ios::out | std::ios::binary);

        int chunk_size = sizeof(chunk);

        for (int _x = 0; _x < 16; _x++) {
            for (int _z = 0; _z < 16; _z++) {
                chunk *c = world_map[_x][_z];
                if (c != nullptr) {
                    outFile.write((char *) c, chunk_size);
                }
            }
        }

        outFile.close();
    }

    void load_world(void) {
        std::string pwd;
        char cwd[100];
        getcwd(cwd, sizeof(cwd));
        pwd = std::string(cwd) + "/";

        std::ifstream inFile(pwd + "saves/region.mcr", std::ios::in | std::ios::binary);

        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < 16; j++) {
                world_map[i][j] = nullptr;
            }
        }

        if (inFile) {
            int chunk_size = sizeof(chunk);
            char *chunk_data = new char[chunk_size];

            while (inFile.read(chunk_data, chunk_size)) {
                if (inFile.gcount() != chunk_size) {
                    std::cerr << "[WORLD] save file is corrupted" << std::endl;
                    goto world_generation;
                }

                int _x = *(int *) chunk_data;
                int _z = *(int *) (chunk_data + sizeof(int));

                world_map[_x][_z] = new chunk;
                world_map[_x][_z]->x = _x;
                world_map[_x][_z]->z = _z;
                memcpy(
                        world_map[_x][_z]->block,
                        chunk_data + 2 * sizeof(int),
                        sizeof(world_map[_x][_z]->block)
                );
                memcpy(
                        world_map[_x][_z]->light,
                        chunk_data + 2 * sizeof(int) + sizeof(world_map[_x][_z]->block),
                        sizeof(world_map[_x][_z]->light)
                );
            }

            std::clog << "[WORLD] read world successfully" << std::endl;
            goto world_load_end;
        }

        world_generation:
        generate_map();
//        build_house();
        world_load_end:
        inFile.close();
        return;
    }
}