#include "app.hpp"

#include <SDL2/SDL.h>
#include <argparse/argparse.hpp>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "window.hpp"
#include "context.hpp"
#include "sandboxmaterial.hpp"
#include "sandbox.hpp"
#include "camera.hpp"
#include "uicontext.hpp"
#include "mainwindow.hpp"

#include "cameraui.hpp"

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
        .required();

    program.add_argument("-l", "--low-dpi")
        .help("switch to disable high dpi")
        .default_value(false)
        .implicit_value(true);

	program.parse_args(args);

    {
        int result;
        result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        if (result)
            LOG_ERROR << "failure in SDL_Init. SDL_GetError: " << SDL_GetError() << std::endl;
    }

    std::string shader_source_name = program.get<std::string>("shader_source");

    high_dpi_ = !program.get<bool>("-l");
    
    window_ = std::make_shared<Window>("shader-viewer-4, " + shader_source_name, high_dpi_);

    {
        const auto x = window_->getDefaultResolution();
        default_resolution_ = glm::vec3(x.first, x.second, (double)x.first / (double)x.second);
        resolution_ = default_resolution_;
    }

    context_ = std::make_shared<Context>(window_);
    ui_context_ = std::make_shared<UiContext>(window_, context_);

    LOG_INFO << "loading shader source: " << shader_source_name << std::endl;

    sandbox_material_ = std::make_shared<SandboxMaterial>(program.get<std::string>("shader_source"));
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

            sandbox_material_->setViewMatrixUniform(camera_->view_matrix_);
            sandbox_material_->setTimeUniform((SDL_GetTicks() - start_time_) / 1000.0);

            sandbox_->render();
        }

        {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            main_window_->update();

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

        case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
            {
                int width = e.window.data1;
                int height = e.window.data2;

                LOG_INFO << "window resize: " << width << " " << height << std::endl;

                resolution_ = glm::vec3(width, height, (double)width/ (double)height);
                sandbox_material_->setResolutionUniform(resolution_);

                if (high_dpi_)
                    SDL_GL_GetDrawableSize(window_->get(), &width, &height);

                glViewport(0, 0, width, height);
            }
            break;
    }

    if (ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard)
		return;

    switch (e.type)
    {
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

                case SDLK_g:
                    LOG_INFO << "toggle GUI" << std::endl;
                    main_window_->toggleShow();
					break;

                case SDLK_0:
                    window_->setWindowSize(
                        default_resolution_.x,
                        default_resolution_.y);
                    break;

                case SDLK_1:
                case SDLK_2:
                case SDLK_3:
                case SDLK_4:
                case SDLK_5:
                case SDLK_6:
                case SDLK_7:
                    window_->setWindowSize(
                        window_->default_resolution_list_[e.key.keysym.sym - SDLK_1].first,
                        window_->default_resolution_list_[e.key.keysym.sym - SDLK_1].second);
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

