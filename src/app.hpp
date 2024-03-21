#pragma once

#include <string>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

class Window;
class Context;
class UiContext;
class SandboxMaterial;
class Sandbox;
class Camera;
class MainWindow;

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
    
    std::shared_ptr<Context> context_;
    std::shared_ptr<Window> window_;
    std::shared_ptr<UiContext> ui_context_;

    std::shared_ptr<SandboxMaterial> sandbox_material_;
    std::shared_ptr<Sandbox> sandbox_;
    std::shared_ptr<Camera> camera_;

    std::shared_ptr<MainWindow> main_window_;
    
    glm::vec3 resolution_;
    glm::vec3 default_resolution_;

    Uint32 start_time_;
    bool fullscreen_;
};

