#include "sandbox.hpp"

#include <GL/glew.h>

#include "planegeometry.hpp"

#include "sandboxmaterial.hpp"
#include "geometry.hpp"
#include "program.hpp"

#define LOG_MODULE_NAME ("Sandbox")
#include "log.hpp"

Sandbox::Sandbox(const std::shared_ptr<SandboxMaterial>& sandbox_material) :
    sandbox_material_(sandbox_material)
{
    LOG_INFO << "instance created. " << this << std::endl;

    geometry_ = std::make_shared<PlaneGeometry>();
}

void Sandbox::render(const glm::mat4& view_matrix)
{
    sandbox_material_->useMaterial();
    sandbox_material_->setViewMatrixUniform(view_matrix);
    geometry_->render(sandbox_material_->getPositionAttrib());
}

