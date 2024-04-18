#pragma once

#include "node.hpp"

class Transform : public Node
{
public:
    Transform() :
        parent_(nullptr),
        transform_(1.0f)
    {
    }

    Transform(
        const std::shared_ptr<Node>& parent,
        const glm::mat4& transform = glm::mat4(1.0f)) :
        parent_(parent),
        transform_(transform)
    {
    }

    Transform(const glm::mat4& transform) :
        parent_(nullptr),
        transform_(transform)
    {
    }

public:
    std::shared_ptr<Node> getParent() const override { return parent_; }
    void setParent(const std::shared_ptr<Node>& parent) { parent_ = parent; }

    glm::mat4 getTransform() const override { return transform_; }
    void setTransform(const glm::mat4& transform) { transform_ = transform; }

private:
    std::shared_ptr<Node> parent_;
    glm::mat4 transform_;
};

