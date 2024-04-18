#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

class Window;
class Context;
class UiContext;
class MainWindow;
class AppCamera;
class Transform;
class Axis;
class Renderer;
class Renderable;

class App
{
public:
    App() = delete;
    App(const std::vector<std::string>& args);
    ~App();

    void run();

    void handleEvents(const SDL_Event& e);

private:
    bool done_;
    
    std::shared_ptr<Window> window_;
    std::shared_ptr<Context> context_;

    std::shared_ptr<UiContext> ui_context_;
    std::shared_ptr<MainWindow> main_window_;
    
    glm::vec2 resolution_;
    glm::vec2 default_resolution_;
    bool fullscreen_;

    std::shared_ptr<Transform> root_;
    std::shared_ptr<AppCamera> app_camera_;
    std::shared_ptr<Axis> axis_;
    std::shared_ptr<Axis> axis_2_;

    std::shared_ptr<Renderer> renderer_;
    std::vector<std::shared_ptr<Renderable>> renderables_;

public:
    static const std::string APPLICATION_NAME;
};

