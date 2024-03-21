#include "cameraui.hpp"

#include <sstream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui.h"

#include "camera.hpp"

CameraUi::CameraUi(const std::shared_ptr<Camera>& camera) :
	camera_(camera)
{
}

void CameraUi::update()
{
	auto& view_matrix = camera_->view_matrix_;

	{
		float position[3];
		std::memcpy(position, &glm::value_ptr(view_matrix)[12], sizeof(position));

		ImGui::DragFloat3("position", position, 0.0f);
	}

	{
		float rotation[3];
		rotation[0] = glm::degrees(glm::atan(view_matrix[1][2], view_matrix[2][2]));
		rotation[1] = glm::degrees(-glm::asin(view_matrix[0][2]));
		rotation[2] = glm::degrees(glm::atan(view_matrix[0][1], view_matrix[0][0]));

		float temp[3];
		std::memcpy(temp, rotation, sizeof(rotation));

		ImGui::DragFloat3("rotation", rotation, 0.0f);
	}

	{
		float distance = camera_->getDistance();

		ImGui::DragFloat("pivot distance", &distance, 0.01f);

        camera_->setDistance(distance);
	}

	if (ImGui::TreeNode("matrix"))
	{
		std::stringstream ss;

		for (int i = 0; i < 16; i++)
		{
			ss << glm::value_ptr(view_matrix)[i];

			if (i < 15)
				ss << ",";

			if (i % 4 == 3)
				ss << std::endl;
			else
				ss << " ";
		}

		char buffer[1024];
		std::strcpy(buffer, ss.str().c_str());
		ImGui::InputTextMultiline("view", buffer, 1024);

        ImGui::TreePop();
	}

}
