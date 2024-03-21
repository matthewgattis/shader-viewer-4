#pragma once

#include <string>
#include <vector>
#include <memory>

#include "ui.hpp"

class UiContext;

class MainWindow : public Ui
{
public:
	MainWindow(
		const std::shared_ptr<UiContext>& ui_context,
		const std::string name,
		const std::vector<std::pair<std::string, std::shared_ptr<Ui>>>& uis);

	virtual void update() override;

	void toggleShow() { show_ = !show_;	}

private:
	bool show_;
	std::string name_;
	std::vector<std::pair<std::string, std::shared_ptr<Ui>>> uis_;
	std::shared_ptr<UiContext> ui_context_;
};
