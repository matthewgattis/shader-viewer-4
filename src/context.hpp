#pragma once

#include <memory>

#include <SDL2/SDL.h>

class Window;

class Context
{
public:
    Context(const std::shared_ptr<Window>& window);
    ~Context();

    SDL_GLContext get() { return context_; }

private:
    SDL_GLContext context_;
    std::shared_ptr<Window> window_;
};

