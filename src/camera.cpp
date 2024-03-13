#include "camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

constexpr glm::mat4 IDENTITY = glm::identity<glm::mat4>();
constexpr float RAD = glm::pi<float>() / 180.0;


Camera::Camera(float distance, float min_distance, float max_distance) :
	view_matrix_(IDENTITY),
	distance_(std::max(min_distance, std::min(distance, max_distance))),
	min_distance_(min_distance),
	max_distance_(max_distance),
	velocity_(0.0)
{
	view_matrix_[3].z = -distance;
}

constexpr glm::vec3 right(1.0, 0.0, 0.0);
constexpr glm::vec3 up(0.0, 1.0, 0.0);
constexpr glm::vec3 forward(0.0, 0.0, 1.0);

void Camera::rotate(float x, float y)
{
	glm::mat4 r =
		glm::rotate(IDENTITY, ROTATE_SPEED * RAD * x, up) *
		glm::rotate(IDENTITY, ROTATE_SPEED * RAD * y, right);
	glm::vec3 a = forward * distance_;
	glm::mat4 b = glm::translate(
		IDENTITY,
		glm::vec3(r * glm::vec4(a, 0.0)) - a);
	view_matrix_ = b * r * view_matrix_;
}

void Camera::rotate2(float x, float y)
{
	glm::mat4 r =
		glm::rotate(IDENTITY, ROTATE_SPEED * RAD * x, up) *
		glm::rotate(IDENTITY, ROTATE_SPEED * RAD * y, right);

	view_matrix_ = r * view_matrix_;
}

void Camera::pan(float x, float y)
{
	view_matrix_ =
		glm::translate(
			IDENTITY,
			PAN_SPEED * distance_ * glm::vec3(x, -y, 0.0)) *
		view_matrix_;
}

void Camera::zoom(float z)
{
	float d = ZOOM_SPEED * distance_ * z;
	if (distance_ - d >= max_distance_)
		d = distance_ - max_distance_;
	if (distance_ - d <= min_distance_)
		d = distance_ - min_distance_;
	view_matrix_ = glm::translate(IDENTITY, forward * d) * view_matrix_;
	distance_ = glm::max(min_distance_, glm::min(distance_ - d, max_distance_));
}

void Camera::update(float frame_delay, const glm::vec3& movement)
{
	if (frame_delay < glm::epsilon<float>())
		return;

	if (glm::length(velocity_) < FRICTION * frame_delay)
		velocity_ = glm::vec3(0.0, 0.0, 0.0);
	else
		velocity_ -= FRICTION * frame_delay * glm::normalize(velocity_);

	velocity_ += frame_delay * movement * ACCELERATION;

	if (glm::length(velocity_) > MAX_VELOCITY)
		velocity_ = MAX_VELOCITY * glm::normalize(velocity_);

	view_matrix_ =
		glm::translate(
			IDENTITY,
			frame_delay * distance_ * velocity_) *
		view_matrix_;
}

