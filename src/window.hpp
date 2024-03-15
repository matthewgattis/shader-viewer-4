#pragma once

#include <string>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>

class Window
{
public:
    Window(const std::string &window_title);
    ~Window();

    SDL_Window *get() const { return sdl_window_; }

    const std::pair<int, int> &getDefaultResolution() const;

    void setWindowTitle(const std::string &title);
    void setWindowSize(int width, int height);
    void setFullscreen(bool fullscreen);

    float getDpi() const;

private:
    SDL_Window *sdl_window_;

private:
    static const std::vector<std::pair<int, int>> default_resolution_list_;

    int resolution_selection_;

    std::string window_title_;
};

