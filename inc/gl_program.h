#ifndef FINAL_CPP_GL_PROGRAM_H
#define FINAL_CPP_GL_PROGRAM_H

#include "glad/gl.h"
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "linmath.h"

typedef struct {
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLint a_Position;
    GLint a_TexCoord;
    GLint a_Normal;
    GLint M;
    GLint V;
    GLint P;
    GLint M_FromLight;
    GLint V_FromLight;
    GLint P_FromLight;
    GLint u_Sampler;
    GLint u_color_Sampler;
    GLint u_ShadowMap;
    GLint u_alpha_mode;
    GLint u_LightColor;
    GLint u_LightDirection;
    GLint u_need_shadow;
} NormalProgram;

typedef struct {
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;
    GLint a_Position;
    GLint M;
    GLint V;
    GLint P;
} ShadowProgram;

extern NormalProgram normal_program;
extern ShadowProgram shadow_program;

void program_init(void);
void create_normal_program(void);
void create_shadow_program(void);

#endif //FINAL_CPP_GL_PROGRAM_H
