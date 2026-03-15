#include "program.hpp"

#include <stdexcept>

#include "shader.hpp"

#define LOG_MODULE_NAME "Program"
#include "log.hpp"

Program::Program(const std::vector<std::shared_ptr<Shader>>& shaders) :
    shaders_(shaders)
{
    program_ = glCreateProgram();
    if (program_ == GL_NONE)
    {
        std::string err("glCreateProgram failed");
        LOG_ERROR(err);
        throw std::runtime_error(err);
    }

    for (const auto& shader : shaders)
        glAttachShader(program_, shader->get());

    glLinkProgram(program_);

    GLint info_log_length = 0;
    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &info_log_length);

    std::string info_log;
    if (info_log_length > 0)
    {
        info_log.resize(info_log_length);
        glGetProgramInfoLog(program_, info_log_length, nullptr, info_log.data());
        while (!info_log.empty() && (info_log.back() == '\n' || info_log.back() == '\r'))
            info_log.pop_back();
    }

    GLint link_status = 0;
    glGetProgramiv(program_, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE)
    {
        std::string err = "program link failed: " + info_log;
        LOG_ERROR(err);
        throw std::runtime_error(err);
    }

    if (!info_log.empty())
        LOG_DEBUG("program linked (with warnings): {}", info_log);
    else
        LOG_DEBUG("program linked successfully (id={})", program_);
}

Program::~Program()
{
    glUseProgram(GL_NONE);
    glDeleteProgram(program_);
}
