#include "axis.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL2/SDL.h>

#include "axismaterial.hpp"
#include "axisgeometry.hpp"
#include "node.hpp"

Axis::Axis(
    const std::shared_ptr<Node>& parent,
    const std::shared_ptr<AxisMaterial>& axis_material,
    const std::shared_ptr<AxisGeometry>& axis_geometry) :
    parent_(parent),
    transform_(1.0f),
    axis_material_(axis_material),
    axis_geometry_(axis_geometry)
{
}

void Axis::draw(const glm::mat4& model_view_projection_matrix)
{
    axis_material_->use();
    axis_material_->setModelViewProjection(model_view_projection_matrix);
    axis_geometry_->draw();
}

