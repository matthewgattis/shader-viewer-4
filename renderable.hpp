#pragma once

#include <memory>

#include <glm/glm.hpp>

class Material;
class Geometry;

class Renderable
{
public:
    virtual void render(const glm::mat4& view_matrix) = 0;
};

