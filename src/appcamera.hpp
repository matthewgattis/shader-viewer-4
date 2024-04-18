#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

#include "camera.hpp"

class AppCamera : public Camera
{
public:
    AppCamera(
        const std::shared_ptr<Node>& parent,
        const glm::vec2& resolution);

    void update(double frame_delay);

    void setResolution(const glm::vec2& resolution);

    void handleEvents(const SDL_Event& e);

private:
    float distance;

public:
    glm::mat4 getProjectionMatrix() const override { return projection_matrix_; }

private:
    glm::mat4 projection_matrix_;

public:
    std::shared_ptr<Node> getParent() const override { return parent_; }
    glm::mat4 getTransform() const override { return transform_; }

private:
    std::shared_ptr<Node> parent_;
    glm::mat4 transform_;

    glm::vec2 half_;
};

