#pragma once

#include <vector>
#include <memory>
#include <optional>
#include <string>

#include <GL/glew.h>

class Shader;

class Program
{
public:
    Program(const std::vector<std::shared_ptr<Shader>>& shaders);
    Program() = delete;
    ~Program();

    GLuint get() const { return program_; }

    std::optional<GLint> getAttributeLocation(const std::string& name);
    std::optional<GLint> getUniformLocation(const std::string& name);

private:
    GLuint program_;
    std::vector<std::shared_ptr<Shader>> shaders_;
};

