#ifndef FINAL_CPP_CALLBACK_H
#define FINAL_CPP_CALLBACK_H

#include "main.h"

/**
 * callback
 */
void error_callback(int error, const char *description);

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);

void cursor_position_callback(GLFWwindow *window, double x, double y);

void window_size_callback(GLFWwindow *window, int width, int height);

void window_focus_callback(GLFWwindow *window, int focused);

/**
 * handler
 */
void key_handler(void);

void mouse_handler(GLFWwindow *window);

#endif //FINAL_CPP_CALLBACK_H
