#include <cstdlib>
#include <iostream>
#include <cmath>
#include <map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "stb/stb_image.h"
#include "main.h"


// light
float light_atX = -8;
float light_atY = 8;
float light_atZ = 32;

float light_cX = 16;
float light_cY = -12;
float light_cZ = 8;

GLuint fbo;
int __last_texture = -1;

extern std::string pwd;
GLFWwindow *window;

int WINDOW_WIDTH = 1280;
int WINDOW_HEIGHT = 720;


std::map<uint8_t, uint8_t *> texture_map = {
        {id_grass,                   (uint8_t *) img_grass},
        {id_stone,                   (uint8_t *) img_stone},
        {id_brick,                   (uint8_t *) img_brick},
        {id_dirt,                    (uint8_t *) img_dirt},
        {id_log_oak,                 (uint8_t *) img_log},
        {id_planks_oak,              (uint8_t *) img_planks_oak},
        {id_iron_block,              (uint8_t *) img_iron_block},
        {id_gold_block,              (uint8_t *) img_gold_block},
        {id_wool_colored_blue,       (uint8_t *) img_wool_colored_blue},
        {id_wool_colored_brown,      (uint8_t *) img_wool_colored_brown},
        {id_wool_colored_cyan,       (uint8_t *) img_wool_colored_cyan},
        {id_wool_colored_gray,       (uint8_t *) img_wool_colored_gray},
        {id_wool_colored_green,      (uint8_t *) img_wool_colored_green},
        {id_wool_colored_light_blue, (uint8_t *) img_wool_colored_light_blue},
        {id_wool_colored_lime,       (uint8_t *) img_wool_colored_lime},
        {id_wool_colored_magenta,    (uint8_t *) img_wool_colored_magenta},
        {id_wool_colored_orange,     (uint8_t *) img_wool_colored_orange},
        {id_wool_colored_pink,       (uint8_t *) img_wool_colored_pink},
        {id_wool_colored_purple,     (uint8_t *) img_wool_colored_purple},
        {id_wool_colored_red,        (uint8_t *) img_wool_colored_red},
        {id_wool_colored_silver,     (uint8_t *) img_wool_colored_silver},
        {id_wool_colored_white,      (uint8_t *) img_wool_colored_white},
        {id_wool_colored_yellow,     (uint8_t *) img_wool_colored_yellow},
};

std::map<uint8_t, uint8_t> blocks_icon{
        {id_grass,                   7},
        {id_stone,                   11},
        {id_brick,                   1},
        {id_dirt,                    2},
        {id_log_oak,                 9},
        {id_planks_oak,              27},
        {id_iron_block,              28},
        {id_gold_block,              29},
        {id_wool_colored_blue,       12},
        {id_wool_colored_brown,      13},
        {id_wool_colored_cyan,       14},
        {id_wool_colored_gray,       15},
        {id_wool_colored_green,      16},
        {id_wool_colored_light_blue, 17},
        {id_wool_colored_lime,       18},
        {id_wool_colored_magenta,    19},
        {id_wool_colored_orange,     20},
        {id_wool_colored_pink,       21},
        {id_wool_colored_purple,     22},
        {id_wool_colored_red,        23},
        {id_wool_colored_silver,     24},
        {id_wool_colored_white,      25},
        {id_wool_colored_yellow,     26}
};

void glfw_init(void);

void gl_config(void);

void gl_fbo_init(void);

void gl_texture_init(void);

void draw_map(bool isDepth);

void draw_cube(
        float atX, float atY, float atZ,
        float size,
        uint8_t *imgs,
        uint8_t mode,
        uint8_t isDepth,
        bool alpha_mode = false,
        bool need_shadow = false
);

void draw_cross();

void draw_handed_block();

void set_MVP_matrix();

void set_sky_light(GLfloat R, GLfloat G, GLfloat B, bool isDepth);

void get_chunk_index(float x, float z, int *_x, int *_z);

GLuint init_vertex_buffers(GLfloat vertices[48]);

void load_texture(int image, int alpha_mode);

void turn_to_inner_mode(GLfloat vertices[48]);

void begin(void);

