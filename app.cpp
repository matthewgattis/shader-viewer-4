#include "app.hpp"

#include <SDL2/SDL.h>

#include "window.hpp"
#include "context.hpp"
#include "sandboxmaterial.hpp"
#include "sandbox.hpp"
#include "camera.hpp"

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
        result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
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

    auto camera = std::make_shared<Camera>(1.0, 0.01, 100.0);

    glClearColor(0.0, 0.0, 0.0, 0.0);

    Uint32 start_time = SDL_GetTicks();
    bool fullscreen = false;
    glm::vec3 movement(0.0, 0.0, 0.0);
    int frame_time = 0;

    bool done = false;

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

                case SDL_MOUSEMOTION:
					{
						if (e.motion.state & SDL_BUTTON_LMASK)
							camera->rotate2(e.motion.xrel, e.motion.yrel);
						if (e.motion.state & SDL_BUTTON_RMASK)
							camera->rotate(e.motion.xrel, e.motion.yrel);
						else if (e.motion.state & SDL_BUTTON_MMASK)
							camera->pan(e.motion.xrel, e.motion.yrel);
					}
					break;

                case SDL_MOUSEWHEEL:
                    camera->zoom(e.wheel.preciseY);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (e.button.button == SDL_BUTTON_LEFT ||
                        e.button.button == SDL_BUTTON_RIGHT ||
                        e.button.button == SDL_BUTTON_MIDDLE)
                    {
                        SDL_ShowCursor(SDL_DISABLE);
                        SDL_SetRelativeMouseMode(SDL_TRUE);
                    }
					break;

                case SDL_MOUSEBUTTONUP:
                    if (e.button.button == SDL_BUTTON_LEFT ||
                        e.button.button == SDL_BUTTON_RIGHT ||
                        e.button.button == SDL_BUTTON_MIDDLE)
                    {
                        SDL_ShowCursor(SDL_ENABLE);
                        SDL_SetRelativeMouseMode(SDL_FALSE);
                    }
					break;

                case SDL_KEYUP:
                    switch (e.key.keysym.sym)
                    {
						case SDLK_w:
                            movement.z = 0.0;
                            break;

						case SDLK_a:
                            movement.x = 0.0;
                            break;

						case SDLK_s:
                            movement.z = 0.0;
                            break;

						case SDLK_d:
                            movement.x = 0.0;
                            break;

                        case SDLK_SPACE:
							movement.y = 0.0;
							break;

                        case SDLK_LSHIFT:
							movement.y = 0.0;
							break;
                    }
                    break;

                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym)
                    {
						case SDLK_w:
                            movement.z = 1.0;
                            break;

						case SDLK_a:
                            movement.x = 1.0;
                            break;

						case SDLK_s:
                            movement.z = -1.0;
                            break;

						case SDLK_d:
                            movement.x = -1.0;
                            break;

                        case SDLK_SPACE:
							movement.y = -1.0;
							break;

                        case SDLK_LSHIFT:
							movement.y = 1.0;
							break;

                        case SDLK_r:
                            LOG_INFO << "shader reload" << std::endl;
                            sandbox_material->reload();
                            sandbox_material->setResolutionUniform(resolution.first, resolution.second);
                            break;

                        case SDLK_q:
                            LOG_INFO << "stop shader" << std::endl;
                            sandbox_material->blank();
                            fullscreen = false;
							window->setFullscreen(fullscreen);
                            break;
                        
                        case SDLK_t:
                            LOG_INFO << "reset time" << std::endl;
                            start_time = SDL_GetTicks();
                            break;

                        case SDLK_c:
                            LOG_INFO << "camera reset" << std::endl;
                            camera = std::make_shared<Camera>(1.0, 0.01, 100.0);
                            break;

                        case SDLK_f:
                        case SDLK_F11:
                            LOG_INFO << "fullscreen toggle" << std::endl;
							fullscreen = !fullscreen;
							window->setFullscreen(fullscreen);
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

                        case SDLK_7:
                            window->setWindowSize(1366, 768);
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

        {
            camera->update(frame_time / 1000.0f, movement);

            sandbox_material->setViewMatrixUniform(glm::inverse(camera->get()));
            sandbox_material->setTimeUniform((SDL_GetTicks() - start_time) / 1000.0);

            sandbox->render();
        }

        SDL_GL_SwapWindow(window->get());

        int frame_end = SDL_GetTicks();
        frame_time = frame_end - frame_start;
    }
}

App::~App()
{
    SDL_Quit();
}

