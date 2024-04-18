#pragma once

#include <GL/glew.h>

#include "geometry.hpp"

class AxisGeometry : public Geometry
{
public:
    AxisGeometry();

    void draw() override;

private:
    GLuint vbo_;
    GLuint cbo_;
    GLuint ibo_;
    GLuint vao_;
};

