#include "shader.hpp"

#include <sstream>

#define LOG_MODULE_NAME "Shader"
#include "log.hpp"

Shader::Shader(std::istream& is, GLenum type) :
    type_(type)
{
    shader_ = glCreateShader(type_);
    if (shader_ == GL_NONE)
    {
        std::string err("glCreateShader failed");
        LOG_ERROR(err);
        throw std::runtime_error(err);
    }

    std::stringstream ss;
    ss << is.rdbuf();
    std::string source = ss.str();
    const char* src = source.c_str();

    glShaderSource(shader_, 1, &src, nullptr);
    glCompileShader(shader_);

    GLint info_log_length = 0;
    glGetShaderiv(shader_, GL_INFO_LOG_LENGTH, &info_log_length);

    std::string info_log;
    if (info_log_length > 0)
    {
        info_log.resize(info_log_length);
        glGetShaderInfoLog(shader_, info_log_length, nullptr, info_log.data());
        // Strip trailing newline that drivers often append
        while (!info_log.empty() && (info_log.back() == '\n' || info_log.back() == '\r'))
            info_log.pop_back();
    }

    GLint compile_status;
    glGetShaderiv(shader_, GL_COMPILE_STATUS, &compile_status);
    if (compile_status == GL_FALSE)
    {
        std::string err = "shader compilation failed: " + info_log;
        LOG_ERROR(err);
        throw std::runtime_error(err);
    }

    if (!info_log.empty())
        LOG_DEBUG("shader compiled (with warnings): {}", info_log);
    else
        LOG_DEBUG("shader compiled successfully (id={})", shader_);
}

void Shader::log()
{
}

Shader::~Shader()
{
    glDeleteShader(shader_);
}
