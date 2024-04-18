#include "app.hpp"

#include <SDL2/SDL.h>
#include <argparse/argparse.hpp>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "window.hpp"
#include "context.hpp"
#include "appcamera.hpp"
#include "uicontext.hpp"
#include "mainwindow.hpp"
#include "transform.hpp"
#include "axismaterial.hpp"
#include "axisgeometry.hpp"
#include "axis.hpp"
#include "renderer.hpp"
#include "renderable.hpp"

#define LOG_MODULE_NAME ("App")
#include "log.hpp"

const std::string App::APPLICATION_NAME = "voxel-experiement";

App::App(const std::vector<std::string> &args) :
    done_(false),
    fullscreen_(false)
{
    LOG_INFO << "instance created. " << this << std::endl;

    argparse::ArgumentParser program(APPLICATION_NAME, APPLICATION_NAME + " v0.1.0");

	program.parse_args(args);

    {
        int result;
        result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        if (result)
            LOG_ERROR << "failure in SDL_Init. SDL_GetError: " << SDL_GetError() << std::endl;
    }

    window_ = std::make_shared<Window>(APPLICATION_NAME);

    {
        const auto x = window_->getDefaultResolution();
        default_resolution_ = glm::vec2(x.first, x.second);
        resolution_ = default_resolution_;
    }

    context_ = std::make_shared<Context>(window_);
    ui_context_ = std::make_shared<UiContext>(window_, context_);

    main_window_ = std::make_shared<MainWindow>(
        ui_context_,
        APPLICATION_NAME,
        std::vector<std::pair<std::string, std::shared_ptr<Ui>>>
		{
		});

    root_ = std::make_shared<Transform>();
    app_camera_ = std::make_shared<AppCamera>(root_, resolution_);

    renderables_ = std::vector<std::shared_ptr<Renderable>>();
    {
        auto axis_material = std::make_shared<AxisMaterial>();
        auto axis_geometry = std::make_shared<AxisGeometry>();
        for (int i = 0; i < 16; i++)
            for (int j = 0; j < 16; j++)
            {
                auto transform = std::make_shared<Transform>(root_, glm::translate(glm::mat4(1.0f), glm::vec3(i, j, 0.0f)));
                renderables_.push_back(std::make_shared<Axis>(transform, axis_material, axis_geometry));
            }
    }

    renderer_ = std::make_shared<Renderer>();

    glEnable(GL_DEPTH_TEST);
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

                if ((ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard) == false)
                    app_camera_->handleEvents(e);

                handleEvents(e);
            }
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        {
            app_camera_->update(frame_delay / 1000.0f);

            renderer_->draw(root_, app_camera_, renderables_);
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
                LOG_INFO << "window resize: " << e.window.data1 << " " << e.window.data2 << std::endl;
                resolution_ = glm::vec3(e.window.data1, e.window.data2, (double)e.window.data1 / (double)e.window.data2);
                app_camera_->setResolution(glm::vec2(e.window.data1, e.window.data2));
                int width;
                int height;
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
#ifndef __APPLE__
                case SDLK_F11:
                    LOG_INFO << "fullscreen toggle" << std::endl;
                    fullscreen_ = !fullscreen_;
                    window_->setFullscreen(fullscreen_);
                    break;
#endif

                case SDLK_F10:
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

