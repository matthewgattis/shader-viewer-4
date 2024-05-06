#pragma once

#include <memory>
#include <string>

#include <SDL2/SDL.h>

class Window;

class Context
{
public:
    Context(const std::shared_ptr<Window>& window);
    ~Context();

    SDL_GLContext get() { return context_; }
    
    std::string getGlslVersion() const { return glsl_version_; }

private:
    SDL_GLContext context_;
    std::shared_ptr<Window> window_;
    
    std::string glsl_version_;
};

