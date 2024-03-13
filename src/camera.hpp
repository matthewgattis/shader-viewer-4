#pragma once

#include <glm/glm.hpp>
#include <memory>

class Camera
{
public:
	Camera(float distance, float min_distance, float max_distance);

	void rotate(float x, float y);
	void rotate2(float x, float y);

	void pan(float x, float y);

	void zoom(float z);

	void update(float frame_delay, const glm::vec3& movement);

	glm::mat4& get() { return view_matrix_; }

private:
	glm::mat4 view_matrix_;
	float distance_;
	float min_distance_;
	float max_distance_;

	glm::vec3 velocity_;

    constexpr static float ROTATE_SPEED = 1.0 / 8.0;
    constexpr static float PAN_SPEED = 1.0 / 1024.0;
	constexpr static float ZOOM_SPEED = 1.0 / 8.0;

	constexpr static float MAX_VELOCITY = 1.0;
	constexpr static float ACCELERATION = 16.0;
	constexpr static float FRICTION = 8.0;
};
