#ifndef FINAL_CPP_MAIN_H
#define FINAL_CPP_MAIN_H


#include <cstdint>
#include <cstring>
#include "gl_program.h"
#include "world.h"
#include "callback.h"
#include "physics.h"

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
        "./texture/grass_side.png",             //3
        "./texture/grass_top_alpha.png",        //4
        "./texture/log_oak.png",                //5
        "./texture/log_oak_top.png",            //6
        "./texture/grass_side.png",             //7
        "./texture/grass_top_alpha.png",        //8
        "./texture/log_oak.png",                //9
        "./texture/log_oak_top.png",            //10
        "./texture/stone.png",                  //11
        "./texture/wool_colored_blue.png",      //12
        "./texture/wool_colored_brown.png",     //13
        "./texture/wool_colored_cyan.png",      //14
        "./texture/wool_colored_gray.png",      //15
        "./texture/wool_colored_green.png",     //16
        "./texture/wool_colored_light_blue.png",//17
        "./texture/wool_colored_lime.png",      //18
        "./texture/wool_colored_magenta.png",   //19
        "./texture/wool_colored_orange.png",    //20
        "./texture/wool_colored_pink.png",      //21
        "./texture/wool_colored_purple.png",    //22
        "./texture/wool_colored_red.png",       //23
        "./texture/wool_colored_silver.png",    //24
        "./texture/wool_colored_white.png",     //25
        "./texture/wool_colored_yellow.png",    //26
        "./texture/planks_oak.png",             //27
        "./texture/iron_block.png",             //28
        "./texture/gold_block.png",             //29
};
const static int texture_count = 30;

const static uint8_t img_grass[6] = {8, 2, 7, 7, 7, 7};
const static uint8_t img_log[6] = {10, 10, 9, 9, 9, 9};
const static uint8_t img_sky[6] = {5, 2, 6, 4, 4, 4};
const static uint8_t img_brick[6] = {1, 1, 1, 1, 1, 1};
const static uint8_t img_stone[6] = {11, 11, 11, 11, 11, 11};
const static uint8_t img_dirt[6] = {2, 2, 2, 2, 2, 2};
const static uint8_t img_wool_colored_blue[6] = {12, 12, 12, 12, 12, 12};
const static uint8_t img_wool_colored_brown[6] = {13, 13, 13, 13, 13, 13};
const static uint8_t img_wool_colored_cyan[6] = {14, 14, 14, 14, 14, 14};
const static uint8_t img_wool_colored_gray[6] = {15, 15, 15, 15, 15, 15};
const static uint8_t img_wool_colored_green[6] = {16, 16, 16, 16, 16, 16};
const static uint8_t img_wool_colored_light_blue[6] = {17, 17, 17, 17, 17, 17};
const static uint8_t img_wool_colored_lime[6] = {18, 18, 18, 18, 18, 18};
const static uint8_t img_wool_colored_magenta[6] = {19, 19, 19, 19, 19, 19};
const static uint8_t img_wool_colored_orange[6] = {20, 20, 20, 20, 20, 20};
const static uint8_t img_wool_colored_pink[6] = {21, 21, 21, 21, 21, 21};
const static uint8_t img_wool_colored_purple[6] = {22, 22, 22, 22, 22, 22};
const static uint8_t img_wool_colored_red[6] = {23, 23, 23, 23, 23, 23};
const static uint8_t img_wool_colored_silver[6] = {24, 24, 24, 24, 24, 24};
const static uint8_t img_wool_colored_white[6] = {25, 25, 25, 25, 25, 25};
const static uint8_t img_wool_colored_yellow[6] = {26, 26, 26, 26, 26, 26};
const static uint8_t img_planks_oak[6] = {27, 27, 27, 27, 27, 27};
const static uint8_t img_iron_block[6] = {28, 28, 28, 28, 28, 28};
const static uint8_t img_gold_block[6] = {29, 29, 29, 29, 29, 29};

const static uint8_t outter_mode = 0;
const static uint8_t inner_mode = 1;

const static uint8_t color_normal_mode = 0;
const static uint8_t color_blend_mode = 1;

enum blocks_id {
    id_grass = 1,
    id_stone = 2,
    id_brick,
    id_dirt,
    id_log_oak,
    id_planks_oak,
    id_wool_colored_blue,
    id_wool_colored_brown,
    id_wool_colored_cyan,
    id_wool_colored_gray,
    id_wool_colored_green,
    id_wool_colored_light_blue,
    id_wool_colored_lime,
    id_wool_colored_magenta,
    id_wool_colored_orange,
    id_wool_colored_pink,
    id_wool_colored_purple,
    id_wool_colored_red,
    id_wool_colored_silver,
    id_wool_colored_white,
    id_wool_colored_yellow,
    id_gold_block,
    id_iron_block,
};

const static GLuint OFFSCREEN_WIDTH = 2048, OFFSCREEN_HEIGHT = 2048;


#endif //FINAL_CPP_MAIN_H

