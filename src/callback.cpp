#include "callback.h"
#include <iostream>

bool key_state[400];

void error_callback(int error, const char *description) {
    fprintf(stderr, "Error: %s\n", description);
}

void key_handler(void) {
    static float speed = 0.4;
    float _yaw = g_yaw / 180 * M_PI;

    if (key_state[GLFW_KEY_W]) {
        g_x = g_x + sin(_yaw) * speed;
        g_z = g_z - cos(_yaw) * speed;
    }
    if (key_state[GLFW_KEY_S]) {
        g_x = g_x - sin(_yaw) * speed;
        g_z = g_z + cos(_yaw) * speed;
    }
    if (key_state[GLFW_KEY_A]) {
        g_x = g_x - sin(_yaw + M_PI / 2) * speed;
        g_z = g_z + cos(_yaw + M_PI / 2) * speed;
    }
    if (key_state[GLFW_KEY_D]) {
        g_x = g_x + sin(_yaw + M_PI / 2) * speed;
        g_z = g_z - cos(_yaw + M_PI / 2) * speed;
    }
    if (key_state[GLFW_KEY_LEFT_SHIFT]) {
        g_y -= 0.1 * speed;
    }
    if (key_state[GLFW_KEY_SPACE]) {
        g_y += 0.1 * speed;
    }
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
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS) key_state[GLFW_KEY_LEFT_SHIFT] = true;
    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) key_state[GLFW_KEY_LEFT_SHIFT] = false;
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) key_state[GLFW_KEY_SPACE] = true;
    else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE) key_state[GLFW_KEY_SPACE] = false;
    if (key == GLFW_KEY_ESCAPE) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
}

void cursor_position_callback(GLFWwindow *window, double x, double y) {
    static bool first_move = true;
    static GLfloat lastX, lastY;
    static GLfloat mouse_step = 0.3;

    if (first_move) {
        lastX = x, lastY = y;
        first_move = false;
    }

    GLfloat dx = x - lastX;
    GLfloat dy = y - lastY;
    lastX = x;
    lastY = y;

    if (dx > 0) {
        g_yaw = g_yaw + mouse_step * dx;
        g_yaw = g_yaw > 360.0 ? g_yaw - 360 : g_yaw;
    } else if (dx < 0) {
        g_yaw = g_yaw - mouse_step * -dx;
        g_yaw = g_yaw < 0 ? g_yaw + 360 : g_yaw;
    }
    if (dy < 0) {
        g_pitch = g_pitch + mouse_step * -dy;
        g_pitch = g_pitch > 90 ? 90 : g_pitch;
    } else if (dy > 0) {
        g_pitch = g_pitch - mouse_step * dy;
        g_pitch = g_pitch < -90 ? -90 : g_pitch;
    }
}