int main() {
    glfw_init();
    program_init();
    gl_config();
    gl_fbo_init();
    gl_texture_init();

    begin();

    // should not be there
    return 0;
}

void glfw_init(void) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "my craft: 请认准官方盗版", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetWindowFocusCallback(window, window_focus_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, mouse_scroll_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    std::clog << "[GLFW] Window loaded" << std::endl;
}

void gl_config(void) {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
}

void gl_fbo_init(void) {
    glGenTextures(1, &fbo);
    glActiveTexture(texture_index[31]);
    glBindTexture(GL_TEXTURE_2D, fbo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void gl_texture_init(void) {
    GLint max;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max);
    std::clog << "[INFO] OpenGL max texture size is " << max << std::endl;

    for (int i = 0; i < texture_count; i++) {
        uint32_t texture;
        int width, height, nrChannels;
        uint8_t *data = stbi_load((pwd + img_list[i]).c_str(), &width, &height, &nrChannels, 0);
        std::clog << "[INFO] loading " << img_list[i] << std::endl;
        if (data) {
            glGenTextures(1, &texture);
            glActiveTexture(texture_index[i]);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            stbi_image_free(data);
        } else {
            std::cerr << "[TEXTURE] Failed to read texture images" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    std::clog << "[INFO] texture loaded" << std::endl;
}

void draw_map(bool isDepth) {
    int current_chunk_x, current_chunk_z;
    get_chunk_index(Physics::g_x, Physics::g_z, &current_chunk_x, &current_chunk_z);

    for (int dx = -1; dx <= 1; dx++) {
        for (int dz = -1; dz <= 1; dz++) {
            int chunk_base_x = current_chunk_x + dx;
            int chunk_base_z = current_chunk_z + dz;

            if (chunk_base_x < 0 || chunk_base_x > WORLD_SIZE
                || chunk_base_z < 0 || chunk_base_z > WORLD_SIZE) {
                continue;
            }

            World::chunk *c = World::world_map[chunk_base_x][chunk_base_z];

            for (int in_x = 0; in_x < 16; in_x++) {
                for (int in_z = 0; in_z < 16; in_z++) {
                    for (int in_y = 0; in_y < 64; in_y++) {
                        int block_id = c->block[in_x][in_y][in_z];

                        if (block_id) {
                            uint8_t *imgs;

                            imgs = texture_map[block_id];

                            float dx0 = chunk_base_x * 16 + in_x - Physics::g_x;
                            float dy0 = in_y - Physics::g_y;
                            float dz0 = chunk_base_z * 16 + in_z - Physics::g_z;
                            float block_to_eye_len = sqrt(dx0 * dx0 + dy0 * dy0 + dz0 * dz0);
                            dx0 = dx0 / block_to_eye_len;
                            dy0 = dy0 / block_to_eye_len;
                            dz0 = dz0 / block_to_eye_len;

                            float dx1 = Physics::lookX - Physics::g_x;
                            float dy1 = Physics::lookY - Physics::g_y;
                            float dz1 = Physics::lookZ - Physics::g_z;

                            if (dx0 * dx1 + dy0 * dy1 + dz0 * dz1 > 0) {
                                if (block_id == id_grass) {
                                    draw_cube(chunk_base_x * 16 + in_x, in_y, chunk_base_z * 16 + in_z, 1, imgs,
                                              outter_mode,
                                              isDepth, color_blend_mode, true);
                                } else {
                                    draw_cube(chunk_base_x * 16 + in_x, in_y, chunk_base_z * 16 + in_z, 1, imgs,
                                              outter_mode,
                                              isDepth, color_normal_mode, true);
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

void draw_cube(
        float atX, float atY, float atZ,
        float size,
        uint8_t *imgs,
        uint8_t mode,
        uint8_t isDepth,
        bool alpha_mode,
        bool need_shadow
) {
    size = size / 2;

    if (!isDepth) {
        if (need_shadow) {
            glUniform1i(normal_program.u_need_shadow, 1);
        } else {
            glUniform1i(normal_program.u_need_shadow, 0);
        }
    }

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
    float cube[8][3] = {
            {-size + atX, size + atY,  size + atZ},
            {size + atX,  size + atY,  size + atZ},
            {size + atX,  -size + atY, size + atZ},
            {-size + atX, -size + atY, size + atZ},
            {-size + atX, size + atY,  -size + atZ},
            {size + atX,  size + atY,  -size + atZ},
            {size + atX,  -size + atY, -size + atZ},
            {-size + atX, -size + atY, -size + atZ}
    };

    // texture index
    // the number of each face equals the order of texture image
    //             ┌────┐
    //             │ 1  │
    //             │top │
    //        ┌────┼────┼────┬───┐
    //        │ 3  │ 5  │ 4  │ 6 │
    //        │lft │frt │rit │ bk│
    //        └────┼────┼────┴───┘
    //             │ 2  │
    //             │btm │
    //             └────┘
    if (!World::have_block(atX, atY + 1, atZ)) {
        GLfloat arrtop[48] = {
                cube[0][0], cube[0][1], cube[0][2], 0, 0, 0, 1, 0,
                cube[1][0], cube[1][1], cube[1][2], 1, 0, 0, 1, 0,
                cube[5][0], cube[5][1], cube[5][2], 1, 1, 0, 1, 0,
                cube[0][0], cube[0][1], cube[0][2], 0, 0, 0, 1, 0,
                cube[5][0], cube[5][1], cube[5][2], 1, 1, 0, 1, 0,
                cube[4][0], cube[4][1], cube[4][2], 0, 1, 0, 1, 0,
        };
        if (mode) turn_to_inner_mode(arrtop);
        GLuint buffer = init_vertex_buffers(arrtop);
        if (!isDepth && imgs[0] != __last_texture) {
            load_texture(imgs[0], alpha_mode);
            __last_texture = imgs[0];
        }
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDeleteBuffers(1, &buffer);
        glDeleteVertexArrays(1, &buffer);
    }

    if (!World::have_block(atX, atY - 1, atZ)) {
        GLfloat arrbtm[48] = {
                cube[3][0], cube[3][1], cube[3][2], 0, 0, 0, -1, 0,
                cube[7][0], cube[7][1], cube[7][2], 0, 1, 0, -1, 0,
                cube[6][0], cube[6][1], cube[6][2], 1, 1, 0, -1, 0,
                cube[3][0], cube[3][1], cube[3][2], 0, 0, 0, -1, 0,
                cube[6][0], cube[6][1], cube[6][2], 1, 1, 0, -1, 0,
                cube[2][0], cube[2][1], cube[2][2], 1, 0, 0, -1, 0,
        };
        if (mode) turn_to_inner_mode(arrbtm);
        GLuint buffer = init_vertex_buffers(arrbtm);
        if (!isDepth && imgs[1] != __last_texture) {
            load_texture(imgs[1], alpha_mode);
            __last_texture = imgs[1];
        }
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDeleteBuffers(1, &buffer);
        glDeleteVertexArrays(1, &buffer);
    }

    if (!World::have_block(atX - 1, atY, atZ)) {
        GLfloat arrlft[48] = {
                cube[7][0], cube[7][1], cube[7][2], 0, 0, -1, 0, 0,
                cube[3][0], cube[3][1], cube[3][2], 1, 0, -1, 0, 0,
                cube[0][0], cube[0][1], cube[0][2], 1, 1, -1, 0, 0,
                cube[7][0], cube[7][1], cube[7][2], 0, 0, -1, 0, 0,
                cube[0][0], cube[0][1], cube[0][2], 1, 1, -1, 0, 0,
                cube[4][0], cube[4][1], cube[4][2], 0, 1, -1, 0, 0,
        };
        if (mode) turn_to_inner_mode(arrlft);
        GLuint buffer = init_vertex_buffers(arrlft);
        if (!isDepth && imgs[2] != __last_texture) {
            load_texture(imgs[2], false);
            __last_texture = imgs[2];
        }
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDeleteBuffers(1, &buffer);
        glDeleteVertexArrays(1, &buffer);
    }

    if (!World::have_block(atX + 1, atY, atZ)) {
        GLfloat arrrit[48] = {
                cube[2][0], cube[2][1], cube[2][2], 0, 0, 1, 0, 0,
                cube[6][0], cube[6][1], cube[6][2], 1, 0, 1, 0, 0,
                cube[5][0], cube[5][1], cube[5][2], 1, 1, 1, 0, 0,
                cube[2][0], cube[2][1], cube[2][2], 0, 0, 1, 0, 0,
                cube[5][0], cube[5][1], cube[5][2], 1, 1, 1, 0, 0,
                cube[1][0], cube[1][1], cube[1][2], 0, 1, 1, 0, 0,
        };
        if (mode) turn_to_inner_mode(arrrit);
        GLuint buffer = init_vertex_buffers(arrrit);
        if (!isDepth && imgs[3] != __last_texture) {
            load_texture(imgs[3], false);
            __last_texture = imgs[3];
        }
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDeleteBuffers(1, &buffer);
        glDeleteVertexArrays(1, &buffer);
    }

    if (!World::have_block(atX, atY, atZ + 1)) {
        GLfloat arrfrt[48] = {
                cube[3][0], cube[3][1], cube[3][2], 0, 0, 0, 0, 1,
                cube[2][0], cube[2][1], cube[2][2], 1, 0, 0, 0, 1,
                cube[1][0], cube[1][1], cube[1][2], 1, 1, 0, 0, 1,
                cube[3][0], cube[3][1], cube[3][2], 0, 0, 0, 0, 1,
                cube[1][0], cube[1][1], cube[1][2], 1, 1, 0, 0, 1,
                cube[0][0], cube[0][1], cube[0][2], 0, 1, 0, 0, 1,
        };
        if (mode) turn_to_inner_mode(arrfrt);
        GLuint buffer = init_vertex_buffers(arrfrt);
        if (!isDepth && imgs[4] != __last_texture) {
            load_texture(imgs[4], false);
            __last_texture = imgs[4];
        }
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDeleteBuffers(1, &buffer);
        glDeleteVertexArrays(1, &buffer);
    }

    if (!World::have_block(atX, atY, atZ - 1)) {
        GLfloat arrbk[48] = {
                cube[6][0], cube[6][1], cube[6][2], 0, 0, 0, 0, -1,
                cube[4][0], cube[4][1], cube[4][2], 1, 1, 0, 0, -1,
                cube[5][0], cube[5][1], cube[5][2], 0, 1, 0, 0, -1,
                cube[6][0], cube[6][1], cube[6][2], 0, 0, 0, 0, -1,
                cube[7][0], cube[7][1], cube[7][2], 1, 0, 0, 0, -1,
                cube[4][0], cube[4][1], cube[4][2], 1, 1, 0, 0, -1,
        };
        if (mode) turn_to_inner_mode(arrbk);
        GLuint buffer = init_vertex_buffers(arrbk);
        if (!isDepth && imgs[5] != __last_texture) {
            load_texture(imgs[5], false);
            __last_texture = imgs[5];
        }
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDeleteBuffers(1, &buffer);
        glDeleteVertexArrays(1, &buffer);
    }
}

void draw_cross() {
    GLfloat vertices[] = {-0.01, 0, 0,
                          0.01, 0, 0,
                          0, (GLfloat) (0.01 * WINDOW_WIDTH / WINDOW_HEIGHT), 0,
                          0, (GLfloat) (-0.01 * WINDOW_WIDTH / WINDOW_HEIGHT), 0};

    GLuint vertexBuffer;
    glGenVertexArrays(1, &vertexBuffer);
    glGenBuffers(1, &vertexBuffer);
    if (!vertexBuffer) {
        std::clog << "Failed to create buffer object" << std::endl;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, vertices, GL_STATIC_DRAW);

    uint64_t bpe = sizeof(GLfloat);

    glVertexAttribPointer(cross_program.a_Position, 3, GL_FLOAT, false, 3 * bpe, 0);
    glEnableVertexAttribArray(cross_program.a_Position);

    glUniform1i(cross_program.fbo, 31);

    glLineWidth(3);
    glDrawArrays(GL_LINES, 0, 4);

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexBuffer);
}

void draw_handed_block() {
    GLfloat vertices[] = {
            -0.9, -0.9, 0, 0, 0,
            -0.8, -0.9, 0, 1, 0,
            -0.9, GLfloat(-0.9 + ((float) WINDOW_WIDTH / WINDOW_HEIGHT) * 0.1), 0, 0, 1,
            -0.9, GLfloat(-0.9 + ((float) WINDOW_WIDTH / WINDOW_HEIGHT) * 0.1), 0, 0, 1,
            -0.8, -0.9, 0, 1, 0,
            -0.8, GLfloat(-0.9 + ((float) WINDOW_WIDTH / WINDOW_HEIGHT) * 0.1), 0, 1, 1
    };

    GLuint vertexBuffer;
    glGenVertexArrays(1, &vertexBuffer);
    glGenBuffers(1, &vertexBuffer);
    if (!vertexBuffer) {
        std::clog << "Failed to create buffer object" << std::endl;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 30, vertices, GL_STATIC_DRAW);

    uint64_t bpe = sizeof(GLfloat);

    glVertexAttribPointer(single_face_program.a_Position, 3, GL_FLOAT, false, 5 * bpe, 0);
    glEnableVertexAttribArray(single_face_program.a_Position);

    glVertexAttribPointer(single_face_program.a_TexCoord, 2, GL_FLOAT, false, 5 * bpe, (GLvoid *) (3 * bpe));
    glEnableVertexAttribArray(single_face_program.a_TexCoord);

    glUniform1i(single_face_program.u_Sampler, blocks_icon[Physics::holding_block]);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexBuffer);
}

void set_MVP_matrix() {
    GLfloat yaw = Physics::g_yaw / 180.0 * M_PI;
    GLfloat pitch = Physics::g_pitch / 180 * M_PI;
    double fov = Physics::g_fov / 180.0 * M_PI;

    Physics::lookX = Physics::g_x + cos(pitch) * sin(yaw);
    Physics::lookY = Physics::g_y + sin(pitch);
    Physics::lookZ = Physics::g_z - cos(pitch) * cos(yaw);

    glm::mat4 gMatrixM(1.0f);
    glUniformMatrix4fv(normal_program.M, 1, false, glm::value_ptr(gMatrixM));

    glm::mat4 gMatrixV;
    gMatrixV = glm::lookAt(
            glm::vec3(Physics::g_x, Physics::g_y, Physics::g_z),
            glm::vec3(Physics::lookX, Physics::lookY, Physics::lookZ),
            glm::vec3(0.0, 1.0, 0.0)
    );
    glUniformMatrix4fv(normal_program.V, 1, false, glm::value_ptr(gMatrixV));

    glm::mat4 gMatrixP;
    gMatrixP = glm::perspectiveFov(fov, (double) WINDOW_WIDTH, (double) WINDOW_HEIGHT, 0.01, 100.0);
    glUniformMatrix4fv(normal_program.P, 1, false, glm::value_ptr(gMatrixP));
}


void set_sky_light(GLfloat R, GLfloat G, GLfloat B, bool isDepth) {
    GLfloat yaw = 45.0 / 180.0 * M_PI;
    GLfloat pitch = -40.0 / 180 * M_PI;

//    light_atX = Physics::g_x - 100 * cos(pitch) * sin(yaw);
//    light_atY = Physics::g_y - 100 * sin(pitch);
//    light_atZ = Physics::g_z + 100 * cos(pitch) * cos(yaw);
//    light_atX = Physics::g_x;
//    light_atY = Physics::g_y + 100;
//    light_atZ = Physics::g_z;
    light_atX = -100;
    light_atY = 100;
    light_atZ = -100;

    GLfloat light_vX = light_atX - Physics::g_x;
    GLfloat light_vY = light_atY - Physics::g_y;
    GLfloat light_vZ = light_atZ - Physics::g_z;

    glm::mat4 lightM(1.0f);

    glm::mat4 lightV;
    lightV = glm::lookAt(
            glm::vec3(light_atX, light_atY, light_atZ),
            glm::vec3(Physics::g_x,Physics::g_y, Physics::g_z),
            glm::vec3(0.0, 1.0, 0.0)
    );

    glm::mat4 lightP;
//    lightP = glm::perspectiveFov(50.0/180.0*M_PI, (double) OFFSCREEN_WIDTH, (double) OFFSCREEN_HEIGHT, 0.1, 200.0);
    lightP = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.1f, 200.0f);

    if (isDepth) {
        glUniformMatrix4fv(shadow_program.M, 1, false, glm::value_ptr(lightM));
        glUniformMatrix4fv(shadow_program.V, 1, false, glm::value_ptr(lightV));
        glUniformMatrix4fv(shadow_program.P, 1, false, glm::value_ptr(lightP));
    } else {
        glUniform3f(normal_program.u_LightDirection, light_vX, light_vY, light_vZ);
        glUniform3f(normal_program.u_LightColor, R, G, B);
        glUniformMatrix4fv(normal_program.M_FromLight, 1, false, glm::value_ptr(lightM));
        glUniformMatrix4fv(normal_program.V_FromLight, 1, false, glm::value_ptr(lightV));
        glUniformMatrix4fv(normal_program.P_FromLight, 1, false, glm::value_ptr(lightP));
    }
}

void get_chunk_index(float x, float z, int *_x, int *_z) {
    *_x = (int) x / 16;
    *_z = (int) z / 16;
}

GLuint init_vertex_buffers(GLfloat vertices[48]) {
    GLuint vertexBuffer;
    glGenVertexArrays(1, &vertexBuffer);
    glGenBuffers(1, &vertexBuffer);
    if (!vertexBuffer) {
        std::clog << "Failed to create buffer object" << std::endl;
        return vertexBuffer;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 48, vertices, GL_STATIC_DRAW);

    uint64_t bpe = sizeof(GLfloat);

    glVertexAttribPointer(normal_program.a_Position, 3, GL_FLOAT, false, 8 * bpe, 0);
    glEnableVertexAttribArray(normal_program.a_Position);

    glVertexAttribPointer(normal_program.a_TexCoord, 2, GL_FLOAT, false, 8 * bpe, (GLvoid *) (3 * bpe));
    glEnableVertexAttribArray(normal_program.a_TexCoord);

    glVertexAttribPointer(normal_program.a_Normal, 3, GL_FLOAT, false, 8 * bpe, (GLvoid *) (5 * bpe));
    glEnableVertexAttribArray(normal_program.a_Normal);

    return vertexBuffer;
}

void load_texture(int image, int alpha_mode) {
    glUniform1i(normal_program.u_Sampler, image);
    glUniform1i(normal_program.u_alpha_mode, alpha_mode);
}

void turn_to_inner_mode(GLfloat vertices[48]) {
    int col = 8;
    int index_normal = 5;

    for (int i = col; i < 2 * col; i++) {
        GLfloat t = vertices[i];
        vertices[i] = vertices[i + col];
        vertices[i + col] = t;
    }
    for (int i = 4 * col; i < 5 * col; i++) {
        GLfloat t = vertices[i];
        vertices[i] = vertices[i + col];
        vertices[i + col] = t;
    }

    for (int i = 0; i < 6; i++) {
        vertices[index_normal + i * col] = 0;
        vertices[index_normal + i * col + 1] = 1;
        vertices[index_normal + i * col + 2] = 0;
    }
}

void begin(void) {
    World::load_world();
    Physics::init();


    while (!glfwWindowShouldClose(window)) {
        glBindBuffer(GL_FRAMEBUFFER, fbo);
        glViewport(0, 0, OFFSCREEN_HEIGHT, OFFSCREEN_WIDTH);
        glClearColor(0, 0, 0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shadow_program.program);
        set_sky_light(1, 0.99, 0.95, true);
        draw_map(true);

        glBindBuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
        glClearColor(172.0 / 255.0, 224.0 / 255.0, 252.0 / 255.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(normal_program.program);
        set_MVP_matrix();
        set_sky_light(1, 0.99, 0.95, false);
        draw_map(false);

        glUseProgram(cross_program.program);
        draw_cross();

        glUseProgram(single_face_program.program);
        draw_handed_block();

        glfwPollEvents();
        key_handler();
        mouse_handler(window);
        glfwSwapBuffers(window);
    }

    World::save_world();
    std::clog << "[WORLD] world saved" << std::endl;

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

