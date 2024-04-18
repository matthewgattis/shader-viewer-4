#include "mainwindow.hpp"

#include "imgui.h"

#include "uicontext.hpp"

MainWindow::MainWindow(
	const std::shared_ptr<UiContext>& ui_context,
	const std::string name,
	const std::vector<std::pair<std::string, std::shared_ptr<Ui>>>& uis) :
	show_(false),
	name_(name),
	uis_(uis),
	ui_context_(ui_context)
{
}

void MainWindow::update()
{
	if (!show_)
		return;

	ImGui::Begin(
		name_.c_str(),
		&show_,
		ImGuiWindowFlags_AlwaysAutoResize);
	{
		ImGui::PushID(this);

		static bool show_demo_window = false;
		{
			ImGui::Text("ImGui");
			ImGui::Checkbox("show_demo_window", &show_demo_window);
		}
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		for (const auto& ui : uis_)
		{
			ImGui::PushID(ui.second.get());

			ImGui::Separator();
			ImGui::Text("%s", ui.first.c_str());
			ui.second->update();

			ImGui::PopID();
		}

		ImGui::PopID();
	}
	ImGui::End();
}
