#pragma once

#include <memory>
#include <vector>

class Node;
class Camera;
class Renderable;

class Renderer
{
public:
    Renderer();

    void draw(
        const std::shared_ptr<Node>& root,
        const std::shared_ptr<Camera> camera,
        const std::vector<std::shared_ptr<Renderable>>& renderables);
};

