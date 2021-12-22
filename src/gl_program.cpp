#include "gl_program.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <unistd.h>


NormalProgram normal_program;
ShadowProgram shadow_program;
CrossProgram cross_program;
SingleFaceProgram single_face_program;

char *vertex_shader_buf;
char *fragment_shader_buf;

std::string pwd;

void program_init(void) {
    char cwd[100];
    getcwd(cwd, sizeof(cwd));
    pwd = std::string(cwd) + "/";
    std::clog << "[INFO] program is running at " << pwd << std::endl;

    create_normal_program();
    create_shadow_program();
    create_cross_program();
    create_single_face_program();
    std::clog << "[SHADER] Shader program loaded" << std::endl;
}

void create_normal_program(void) {
    read_compile_program(
            (pwd + "./shader/normal_vertext_shader.glsl").c_str(),
            (pwd + "./shader/normal_fragment_shader.glsl").c_str(),
            &normal_program
    );

    {
        normal_program.a_Position = glGetAttribLocation(normal_program.program, "a_Position");
        normal_program.a_TexCoord = glGetAttribLocation(normal_program.program, "a_TexCoord");
        normal_program.a_Normal = glGetAttribLocation(normal_program.program, "a_Normal");
        normal_program.M = glGetUniformLocation(normal_program.program, "M");
        normal_program.V = glGetUniformLocation(normal_program.program, "V");
        normal_program.P = glGetUniformLocation(normal_program.program, "P");
        normal_program.u_LightColor = glGetUniformLocation(normal_program.program, "u_LightColor");
        normal_program.u_LightDirection = glGetUniformLocation(normal_program.program, "u_LightDirection");
        normal_program.M_FromLight = glGetUniformLocation(normal_program.program, "M_FromLight");
        normal_program.V_FromLight = glGetUniformLocation(normal_program.program, "V_FromLight");
        normal_program.P_FromLight = glGetUniformLocation(normal_program.program, "P_FromLight");
        normal_program.u_Sampler = glGetUniformLocation(normal_program.program, "u_Sampler");
        normal_program.u_color_Sampler = glGetUniformLocation(normal_program.program, "u_color_Sampler");
        normal_program.u_alpha_mode = glGetUniformLocation(normal_program.program, "u_alpha_mode");
        normal_program.u_need_shadow = glGetUniformLocation(normal_program.program, "u_need_shadow");
        normal_program.u_ShadowMap = glGetUniformLocation(normal_program.program, "u_ShadowMap");
    }
}

void create_shadow_program(void) {
    read_compile_program(
            (pwd + "./shader/shadow_vertext_shader.glsl").c_str(),
            (pwd + "./shader/shadow_fragment_shader.glsl").c_str(),
            &shadow_program
    );

    {
        shadow_program.a_Position = glGetAttribLocation(shadow_program.program, "a_Position");
        shadow_program.M = glGetUniformLocation(shadow_program.program, "M");
        shadow_program.V = glGetUniformLocation(shadow_program.program, "V");
        shadow_program.P = glGetUniformLocation(shadow_program.program, "P");
    }
}

void create_cross_program(void) {
    read_compile_program(
            (pwd + "./shader/cross_vertext_shader.glsl").c_str(),
            (pwd + "./shader/cross_fragment_shader.glsl").c_str(),
            &cross_program
    );

    {
        cross_program.a_Position = glGetAttribLocation(cross_program.program, "a_Position");
        cross_program.fbo = glGetUniformLocation(cross_program.program, "fbo");
    }
}

void create_single_face_program(void) {
    read_compile_program(
            (pwd + "./shader/single_block_face_vertext_shader.glsl").c_str(),
            (pwd + "./shader/single_block_face_fragment_shader.glsl").c_str(),
            &single_face_program
    );

    {
        single_face_program.a_Position = glGetAttribLocation(single_face_program.program, "a_Position");
        single_face_program.a_TexCoord = glGetAttribLocation(single_face_program.program, "a_TexCoord");
        single_face_program.u_Sampler = glGetUniformLocation(single_face_program.program, "u_Sampler");
    }
}

void read_compile_program(const char *vshader, const char *fshader, Program *p) {
    std::ifstream t;
    int length;

    std::string content;
    t.open(vshader);
    t.seekg(0, std::ios::end);
    length = t.tellg();
    t.seekg(0, std::ios::beg);
    vertex_shader_buf = new char[length + 2];
    memset(vertex_shader_buf, 0, sizeof(vertex_shader_buf));
    t.read(vertex_shader_buf, length);
    vertex_shader_buf[length] = '\n';
    vertex_shader_buf[length + 1] = 0;
    t.close();


    t.open(fshader);
    t.seekg(0, std::ios::end);
    length = t.tellg();
    t.seekg(0, std::ios::beg);
    fragment_shader_buf = new char[length + 2];
    memset(fragment_shader_buf, 0, sizeof(fragment_shader_buf));
    t.read(fragment_shader_buf, length);
    fragment_shader_buf[length] = '\n';
    fragment_shader_buf[length + 1] = 0;
    t.close();

    GLint success;
    GLchar infoLog[512];

    p->vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(p->vertex_shader, 1, &vertex_shader_buf, NULL);
    glCompileShader(p->vertex_shader);

    glGetShaderiv(p->vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(p->vertex_shader, 512, NULL, infoLog);
        std::cerr << "[COMPILER] " << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }

    p->fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(p->fragment_shader, 1, &fragment_shader_buf, NULL);
    glCompileShader(p->fragment_shader);

    glGetShaderiv(p->fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(p->fragment_shader, 512, NULL, infoLog);
        std::cerr << "[COMPILER] " << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }

    p->program = glCreateProgram();
    glAttachShader(p->program, p->vertex_shader);
    glAttachShader(p->program, p->fragment_shader);
    glLinkProgram(p->program);

    delete (vertex_shader_buf);
    delete (fragment_shader_buf);
}