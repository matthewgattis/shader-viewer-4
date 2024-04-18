#pragma once

#include <glm/glm.hpp>

#include <SDL2/SDL.h>

#include "node.hpp"

class Camera : public Node
{
protected:
    Camera() = default;

public:
    virtual glm::mat4 getProjectionMatrix() const = 0;
};

