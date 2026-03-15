#include "sandboxmaterial.hpp"

#include <fstream>
#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include "program.hpp"
#include "shader.hpp"

#define LOG_MODULE_NAME "SandboxMaterial"
#include "log.hpp"

SandboxMaterial::SandboxMaterial(const std::string& fragment_shader_filename) :
    fragment_shader_filename_(fragment_shader_filename)
{
    {
        std::stringstream ss(
            "#version 150\n"
            "in vec2 in_Position;\n"
            "uniform vec3 Resolution;\n"
            "out vec2 FragCoord;\n"
            "void main() {\n"
            "    FragCoord = in_Position * vec2(Resolution.z, 1.0);\n"
            "    gl_Position = vec4(in_Position, 0.0, 1.0);\n"
            "}");
        vertex_ = std::make_shared<Shader>(ss, GL_VERTEX_SHADER);
    }

    {
        std::stringstream ss(
            "#version 150\n"
            "out vec4 FragColor;\n"
            "void main() { FragColor = vec4(0.0); }");
        blank_fragment_shader_ = std::make_shared<Shader>(ss, GL_FRAGMENT_SHADER);
    }

    reload();
}

void SandboxMaterial::useMaterial()
{
    glUseProgram(program_->get());
}

void SandboxMaterial::reload()
{
    std::ifstream ifs(fragment_shader_filename_);
    if (!ifs)
        LOG_WARN("could not open shader file: {}", fragment_shader_filename_);

    try
    {
        auto fragment = std::make_shared<Shader>(ifs, GL_FRAGMENT_SHADER);
        program_ = std::make_shared<Program>(std::vector<std::shared_ptr<Shader>>{ vertex_, fragment });
        LOG_INFO("shader loaded: {}", fragment_shader_filename_);
    }
    catch (const std::runtime_error& e)
    {
        LOG_WARN("shader compile/link failed, using blank fallback: {}", e.what());
        program_ = std::make_shared<Program>(std::vector<std::shared_ptr<Shader>>{ vertex_, blank_fragment_shader_ });
    }

    glUseProgram(program_->get());

    auto query = [&](const char* name, std::optional<GLint>& loc) {
        GLint l = glGetUniformLocation(program_->get(), name);
        if (l != GL_INVALID_VALUE && l != GL_INVALID_OPERATION && l != -1)
            loc = l;
        else
            loc = std::nullopt;
    };

    query("ViewMatrix", view_matrix_uniform_location_);
    query("Resolution", resolution_uniform_location_);
    query("Time",       time_uniform_location_);

    position_attrib_location_ = glGetAttribLocation(program_->get(), "in_Position");
}

void SandboxMaterial::blank()
{
    program_ = std::make_shared<Program>(
        std::vector<std::shared_ptr<Shader>>{ vertex_, blank_fragment_shader_ });

    glUseProgram(program_->get());

    view_matrix_uniform_location_ = std::nullopt;
    resolution_uniform_location_  = std::nullopt;
    time_uniform_location_        = std::nullopt;
    position_attrib_location_     = glGetAttribLocation(program_->get(), "in_Position");
}

void SandboxMaterial::setViewMatrixUniform(const glm::mat4& view_matrix)
{
    if (view_matrix_uniform_location_)
        glUniformMatrix4fv(*view_matrix_uniform_location_, 1, GL_FALSE, glm::value_ptr(view_matrix));
}

void SandboxMaterial::setResolutionUniform(const glm::vec3& resolution)
{
    if (resolution_uniform_location_)
        glUniform3fv(*resolution_uniform_location_, 1, glm::value_ptr(resolution));
}

void SandboxMaterial::setTimeUniform(float time)
{
    if (time_uniform_location_)
        glUniform1f(*time_uniform_location_, time);
}
