#include "shader.hpp"

#include <cstring>
#include <sstream>

#define LOG_MODULE_NAME ("Shader")
#include "log.hpp"

Shader::Shader(std::istream& is, GLenum shader_type)
{
    LOG_INFO << "instance created. " << this << std::endl;
    LOG_INFO << "shader " << shader_type << std::endl;

    shader_ = glCreateShader(shader_type);

    // Read source code from stream.
    std::stringstream ss;
    ss << is.rdbuf();

    char *strings = new char[ss.str().size() + 1]();
    ss.str().copy(strings, ss.str().size());

    glShaderSource(shader_, 1, (const char**)&strings, nullptr);

    glCompileShader(shader_);

    delete[] strings;

    log();
}

Shader::Shader(const std::string& source, GLenum shader_type)
{
    LOG_INFO << "instance created. " << this << std::endl;
    LOG_INFO << "shader " << shader_type << std::endl;

    shader_ = glCreateShader(shader_type);

    char *strings = new char[source.size() + 1]();
    source.copy(strings, source.size());

    glShaderSource(shader_, 1, (const char**)&strings, nullptr);

    glCompileShader(shader_);

    delete[] strings;

    log();
}

void Shader::log()
{
    GLint info_log_length = 0;
    glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &info_log_length);

    std::string info_log;
    if (info_log_length > 0)
    {
        GLchar *buffer = new GLchar[info_log_length + 1]();

        glGetShaderInfoLog(shader_, info_log_length, nullptr, buffer);

        info_log = buffer;
        delete[] buffer;
    }

    GLint compile_status;
    glGetShaderiv(shader_, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE)
    {
        std::string err = "shader compilation failed: " + info_log;
        LOG_ERROR << err << std::endl;
        throw std::runtime_error(err);
    }
    else
    {
        if (info_log.length() > 0)
            LOG_INFO << "shader compiled successfully: " << info_log << std::endl;
        else
            LOG_INFO << "shader compiled successfully" << std::endl;
    }
}

Shader::~Shader()
{
    glDeleteShader(shader_);
}

