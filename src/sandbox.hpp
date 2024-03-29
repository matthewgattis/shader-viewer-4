#pragma once

#include <memory>

#include "renderable.hpp"


class Geometry;
class SandboxMaterial;

class Sandbox : public Renderable
{
public:
    Sandbox(const std::shared_ptr<SandboxMaterial>& sandbox_material);
    Sandbox() = delete;

    void render() override;

private:
    std::shared_ptr<Geometry> geometry_;
    std::shared_ptr<SandboxMaterial> sandbox_material_;
};

