#include "camera.hpp"

#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

#define LOG_MODULE_NAME ("App")
#include "log.hpp"

constexpr glm::mat4 IDENTITY = glm::identity<glm::mat4>();
constexpr float RAD = glm::pi<float>() / 180.0;


Camera::Camera(float distance, float min_distance, float max_distance) :
	view_matrix_(IDENTITY),
	distance_(std::max(min_distance, std::min(distance, max_distance))),
	velocity_(0.0),
    acceleration_(0.0),
    default_distance_(distance)
{
	view_matrix_[3].z = distance;
}

constexpr glm::vec3 right(1.0, 0.0, 0.0);
constexpr glm::vec3 up(0.0, 1.0, 0.0);
constexpr glm::vec3 forward(0.0, 0.0, 1.0);

void Camera::rotate(float x, float y)
{
	glm::mat4 r =
		glm::rotate(IDENTITY, ROTATE_SPEED * RAD * -x, up) *
		glm::rotate(IDENTITY, ROTATE_SPEED * RAD * -y, right);

	glm::vec3 a = forward * distance_;

	glm::mat4 b = glm::translate(
		IDENTITY,
		glm::vec3(r * glm::vec4(a, 0.0)) - a);

    view_matrix_ = view_matrix_ * (b * r);
}

void Camera::rotate2(float x, float y)
{
	glm::mat4 r =
		glm::rotate(IDENTITY, ROTATE_SPEED * RAD * -x, up) *
		glm::rotate(IDENTITY, ROTATE_SPEED * RAD * -y, right);

	view_matrix_ = view_matrix_ * r;
}

void Camera::pan(float x, float y)
{
    view_matrix_ =
        view_matrix_ *
        glm::translate(
            IDENTITY,
            PAN_SPEED * distance_ * glm::vec3(-x, y, 0.0));
}

void Camera::zoom(float z)
{
	float d = ZOOM_SPEED * distance_ * -z;

	view_matrix_ = view_matrix_ * glm::translate(IDENTITY, forward * d);
    distance_ = distance_ + d;
}

void Camera::update(float frame_delay)
{
	if (frame_delay < glm::epsilon<float>())
		return;

    {
        if (glm::length(velocity_) < FRICTION * frame_delay)
            velocity_ = glm::vec3(0.0, 0.0, 0.0);
        else
            velocity_ -= FRICTION * frame_delay * glm::normalize(velocity_);

        velocity_ += frame_delay * acceleration_ * ACCELERATION;

        if (glm::length(velocity_) > MAX_VELOCITY)
            velocity_ = MAX_VELOCITY * glm::normalize(velocity_);

        view_matrix_ =
            view_matrix_ *
            glm::translate(
                IDENTITY,
                frame_delay * distance_ * velocity_);
    }
}

void Camera::setDistance(float distance)
{
    distance_ = glm::max(0.0f, distance);
}

void Camera::handleEvents(const SDL_Event& e, bool enabled)
{
    if (enabled == false)
    {
        acceleration_ = glm::vec3(0.0);
        return;
    }

    switch (e.type)
    {
        case SDL_MOUSEMOTION:
            {
                if ((e.motion.state & SDL_BUTTON_LMASK) &&
                    (e.motion.state & SDL_BUTTON_RMASK))
                {
                    zoom(-e.motion.yrel / 32.0f);
                }
                else
                {
                    if (e.motion.state & SDL_BUTTON_LMASK)
                        rotate2(e.motion.xrel, e.motion.yrel);

                    if (e.motion.state & SDL_BUTTON_RMASK)
                        rotate(e.motion.xrel, e.motion.yrel);
                }

                if (e.motion.state & SDL_BUTTON_MMASK)
                    pan(e.motion.xrel, e.motion.yrel);
            }
            break;

        case SDL_MOUSEWHEEL:
            zoom(e.wheel.preciseY);
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
                    acceleration_.z = 0.0;
                    break;

                case SDLK_a:
                    acceleration_.x = 0.0;
                    break;

                case SDLK_s:
                    acceleration_.z = 0.0;
                    break;

                case SDLK_d:
                    acceleration_.x = 0.0;
                    break;

                case SDLK_SPACE:
                    acceleration_.y = 0.0;
                    break;

                case SDLK_LSHIFT:
                    acceleration_.y = 0.0;
                    break;
            }
            break;

        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
                case SDLK_w:
                    acceleration_.z = -1.0;
                    break;

                case SDLK_a:
                    acceleration_.x = -1.0;
                    break;

                case SDLK_s:
                    acceleration_.z = 1.0;
                    break;

                case SDLK_d:
                    acceleration_.x = 1.0;
                    break;

                case SDLK_SPACE:
                    acceleration_.y = 1.0;
                    break;

                case SDLK_LSHIFT:
                    acceleration_.y = -1.0;
                    break;

                case SDLK_c:
                    {
                        LOG_INFO << "camera reset" << std::endl;
                        
                        distance_ = default_distance_;
                        view_matrix_ = IDENTITY;
                        view_matrix_[3].z = distance_;

                        acceleration_ = glm::vec3(0.0);
                        velocity_ = glm::vec3(0.0);
                    }
                    break;
            }
            break;
    }
}

