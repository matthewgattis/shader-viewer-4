#pragma once

#include <memory>

#include "renderable.hpp"
#include "axismaterial.hpp"
#include "axisgeometry.hpp"

class Node;

class Axis : public Renderable
{
public:
    Axis() = delete;
    Axis(
        const std::shared_ptr<Node>& parent,
        const std::shared_ptr<AxisMaterial>& axis_material,
        const std::shared_ptr<AxisGeometry>& axis_geometry);

    void draw(const glm::mat4& model_view_projection_matrix) override;

private:
    std::shared_ptr<AxisMaterial> axis_material_;
    std::shared_ptr<AxisGeometry> axis_geometry_;

public:
    virtual std::shared_ptr<Node> getParent() const override { return parent_; }
    virtual glm::mat4 getTransform() const override { return transform_; }

private:
    std::shared_ptr<Node> parent_;
    glm::mat4 transform_;
};

