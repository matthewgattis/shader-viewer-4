#pragma once

#include <GL/glew.h>

#include "geometry.hpp"

class Program;
struct AttributeLocations;

class PlaneGeometry : public Geometry
{
public:
    PlaneGeometry();

    void draw(const AttributeLocations& attribute_locations) override;

private:
    GLuint vbo_;
    GLuint ibo_;
    GLuint vao_;
};

