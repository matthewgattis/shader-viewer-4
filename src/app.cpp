#include "app.hpp"

#include <SDL3/SDL.h>
#include <argparse/argparse.hpp>

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

#include "window.hpp"
#include "context.hpp"
#include "sandboxmaterial.hpp"
#include "sandbox.hpp"
#include "camera.hpp"
#include "uicontext.hpp"
#include "mainwindow.hpp"
#include "cameraui.hpp"

#define LOG_MODULE_NAME "App"
#include "log.hpp"

App::App(const std::vector<std::string> &args) :
    done_(false),
    fullscreen_(false)
{
    argparse::ArgumentParser program("shader-viewer-4", "shader-viewer-4 v4.0.0");

    program.add_argument("shader_source")
        .help("shader file to load")
        .required();

    program.add_argument("-l", "--low-dpi")
        .help("disable high-DPI scaling")
        .default_value(false)
        .implicit_value(true);

    program.parse_args(args);

    if (!SDL_Init(SDL_INIT_VIDEO))
        LOG_ERROR("SDL_Init failed: {}", SDL_GetError());

    std::string shader_source_name = program.get<std::string>("shader_source");
    high_dpi_ = !program.get<bool>("-l");

    LOG_INFO("loading shader: {}", shader_source_name);

    window_ = std::make_shared<Window>("shader-viewer-4 — " + shader_source_name, high_dpi_);

    {
        const auto& res = window_->getDefaultResolution();
        default_resolution_ = glm::vec3(res.first, res.second, (double)res.first / (double)res.second);
        resolution_ = default_resolution_;
    }

    context_ = std::make_shared<Context>(window_);
    ui_context_ = std::make_shared<UiContext>(window_, context_);

    sandbox_material_ = std::make_shared<SandboxMaterial>(shader_source_name);
    sandbox_material_->setResolutionUniform(default_resolution_);

    sandbox_ = std::make_shared<Sandbox>(sandbox_material_);
    camera_ = std::make_shared<Camera>(1.0, 0.001, 1000.0);

    main_window_ = std::make_shared<MainWindow>(
        ui_context_,
        "shader-viewer-4",
        std::vector<std::pair<std::string, std::shared_ptr<Ui>>>
        {
            std::make_pair("Camera", std::make_shared<CameraUi>(camera_)),
        });

    start_time_ = SDL_GetTicks();
}

void App::run()
{
    Uint64 frame_delay = 0;
    glClearColor(0.0, 0.0, 0.0, 0.0);

    while (!done_)
    {
        Uint64 frame_start = SDL_GetTicks();

        {
            SDL_Event e;
            while (SDL_PollEvent(&e))
            {
                ImGui_ImplSDL3_ProcessEvent(&e);

                camera_->handleEvents(
                    e,
                    (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard) == false,
                    window_->get());

                handleEvents(e);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        camera_->update(frame_delay / 1000.0f);
        sandbox_material_->setViewMatrixUniform(camera_->view_matrix_);
        sandbox_material_->setTimeUniform((SDL_GetTicks() - start_time_) / 1000.0f);
        sandbox_->render();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        main_window_->update();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        SDL_GL_SwapWindow(window_->get());

        Uint64 frame_end = SDL_GetTicks();
        frame_delay = frame_end - frame_start;
    }
}

void App::handleEvents(const SDL_Event& e)
{
    switch (e.type)
    {
        case SDL_EVENT_QUIT:
            done_ = true;
            break;

        case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
        {
            int width = e.window.data1;
            int height = e.window.data2;
            LOG_DEBUG("window pixel size changed: {}x{}", width, height);
            resolution_ = glm::vec3(width, height, (double)width / (double)height);
            sandbox_material_->setResolutionUniform(resolution_);
            glViewport(0, 0, width, height);
            break;
        }
    }

    if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
        return;

    switch (e.type)
    {
        case SDL_EVENT_KEY_DOWN:
            switch (e.key.key)
            {
                case SDLK_R:
                    LOG_INFO("reloading shader");
                    sandbox_material_->reload();
                    sandbox_material_->setResolutionUniform(resolution_);
                    break;

                case SDLK_Q:
                    LOG_INFO("unloading shader");
                    sandbox_material_->blank();
                    break;

                case SDLK_T:
                    LOG_INFO("resetting time");
                    start_time_ = SDL_GetTicks();
                    break;

                case SDLK_F:
                case SDLK_F11:
                    fullscreen_ = !fullscreen_;
                    LOG_INFO("fullscreen: {}", fullscreen_);
                    window_->setFullscreen(fullscreen_);
                    break;

                case SDLK_G:
                    LOG_INFO("toggling GUI");
                    main_window_->toggleShow();
                    break;

                case SDLK_0:
                    window_->setWindowSize(default_resolution_.x, default_resolution_.y);
                    break;

                case SDLK_1:
                case SDLK_2:
                case SDLK_3:
                case SDLK_4:
                case SDLK_5:
                case SDLK_6:
                case SDLK_7:
                    window_->setWindowSize(
                        window_->default_resolution_list_[e.key.key - SDLK_1].first,
                        window_->default_resolution_list_[e.key.key - SDLK_1].second);
                    break;
            }
            break;

        default:
            break;
    }
}

App::~App()
{
    SDL_Quit();
}
