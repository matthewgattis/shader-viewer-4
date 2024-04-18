#include "appcamera.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

AppCamera::AppCamera(
    const std::shared_ptr<Node>& parent,
    const glm::vec2& resolution) :
    parent_(parent),
    transform_(1.0f),
    projection_matrix_(1.0f)
{
    setResolution(resolution);

    constexpr float ninety_degrees = glm::pi<float>() / 2.0f;
    constexpr float forty_five_degrees = glm::pi<float>() / 4.0f;
    constexpr float thirty_degrees = 30.0f * glm::pi<float>() / 180.0f;
    transform_ = glm::rotate(transform_, ninety_degrees, glm::vec3(1.0f, 0.0f, 0.0f));
    transform_ = glm::rotate(transform_, -forty_five_degrees, glm::vec3(0.0f, 1.0f, 0.0));
    transform_ = glm::rotate(transform_, -thirty_degrees, glm::vec3(1.0f, 0.0f, 0.0f));
}

void AppCamera::update(double frame_delay)
{
}

void AppCamera::setResolution(const glm::vec2& resolution)
{
    const auto a = resolution / 16.0f / 2.0f;
    projection_matrix_ = glm::ortho(-a.x, a.x, -a.y, a.y, -a.y, a.y);
    half_ = resolution / 2.0f;
}

void AppCamera::handleEvents(const SDL_Event& e)
{
    constexpr float ninety_degrees = glm::pi<float>() / 2.0f;
    constexpr float forty_five_degrees = glm::pi<float>() / 4.0f;
    constexpr float thirty_degrees = 30.0f * glm::pi<float>() / 180.0f;
    transform_ = glm::rotate(glm::mat4(1.0f), ninety_degrees, glm::vec3(1.0f, 0.0f, 0.0f));
    transform_ = glm::rotate(transform_, -forty_five_degrees, glm::vec3(0.0f, 1.0f, 0.0));
    transform_ = glm::rotate(transform_, -thirty_degrees, glm::vec3(1.0f, 0.0f, 0.0f));

    switch (e.type)
    {
        case SDL_MOUSEMOTION:
            transform_ =
                transform_ *
                glm::translate(glm::mat4(1.0f), glm::vec3((-e.motion.x + half_.x) / 100.0f, (e.motion.y - half_.y) / 100.0f, 0.0f));
            break;

        default:
            break;
    }
}

