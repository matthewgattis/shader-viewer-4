#pragma once

#include <memory>

#include "material.hpp"

class Program;

class AxisMaterial : public Material
{
public:
    AxisMaterial();
    ~AxisMaterial() = default;

    void use() override;

    void setModelViewProjection(const glm::mat4& model_view_projection_matrix) override;

private:
    std::shared_ptr<Program> program_;
    GLint model_view_projection_matrix_uniform_location_;
};

