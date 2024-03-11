#include "app.hpp"

#include <SDL2/SDL.h>

#include "window.hpp"
#include "context.hpp"
#include "sandboxmaterial.hpp"
#include "sandbox.hpp"

#define LOG_MODULE_NAME ("App")
#include "log.hpp"

App::App()
{
    LOG_INFO << "instance created. " << this << std::endl;
}

void App::run(const std::vector<std::string> &args)
{
    {
        int result;
        //result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
        result = SDL_Init(SDL_INIT_EVERYTHING);
        if (result)
            LOG_ERROR << "failure in SDL_Init. SDL_GetError: " << SDL_GetError() << std::endl;
    }

    auto window = std::make_shared<Window>("shader-viewer-4");
    const auto default_resolution = window->getDefaultResolution();

    auto context = std::make_shared<Context>(window);

    auto sandbox_material = std::make_shared<SandboxMaterial>("fragment.glsl");
    auto resolution = default_resolution;
    sandbox_material->setResolutionUniform(resolution.first, resolution.second);

    auto sandbox = std::make_shared<Sandbox>(sandbox_material);

    glClearColor(0.0, 0.0, 0.0, 0.0);

    Uint32 start_time = SDL_GetTicks();

    bool done = false;
    bool render = true;
    while (!done)
    {
        int frame_start = SDL_GetTicks();

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
                case SDL_QUIT:
                    done = true;
                    break;

                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_r:
                            LOG_INFO << "shader reload" << std::endl;
                            sandbox_material->reload();
                            sandbox_material->setResolutionUniform(resolution.first, resolution.second);
                            render = true;
                            break;

                        case SDLK_q:
                            LOG_INFO << "stop shader" << std::endl;
                            render = false;
                            break;
                        
                        case SDLK_t:
                            LOG_INFO << "reset time" << std::endl;
                            start_time = SDL_GetTicks();
                            break;

                        case SDLK_0:
                            window->setWindowSize(default_resolution.first, default_resolution.second);
                            break;

                        case SDLK_1:
                            window->setWindowSize(800, 600);
                            break;

                        case SDLK_2:
                            window->setWindowSize(1024, 768);
                            break;

                        case SDLK_3:
                            window->setWindowSize(1280, 960);
                            break;

                        case SDLK_4:
                            window->setWindowSize(1440, 1080);
                            break;

                        case SDLK_5:
                            window->setWindowSize(1600, 1200);
                            break;

                        case SDLK_6:
                            window->setWindowSize(2048, 1536);
                            break;
                    }
                    break;
                case SDL_WINDOWEVENT:
                    if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    {
                        LOG_INFO << "window resize: " << e.window.data1 << " " << e.window.data2 << std::endl;
                        resolution = std::make_pair(e.window.data1, e.window.data2);
                        sandbox_material->setResolutionUniform(resolution.first, resolution.second);
                        glViewport(0, 0, resolution.first, resolution.second);
                    }
                    break;
            }
        }

        glClear(GL_COLOR_BUFFER_BIT);

        if (render)
        {
            sandbox_material->setTimeUniform((SDL_GetTicks() - start_time) / 1000.0);

            const glm::mat4 identity(1.0);
            sandbox->render(identity);
        }

        SDL_GL_SwapWindow(window->get());

        int frame_end = SDL_GetTicks();
        int frame_time = frame_end - frame_start;
    }
}

App::~App()
{
    SDL_Quit();
}

