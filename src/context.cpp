#include "context.hpp"

#include <GL/glew.h>

#include "window.hpp"

#define LOG_MODULE_NAME ("Context")
#include "log.hpp"

void GLAPIENTRY MessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(
        stdout,
        "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "",
        type,
        severity,
        message);
}

Context::Context(const std::shared_ptr<Window>& window) :
    window_(window)
{
    LOG_INFO << "instance created. " << this << std::endl;

    LOG_INFO << "SDL_GL_SetAttribte" << std::endl;
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    //SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    LOG_INFO << "SDL_GL_CreateContext" << std::endl;
    context_ = SDL_GL_CreateContext(window->get());
    if (context_ == nullptr)
        LOG_ERROR << "error in SDL_GL_CreateContext. SDL_GetError: " << SDL_GetError() << std::endl;

    {
        LOG_INFO << "glewInit" << std::endl;
        glewExperimental = GL_TRUE;
        GLenum err = glewInit();
        if (err != GLEW_OK)
            LOG_ERROR << "error in glewInit. glewGetErrorString: " << glewGetErrorString(err) << std::endl;
    }

    {
        LOG_INFO << "SDL_GL_MakeCurrent" << std::endl;
        int err = SDL_GL_MakeCurrent(window->get(), context_);
        if (err < 0)
            LOG_WARNING << "error in SDL_GL_MakeCurrent. SDL_GetError: " << SDL_GetError() << std::endl;
    }

    {
        LOG_INFO << "SDL_GL_SetSwapInterval" << std::endl;
        int err = SDL_GL_SetSwapInterval(1);
        if (err < 0)
            LOG_WARNING << "error in SDL_GL_SetSwapInterval. SDL_GetError: " << SDL_GetError() << std::endl;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, 0);
}

Context::~Context()
{
    LOG_INFO << "SDL_GL_DeleteContext" << std::endl;
    SDL_GL_DeleteContext(context_);
}

