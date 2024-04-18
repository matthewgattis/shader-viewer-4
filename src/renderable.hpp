#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "node.hpp"

class Geometry;
class Material;

class Renderable : public Node
{
protected:
    Renderable() = default;

public:
    virtual void draw(const glm::mat4& model_view_projection_matrix) = 0;
};

