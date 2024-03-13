#pragma once

#include <GL/glew.h>

#include "geometry.hpp"

class Program;

class PlaneGeometry : public Geometry
{
public:
    PlaneGeometry();

    void render(GLint position_attrib_location) override;

private:
    GLuint vbo_;
    GLuint ibo_;
    GLuint vao_;
};

