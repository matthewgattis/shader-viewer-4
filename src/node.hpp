#pragma once

#include <memory>

#include <glm/glm.hpp>

class Node
{
protected:
    Node() = default;

public:
    virtual std::shared_ptr<Node> getParent() const = 0;

    virtual glm::mat4 getTransform() const = 0;
};

