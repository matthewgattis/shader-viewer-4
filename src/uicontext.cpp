#include "uicontext.hpp"

#include "window.hpp"
#include "context.hpp"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

UiContext::UiContext(
    const std::shared_ptr<Window>& window,
    const std::shared_ptr<Context>& context)
{
    float ddpi = window->getDpi();
    ddpi = (int)(ddpi / 144.0f);
    if (ddpi < 1.0f)
        ddpi = 1.0f;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
    ImGui::GetStyle().ScaleAllSizes(ddpi);
    ImGui::GetIO().FontGlobalScale = ddpi;

    ImGui_ImplSDL2_InitForOpenGL(window->get(), context->get());
    ImGui_ImplOpenGL3_Init("#version 130");
}

UiContext::~UiContext()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
}

