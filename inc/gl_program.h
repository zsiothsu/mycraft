#ifndef FINAL_CPP_GL_PROGRAM_H
#define FINAL_CPP_GL_PROGRAM_H

#include "glad/gl.h"

#define GLFW_INCLUDE_NONE

#include "GLFW/glfw3.h"
#include "linmath.h"

struct Program {
    GLuint program;
    GLuint vertex_shader;
    GLuint fragment_shader;
};

struct NormalProgram : public Program{
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
};

struct ShadowProgram : public Program{
    GLint a_Position;
    GLint M;
    GLint V;
    GLint P;
};

struct CrossProgram : public Program{
    GLint a_Position;
    GLint fbo;
};

extern NormalProgram normal_program;
extern ShadowProgram shadow_program;
extern CrossProgram cross_program;

void program_init(void);

void read_compile_program(const char* vshader, const char* fshader, Program *p);

void create_normal_program(void);

void create_shadow_program(void);

void create_cross_program(void);


#endif //FINAL_CPP_GL_PROGRAM_H
