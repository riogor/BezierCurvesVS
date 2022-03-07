#pragma once

#include "pch.h"

void error(int error, const char* description);

void reshape(GLFWwindow* window, int w, int h);
void display();

void mouse(GLFWwindow* window, int button, int action, int mode);
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mode);