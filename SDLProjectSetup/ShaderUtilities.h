#pragma once

#include <string>
#include <glad/glad.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Definitions.h"

class ShaderUtilities {
public:
    static GLuint loadShader(const char* vertexPath, const char* fragmentPath);
private:
    static GLuint compileShader(GLenum type, const std::string& source);
};
