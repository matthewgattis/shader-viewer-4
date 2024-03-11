#include "program.hpp"

#include "shader.hpp"
#include <stdexcept>

#define LOG_MODULE_NAME ("Program")
#include "log.hpp"

Program::Program(const std::vector<std::shared_ptr<Shader>>& shaders) :
    shaders_(shaders)
{
    LOG_INFO << "instance created. " << this << std::endl;

    program_ = glCreateProgram();

    for (const auto& shader : shaders)
        glAttachShader(program_, shader->get());

    glLinkProgram(program_);

    GLint info_log_length = 0;
    glGetProgramiv(program_, GL_INFO_LOG_LENGTH, &info_log_length);

    std::string info_log;
    if (info_log_length > 0)
    {
        char *buffer = new char[info_log_length + 1]();

        glGetProgramInfoLog(program_, info_log_length, nullptr, buffer);

        info_log = buffer;
        delete[] buffer;
    }

    GLint link_status = 0;
    glGetProgramiv(program_, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE)
    {
        std::string err = "shader program link failed";
        LOG_WARNING << err<< std::endl;
        throw std::runtime_error(err);
    }
    else
    {
        if (info_log.length())
            LOG_INFO << "program linked successfully: " << info_log << std::endl;
        else
            LOG_INFO << "program linked successfully" << std::endl;
    }
}

Program::~Program()
{
    glDeleteProgram(program_);
}

