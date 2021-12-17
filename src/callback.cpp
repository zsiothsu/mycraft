#include "callback.h"
#include <iostream>
#include "physics.h"

bool key_state[400];
bool button_state[8];
bool window_focus = true;
extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;


void key_handler(void) {
    if(!window_focus) return;

    Physics::update();
}

void mouse_handler(GLFWwindow *window) {
    if(button_state[GLFW_MOUSE_BUTTON_LEFT]) {
        button_state[GLFW_MOUSE_BUTTON_LEFT] = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        window_focus = true;
        Physics::look_at();
        Physics::break_block();
    }
    if(button_state[GLFW_MOUSE_BUTTON_RIGHT]) {
        button_state[GLFW_MOUSE_BUTTON_RIGHT] = false;
        Physics::look_at();
        Physics::place_block();
    }
}

void error_callback(int error, const char *description) {
    std::cerr << "Error: " << description;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_W && action == GLFW_PRESS) key_state[GLFW_KEY_W] = true;
    else if (key == GLFW_KEY_W && action == GLFW_RELEASE) key_state[GLFW_KEY_W] = false;
    if (key == GLFW_KEY_S && action == GLFW_PRESS) key_state[GLFW_KEY_S] = true;
    else if (key == GLFW_KEY_S && action == GLFW_RELEASE) key_state[GLFW_KEY_S] = false;
    if (key == GLFW_KEY_A && action == GLFW_PRESS) key_state[GLFW_KEY_A] = true;
    else if (key == GLFW_KEY_A && action == GLFW_RELEASE) key_state[GLFW_KEY_A] = false;
    if (key == GLFW_KEY_D && action == GLFW_PRESS) key_state[GLFW_KEY_D] = true;
    else if (key == GLFW_KEY_D && action == GLFW_RELEASE) key_state[GLFW_KEY_D] = false;
    if (key == GLFW_KEY_R && action == GLFW_PRESS) key_state[GLFW_KEY_R] = true;
    else if (key == GLFW_KEY_R && action == GLFW_RELEASE) key_state[GLFW_KEY_R] = false;
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) key_state[GLFW_KEY_LEFT_SHIFT] = true;
    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) key_state[GLFW_KEY_LEFT_SHIFT] = false;
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) key_state[GLFW_KEY_SPACE] = true;
    else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) key_state[GLFW_KEY_SPACE] = false;

    if (key == GLFW_KEY_ESCAPE) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        window_focus = false;
    }
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) button_state[GLFW_MOUSE_BUTTON_LEFT] = true;
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) button_state[GLFW_MOUSE_BUTTON_LEFT] = false;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) button_state[GLFW_MOUSE_BUTTON_RIGHT] = true;
    else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) button_state[GLFW_MOUSE_BUTTON_RIGHT] = false;
}

void cursor_position_callback(GLFWwindow *window, double x, double y) {
    static bool first_move = true;
    static GLfloat lastX, lastY;
    static GLfloat mouse_step = 0.2;

    if(!window_focus) return;

    if (first_move) {
        lastX = x, lastY = y;
        first_move = false;
    }

    GLfloat dx = x - lastX;
    GLfloat dy = y - lastY;
    lastX = x;
    lastY = y;

    if (dx > 0) {
        Physics::g_yaw = Physics::g_yaw + mouse_step * dx;
        Physics::g_yaw = Physics::g_yaw > 360.0 ? Physics::g_yaw - 360 : Physics::g_yaw;
    } else if (dx < 0) {
        Physics::g_yaw = Physics::g_yaw - mouse_step * -dx;
        Physics::g_yaw = Physics::g_yaw < 0 ? Physics::g_yaw + 360 : Physics::g_yaw;
    }
    if (dy < 0) {
        Physics::g_pitch = Physics::g_pitch + mouse_step * -dy;
        Physics::g_pitch = Physics::g_pitch > 89 ? 89 : Physics::g_pitch;
    } else if (dy > 0) {
        Physics::g_pitch = Physics::g_pitch - mouse_step * dy;
        Physics::g_pitch = Physics::g_pitch < -89 ? -89 : Physics::g_pitch;
    }
}

void window_size_callback(GLFWwindow* window, int width, int height) {
    WINDOW_HEIGHT = height;
    WINDOW_WIDTH = width;
}

void window_focus_callback(GLFWwindow* window, int focused) {
    window_focus = focused;
}