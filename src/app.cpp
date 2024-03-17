#include "app.hpp"

#include <SDL2/SDL.h>
#include <argparse/argparse.hpp>

#include "window.hpp"
#include "context.hpp"
#include "sandboxmaterial.hpp"
#include "sandbox.hpp"
#include "camera.hpp"
#include "ui.hpp"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#define LOG_MODULE_NAME ("App")
#include "log.hpp"

App::App(const std::vector<std::string> &args) :
    done_(false),
    fullscreen_(false)
{
    LOG_INFO << "instance created. " << this << std::endl;

    argparse::ArgumentParser program("shader-viewer-4", "shader-viewer-4 v4.0.0");

    program.add_argument("shader_source")
        .help("shader file to load")
        .default_value(std::string("fragment.glsl"));

    program.parse_args(args);

    {
        int result;
        result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        if (result)
            LOG_ERROR << "failure in SDL_Init. SDL_GetError: " << SDL_GetError() << std::endl;
    }

    window_ = std::make_shared<Window>("shader-viewer-4");

    {
        const auto x = window_->getDefaultResolution();
        default_resolution_ = glm::vec3(x.first, x.second, (double)x.first / (double)x.second);
        resolution_ = default_resolution_;
    }

    context_ = std::make_shared<Context>(window_);
    ui_ = std::make_shared<Ui>(window_, context_);

    LOG_INFO << "loading shader source: " << program.get<std::string>("shader_source") << std::endl;

    sandbox_material_ = std::make_shared<SandboxMaterial>(program.get<std::string>("shader_source"));
    sandbox_material_->setResolutionUniform(default_resolution_);

    sandbox_ = std::make_shared<Sandbox>(sandbox_material_);
    camera_ = std::make_shared<Camera>(1.0, 0.001, 1000.0);

    Uint32 start_time_ = SDL_GetTicks();
}

void App::run()
{
    int frame_delay = 0;
    bool show_demo_window = true;
    glClearColor(0.0, 0.0, 0.0, 0.0);

    while (!done_)
    {
        int frame_start = SDL_GetTicks();

        {
            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                ImGui_ImplSDL2_ProcessEvent(&e);

                camera_->handleEvents(
                    e,
                    (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard) == false);

                handleEvents(e);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        {
            camera_->update(frame_delay / 1000.0f);

            sandbox_material_->setViewMatrixUniform(glm::inverse(camera_->get()));
            sandbox_material_->setTimeUniform((SDL_GetTicks() - start_time_) / 1000.0);

            sandbox_->render();
        }

        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        SDL_GL_SwapWindow(window_->get());

        int frame_end = SDL_GetTicks();
        frame_delay = frame_end - frame_start;
    }
}

void App::handleEvents(const SDL_Event& e)
{

    switch (e.type)
    {
        case SDL_QUIT:
            done_ = true;
            break;

        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
                case SDLK_r:
                    LOG_INFO << "shader reload" << std::endl;
                    sandbox_material_->reload();
                    sandbox_material_->setResolutionUniform(resolution_);
                    break;

                case SDLK_q:
                    LOG_INFO << "stop shader" << std::endl;
                    sandbox_material_->blank();
                    break;
                
                case SDLK_t:
                    LOG_INFO << "reset time" << std::endl;
                    start_time_ = SDL_GetTicks();
                    break;

                case SDLK_f:
                case SDLK_F11:
                    LOG_INFO << "fullscreen toggle" << std::endl;
                    fullscreen_ = !fullscreen_;
                    window_->setFullscreen(fullscreen_);
                    break;

                case SDLK_0:
                    window_->setWindowSize(default_resolution_.x, default_resolution_.y);
                    break;

                case SDLK_1:
                    window_->setWindowSize(800, 600);
                    break;

                case SDLK_2:
                    window_->setWindowSize(1024, 768);
                    break;

                case SDLK_3:
                    window_->setWindowSize(1280, 960);
                    break;

                case SDLK_4:
                    window_->setWindowSize(1440, 1080);
                    break;

                case SDLK_5:
                    window_->setWindowSize(1600, 1200);
                    break;

                case SDLK_6:
                    window_->setWindowSize(2048, 1536);
                    break;

                case SDLK_7:
                    window_->setWindowSize(1366, 768);
                    break;
            }
            break;

        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                LOG_INFO << "window resize: " << e.window.data1 << " " << e.window.data2 << std::endl;
                resolution_ = glm::vec3(e.window.data1, e.window.data2, (double)e.window.data1 / (double)e.window.data2);
                sandbox_material_->setResolutionUniform(resolution_);
                glViewport(0, 0, e.window.data1, e.window.data2);
            }
            break;
    }
}

App::~App()
{
    SDL_Quit();
}

