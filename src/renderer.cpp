#include "renderer.hpp"

#include "node.hpp"
#include "camera.hpp"
#include "renderable.hpp"

Renderer::Renderer()
{
}

void Renderer::draw(
    const std::shared_ptr<Node>& root,
    const std::shared_ptr<Camera> camera,
    const std::vector<std::shared_ptr<Renderable>>& renderables)
{
    glm::mat4 projection = camera->getProjectionMatrix();

    glm::mat4 view(1.0f);

    {
        std::shared_ptr<Node> ptr = camera;
        while (ptr->getParent().get() != nullptr)
        {
            view = ptr->getTransform() * view;
            ptr = ptr->getParent();
        }
    }

    glm::mat4 view_projection = projection * glm::inverse(view);

    for (auto& renderable : renderables)
    {
        std::shared_ptr<Node> ptr = renderable;

        glm::mat4 model(1.0f);

        while (ptr->getParent().get() != nullptr)
        {
            model = ptr->getTransform() * model;
            ptr = ptr->getParent();
        }

        renderable->draw(view_projection * model);
    }
}

