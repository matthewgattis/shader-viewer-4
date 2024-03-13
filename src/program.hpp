#pragma once

#include <vector>
#include <memory>

#include <GL/glew.h>

class Shader;

class Program
{
public:
    Program(const std::vector<std::shared_ptr<Shader>>& shaders);
    Program() = delete;
    ~Program();

    GLuint get() const { return program_; }

private:
    GLuint program_;
    std::vector<std::shared_ptr<Shader>> shaders_;
};

