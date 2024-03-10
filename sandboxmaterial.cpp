#include "sandboxmaterial.hpp"

#include <sstream>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>

#include "program.hpp"
#include "shader.hpp"

#define LOG_MODULE_NAME ("SandboxMaterial")
#include "log.hpp"

SandboxMaterial::SandboxMaterial(const std::string& fragment_shader_filename) :
    fragment_shader_filename_(fragment_shader_filename)
{
    LOG_INFO << "instance created. " << this << std::endl;

    vertex_ = std::make_shared<Shader>(
        "#version 140\nin vec2 in_Position; out vec2 ex_Position; void main() { ex_Position = in_Position; gl_Position = vec4(in_Position, 0., 1.); }",
        GL_VERTEX_SHADER);

    reload();
}

void SandboxMaterial::useMaterial()
{
    glUseProgram(program_->get());
}

void SandboxMaterial::reload()
{
    std::ifstream ifs(fragment_shader_filename_);
    std::vector<std::shared_ptr<Shader>> shaders
    {
        vertex_,
        std::make_shared<Shader>(ifs, GL_FRAGMENT_SHADER)
    };
    program_ = std::make_shared<Program>(shaders);

    glUseProgram(program_->get());

    GLint view_matrix_uniform_location;
    view_matrix_uniform_location = glGetUniformLocation(program_->get(), "ModelViewMatrix");
    if (view_matrix_uniform_location != GL_INVALID_VALUE && view_matrix_uniform_location != GL_INVALID_OPERATION)
        view_matrix_uniform_location_ = view_matrix_uniform_location;

    GLint resolution_uniform_location;
    resolution_uniform_location = glGetUniformLocation(program_->get(), "Resolution");
    if (resolution_uniform_location != GL_INVALID_VALUE && resolution_uniform_location != GL_INVALID_OPERATION)
        resolution_uniform_location_ = resolution_uniform_location;

    GLint time_uniform_location;
    time_uniform_location = glGetUniformLocation(program_->get(), "Time");
    if (time_uniform_location != GL_INVALID_VALUE && time_uniform_location != GL_INVALID_OPERATION)
        time_uniform_location_ = time_uniform_location;

    position_attrib_location_ = glGetAttribLocation(program_->get(), "in_Position");
}

void SandboxMaterial::setViewMatrixUniform(const glm::mat4& view_matrix)
{
    if (view_matrix_uniform_location_.has_value())
        glUniformMatrix4fv(view_matrix_uniform_location_.value(), 1, GL_FALSE, glm::value_ptr(view_matrix));
}

void SandboxMaterial::setResolutionUniform(float width, float height)
{
    if (resolution_uniform_location_.has_value())
        glUniform2f(resolution_uniform_location_.value(), width, height);
}

void SandboxMaterial::setTimeUniform(float time)
{
    if (time_uniform_location_.has_value())
        glUniform1f(time_uniform_location_.value(), time);
}

