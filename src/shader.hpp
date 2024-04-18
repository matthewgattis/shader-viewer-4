#pragma once

#include <iostream>

#include <GL/glew.h>

class Shader
{
public:
    Shader(std::istream& is, GLenum type);
    Shader() = delete;
    ~Shader();

    GLuint get() const { return shader_; }

private:
    void log();

private:
    GLuint shader_;
    GLenum type_;
};

