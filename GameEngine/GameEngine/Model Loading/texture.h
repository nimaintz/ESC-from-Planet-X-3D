#pragma once
#include <glew.h>
#include <glfw3.h>
#include <vector>
#include <string>

GLuint loadBMP(const char * imagepath);
GLuint loadCubemap(const std::vector<std::string>& faces);
