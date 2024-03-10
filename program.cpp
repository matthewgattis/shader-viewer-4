#include "program.hpp"

#include "shader.hpp"

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

    if (info_log_length > 0)
    {
        char *info_log = new char[info_log_length + 1]();

        glGetProgramInfoLog(program_, info_log_length, nullptr, info_log);
        LOG_INFO << "program info log: " << info_log << std::endl;

        delete[] info_log;
    }

    GLint link_status = 0;
    glGetProgramiv(program_, GL_LINK_STATUS, &link_status);
    if (link_status == GL_FALSE)
        LOG_WARNING << "shader program link failed" << std::endl;
}

Program::~Program()
{
    glDeleteProgram(program_);
}

