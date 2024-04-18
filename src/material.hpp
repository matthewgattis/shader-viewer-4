#pragma once

#include <glm/glm.hpp>
#include <GL/glew.h>

class Program;

class Material
{
protected:
    Material() = default;

public:
    virtual void use() = 0;
    virtual void setModelViewProjection(const glm::mat4& model_view_projection) = 0;
};

