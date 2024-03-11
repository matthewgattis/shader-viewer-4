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
    if (program_ == GL_NONE)
    {
        std::string err("error in glCreateProgram: " + std::to_string(program_) + ": error creating program object");
        LOG_ERROR << err << std::endl;
        throw std::runtime_error(err);
    }

    LOG_INFO << "created program object: program_: " << program_ << std::endl;

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
        std::string err = "shader program link failed: info_log: " + info_log;
        LOG_WARNING << err<< std::endl;
        throw std::runtime_error(err);
    }
    else
        LOG_INFO << "program linked successfully: info_log: " << info_log << std::endl;
}

Program::~Program()
{
    glUseProgram(GL_NONE);
    LOG_INFO << "delete program object: program_: " << program_ << std::endl;
    glDeleteProgram(program_);
}

