#pragma once

#include <glm/glm.hpp>
#include <memory>

#include <SDL2/SDL.h>

class Camera
{
public:
	Camera(float distance, float min_distance, float max_distance);

	void rotate(float x, float y);
	void rotate2(float x, float y);
	void pan(float x, float y);
	void zoom(float z);

	void update(float frame_delay);
    void handleEvents(const SDL_Event& e, bool enabled);

    float getDistance() const { return distance_; }
    void setDistance(float distance);

public:
	glm::mat4 view_matrix_;

private:
	float distance_;
    float default_distance_;

    constexpr static float ROTATE_SPEED = 1.0 / 8.0;
    constexpr static float PAN_SPEED = 1.0 / 1024.0;
	constexpr static float ZOOM_SPEED = 1.0 / 8.0;

	glm::vec3 velocity_;
    glm::vec3 acceleration_;

	constexpr static float MAX_VELOCITY = 1.0;
	constexpr static float ACCELERATION = 16.0;
	constexpr static float FRICTION = 8.0;
};
