#include "uicontext.hpp"

#include "window.hpp"
#include "context.hpp"

#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_opengl3.h"

UiContext::UiContext(
    const std::shared_ptr<Window>& window,
    const std::shared_ptr<Context>& context)
{
    float scale = window->getDpi();
    scale = (int)(scale / 144.0f);
    if (scale < 1.0f)
        scale = 1.0f;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui::GetStyle().ScaleAllSizes(scale);
    ImGui::GetIO().FontGlobalScale = scale;

    ImGui_ImplSDL3_InitForOpenGL(window->get(), context->get());
    ImGui_ImplOpenGL3_Init(context->getGlslVersion().c_str());
}

UiContext::~UiContext()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}
