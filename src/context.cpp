#include "context.hpp"

#include <GL/glew.h>

#include "window.hpp"

#define LOG_MODULE_NAME ("Context")
#include "log.hpp"

void GLAPIENTRY MessageCallback(
    GLenum source,
    GLenum type,
    GLuint id,
    GLsizei length,
    GLenum severity,
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
    
    // Decide GL+GLSL versions
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
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    #else
        // GL 3.0 + GLSL 130
        glsl_version_ = "#version 130";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    #endif

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

    //glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageCallback(MessageCallback, 0);
}

Context::~Context()
{
    LOG_INFO << "SDL_GL_DeleteContext" << std::endl;
    SDL_GL_DeleteContext(context_);
}

