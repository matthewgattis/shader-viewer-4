#pragma once

#include <optional>

class Geometry
{
public:
    virtual void render(GLint postion_attrib_location) = 0;
};

