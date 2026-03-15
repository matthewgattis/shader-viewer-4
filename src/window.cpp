#include "window.hpp"

#define LOG_MODULE_NAME ("Window")
#include "log.hpp"

const std::vector<std::pair<int, int>> Window::default_resolution_list_ = 
{
    std::pair<int, int>(640, 480),
    std::pair<int, int>(800, 600),
    std::pair<int, int>(1024, 768),
    std::pair<int, int>(1280, 960),
    std::pair<int, int>(1440, 1080),
    std::pair<int, int>(1600, 1200),
    std::pair<int, int>(2048, 1536)
};

Window::Window(const std::string &window_title, bool high_dpi) :
    sdl_window_(nullptr),
    window_title_(window_title)
{
    LOG_INFO << "instance created. " << this << std::endl;

    SDL_DisplayID display_id = SDL_GetPrimaryDisplay();
    const SDL_DisplayMode *display_mode = SDL_GetDesktopDisplayMode(display_id);

    float content_scale = SDL_GetDisplayContentScale(display_id);
    LOG_INFO << "Display content scale (" << content_scale << ")" << std::endl;

    int selection = 0;
    for (int i = 0; i < (int)default_resolution_list_.size(); i++)
    {
        if (default_resolution_list_[i].first < display_mode->w &&
            default_resolution_list_[i].second < display_mode->h)
        {
            selection = i;
            continue;
        }
        break;
    }

    resolution_selection_ = selection;

    LOG_INFO << "Window width (" << default_resolution_list_[selection].first << ")" << std::endl;
    LOG_INFO << "Window height (" << default_resolution_list_[selection].second << ")" << std::endl;

    SDL_WindowFlags window_flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
    if (high_dpi)
        window_flags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;

    sdl_window_ = SDL_CreateWindow(
        window_title.c_str(),
        default_resolution_list_[selection].first,
        default_resolution_list_[selection].second,
        window_flags);

    if (!sdl_window_)
    {
        LOG_ERROR << "Failure in SDL_CreateWindow. (" << SDL_GetError() << ")" << std::endl;
        throw std::exception();
    }
}

Window::~Window()
{
    if (sdl_window_)
        SDL_DestroyWindow(sdl_window_);
}

const std::pair<int, int> &Window::getDefaultResolution() const
{
    return default_resolution_list_[resolution_selection_];
}

void Window::setWindowTitle(const std::string& window_title)
{
    SDL_SetWindowTitle(sdl_window_, (window_title_ + " " + window_title).c_str());
}

void Window::setWindowSize(int width, int height)
{
    SDL_SetWindowSize(sdl_window_, width, height);
}

void Window::setFullscreen(bool fullscreen)
{
    SDL_SetWindowFullscreen(sdl_window_, fullscreen);
}

float Window::getDpi() const
{
    return SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
}
