#include "axismaterial.hpp"

#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "program.hpp"
#include "shader.hpp"

#include "axisshaders.h"

AxisMaterial::AxisMaterial()
{
    std::shared_ptr<Shader> vertex;
    std::shared_ptr<Shader> fragment;

    {
        std::stringstream ss(axis_vert_glsl);
        vertex = std::make_shared<Shader>(ss, GL_VERTEX_SHADER);
    }

    {
        std::stringstream ss(axis_frag_glsl);
        fragment = std::make_shared<Shader>(ss, GL_FRAGMENT_SHADER);
    }

    std::vector<std::shared_ptr<Shader>> shaders { vertex, fragment };
    program_ = std::make_shared<Program>(shaders);

    glUseProgram(program_->get());

    model_view_projection_matrix_uniform_location_ = glGetUniformLocation(program_->get(), VAR_ModelViewProjectionMatrix);
}

void AxisMaterial::use()
{
    glUseProgram(program_->get());
}

void AxisMaterial::setModelViewProjection(const glm::mat4& model_view_projection_matrix)
{
    glUniformMatrix4fv(model_view_projection_matrix_uniform_location_, 1, GL_FALSE, glm::value_ptr(model_view_projection_matrix));
}
