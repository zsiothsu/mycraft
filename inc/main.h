#ifndef FINAL_CPP_MAIN_H
#define FINAL_CPP_MAIN_H


#include <cstdint>
#include <cstring>
#include "gl_program.h"
#include "world.h"
#include "callback.h"

const static uint32_t texture_index[] = {
        GL_TEXTURE0, GL_TEXTURE1, GL_TEXTURE2, GL_TEXTURE3, GL_TEXTURE4,
        GL_TEXTURE5, GL_TEXTURE6, GL_TEXTURE7, GL_TEXTURE8, GL_TEXTURE9,
        GL_TEXTURE10, GL_TEXTURE11, GL_TEXTURE12, GL_TEXTURE13, GL_TEXTURE14,
        GL_TEXTURE15, GL_TEXTURE16, GL_TEXTURE17, GL_TEXTURE18, GL_TEXTURE19,
        GL_TEXTURE20, GL_TEXTURE21, GL_TEXTURE22, GL_TEXTURE23, GL_TEXTURE24,
        GL_TEXTURE25, GL_TEXTURE26, GL_TEXTURE27, GL_TEXTURE28, GL_TEXTURE29,
        GL_TEXTURE30, GL_TEXTURE31,
};

//const static char *img_list[] = {
//        "./texture/colormap.png",               //0
//        "./texture/brick.png",                  //1
//        "./texture/dirt.png",                   //2
//        "./texture/flat_grass.png",             //2
//        "./texture/flat_side.png",              //4
//        "./texture/flat_sky.png",               //5
//        "./texture/flat_village.png",           //6
//        "./texture/grass_side.png",             //7
//        "./texture/grass_top_alpha.png",        //8
//        "./texture/log_oak.png",                //9
//        "./texture/log_oak_top.png",            //10
//        "./texture/stone.png",                  //11
//};

const static char *img_list[] = {
        "./texture/colormap.png",               //0
        "./texture/brick.png",                  //1
        "./texture/dirt.png",                   //2
        "./texture/grass_side.png",             //7
        "./texture/grass_top_alpha.png",        //8
        "./texture/log_oak.png",                //9
        "./texture/log_oak_top.png",            //10
        "./texture/grass_side.png",             //7
        "./texture/grass_top_alpha.png",        //8
        "./texture/log_oak.png",                //9
        "./texture/log_oak_top.png",            //10
        "./texture/stone.png",                  //11
};
const static int texture_count = 12;

const static uint8_t img_grass[6] = {8, 2, 7, 7, 7, 7};
const static uint8_t img_log[6] = {10, 10, 9, 9, 9, 9};
const static uint8_t img_sky[6] = {5, 2, 6, 4, 4, 4};
const static uint8_t img_brick[6] = {1, 1, 1, 1, 1, 1};
const static uint8_t img_stone[6] = {11, 11, 11, 11, 11, 11};

const static uint8_t outter_mode = 0;
const static uint8_t inner_mode = 1;

const static uint8_t color_normal_mode = 0;
const static uint8_t color_blend_mode = 1;

const static uint8_t id_grass = 1;
const static uint8_t id_log_oak = 2;
const static uint8_t id_stone = 3;
const static uint8_t id_brick = 4;

const static GLuint OFFSCREEN_WIDTH = 2048, OFFSCREEN_HEIGHT = 2048;

extern float g_x;
extern float g_y;
extern float g_z;
extern float g_yaw;
extern float g_pitch;


#endif //FINAL_CPP_MAIN_H

