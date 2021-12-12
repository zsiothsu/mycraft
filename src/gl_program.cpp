#include "gl_program.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>

NormalProgram normal_program;
ShadowProgram shadow_program;

char *normal_vertex_shader;
char *normal_fragment_shader;
char *shadow_vertex_shader;
char *shadow_fragment_shader;

std::string pwd;

void program_init(void) {
    char cwd[100];
    getcwd(cwd, sizeof(cwd));
    pwd = std::string(cwd) + "/";
    std::clog << "[INFO] program is running at " << pwd << std::endl;

    create_normal_program();
    create_shadow_program();
    std::clog << "[SHADER] Shader program loaded" << std::endl;
}

void create_normal_program(void) {
    std::ifstream t;
    int length;

    t.open((pwd + "./shader/normal_vertext_shader.glsl").c_str());
    t.seekg(0, std::ios::end);
    length = t.tellg();
    t.seekg(0, std::ios::beg);
    normal_vertex_shader = new char[length + 2];
    t.read(normal_vertex_shader, length);
    normal_vertex_shader[length] = '\n';
    normal_vertex_shader[length + 1] = 0;
    t.close();

    t.open((pwd + "./shader/normal_fragment_shader.glsl").c_str());
    t.seekg(0, std::ios::end);
    length = t.tellg();
    t.seekg(0, std::ios::beg);
    normal_fragment_shader = new char[length + 2];
    t.read(normal_fragment_shader, length);
    normal_fragment_shader[length] = '\n';
    normal_fragment_shader[length + 1] = 0;
    t.close();

    GLint success;
    GLchar infoLog[512];

    normal_program.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(normal_program.vertex_shader, 1, &normal_vertex_shader, NULL);
    glCompileShader(normal_program.vertex_shader);

    glGetShaderiv(normal_program.vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(normal_program.vertex_shader, 512, NULL, infoLog);
        std::cerr << "[COMPILER] " << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }

    normal_program.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(normal_program.fragment_shader, 1, &normal_fragment_shader, NULL);
    glCompileShader(normal_program.fragment_shader);

    glGetShaderiv(normal_program.fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(normal_program.fragment_shader, 512, NULL, infoLog);
        std::cerr << "[COMPILER] " << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }

    normal_program.program = glCreateProgram();
    glAttachShader(normal_program.program, normal_program.vertex_shader);
    glAttachShader(normal_program.program, normal_program.fragment_shader);
    glLinkProgram(normal_program.program);

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
    std::ifstream t;
    int length;

    std::string content;
    t.open((pwd + "./shader/shadow_vertext_shader.glsl").c_str());
    t.seekg(0, std::ios::end);
    length = t.tellg();
    t.seekg(0, std::ios::beg);
    shadow_vertex_shader = new char[length + 2];
    memset(normal_vertex_shader, 0, sizeof(shadow_vertex_shader));
    t.read(shadow_vertex_shader, length);
    shadow_vertex_shader[length] = '\n';
    shadow_vertex_shader[length + 1] = 0;
    t.close();


    t.open((pwd + "./shader/shadow_fragment_shader.glsl").c_str());
    t.seekg(0, std::ios::end);
    length = t.tellg();
    t.seekg(0, std::ios::beg);
    shadow_fragment_shader = new char[length + 2];
    memset(normal_vertex_shader, 0, sizeof(shadow_fragment_shader));
    t.read(shadow_fragment_shader, length);
    shadow_fragment_shader[length] = '\n';
    shadow_fragment_shader[length + 1] = 0;
    t.close();

    GLint success;
    GLchar infoLog[512];

    shadow_program.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shadow_program.vertex_shader, 1, &shadow_vertex_shader, NULL);
    glCompileShader(shadow_program.vertex_shader);

    glGetShaderiv(shadow_program.vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shadow_program.vertex_shader, 512, NULL, infoLog);
        std::cerr << "[COMPILER] " << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }

    shadow_program.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shadow_program.fragment_shader, 1, &shadow_fragment_shader, NULL);
    glCompileShader(shadow_program.fragment_shader);

    glGetShaderiv(shadow_program.fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shadow_program.fragment_shader, 512, NULL, infoLog);
        std::cerr << "[COMPILER] " << infoLog << std::endl;
        exit(EXIT_FAILURE);
    }

    shadow_program.program = glCreateProgram();
    glAttachShader(shadow_program.program, shadow_program.vertex_shader);
    glAttachShader(shadow_program.program, shadow_program.fragment_shader);
    glLinkProgram(shadow_program.program);

    {
        shadow_program.a_Position = glGetAttribLocation(shadow_program.program, "a_Position");
        shadow_program.M = glGetUniformLocation(shadow_program.program, "M");
        shadow_program.V = glGetUniformLocation(shadow_program.program, "V");
        shadow_program.P = glGetUniformLocation(shadow_program.program, "P");
    }
}