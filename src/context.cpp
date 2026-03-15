#include "context.hpp"

#include <GL/glew.h>

#include "window.hpp"

#define LOG_MODULE_NAME "Context"
#include "log.hpp"

Context::Context(const std::shared_ptr<Window>& window) :
    window_(window)
{
    // Decide GL + GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    glsl_version_ = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    glsl_version_ = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 Core + GLSL 130
    glsl_version_ = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    context_ = SDL_GL_CreateContext(window->get());
    if (!context_)
        LOG_ERROR("SDL_GL_CreateContext failed: {}", SDL_GetError());

    LOG_DEBUG("GLSL version: {}", glsl_version_);

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK)
        LOG_ERROR("glewInit failed: {}", reinterpret_cast<const char*>(glewGetErrorString(err)));

    if (SDL_GL_MakeCurrent(window->get(), context_) != 0)
        LOG_WARN("SDL_GL_MakeCurrent failed: {}", SDL_GetError());

    if (!SDL_GL_SetSwapInterval(1))
        LOG_WARN("SDL_GL_SetSwapInterval failed: {}", SDL_GetError());

    LOG_INFO("OpenGL {}, renderer: {}",
        reinterpret_cast<const char*>(glGetString(GL_VERSION)),
        reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
}

Context::~Context()
{
    SDL_GL_DestroyContext(context_);
}
