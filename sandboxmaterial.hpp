#pragma once

#include <memory>
#include <string>
#include <optional>

#include <glm/glm.hpp>

#include "material.hpp"

class Program;
class Shader;

class SandboxMaterial : public Material
{
public:
    SandboxMaterial(const std::string& fragment_shader_filename);
    SandboxMaterial() = delete;

    void useMaterial() override;

    GLint getPositionAttrib() override { return position_attrib_location_; }

    void setViewMatrixUniform(const glm::mat4& view_matrix) override;
    void setResolutionUniform(float width, float height);
    void setTimeUniform(float time);

    void reload();

private:
    std::string fragment_shader_filename_;

    std::shared_ptr<Program> program_;
    std::shared_ptr<Shader> vertex_;
    std::shared_ptr<Shader> nothing_fragment_shader_;

    std::optional<GLint> view_matrix_uniform_location_;
    std::optional<GLint> resolution_uniform_location_;
    std::optional<GLint> time_uniform_location_;
    GLint position_attrib_location_;
};

