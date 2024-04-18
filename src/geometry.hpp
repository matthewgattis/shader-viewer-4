#pragma once

#include <GL/glew.h>

class Program;
struct AttributeLocations;

class Geometry
{
public:
    virtual void draw() = 0;
};

