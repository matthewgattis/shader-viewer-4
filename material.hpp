#pragma once

#include <memory>
#include <optional>

#include <GL/glew.h>
#include <glm/glm.hpp>

class Program;

class Material
{
public:
    Material() = default;

    virtual void useMaterial() = 0;

    virtual GLint getPositionAttrib() = 0;

    virtual void setViewMatrixUniform(const glm::mat4& view_matrix) = 0;
};

