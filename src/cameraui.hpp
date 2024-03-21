#pragma once

#include <memory>

#include "ui.hpp"

class Camera;

class CameraUi : public Ui
{
public:
	CameraUi(const std::shared_ptr<Camera>& camera);

	void update() override;

private:
	std::shared_ptr<Camera> camera_;
};
