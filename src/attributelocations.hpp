#pragma once

#include <optional>

#include <GL/glew.h>

struct AttributeLocations
{
    AttributeLocations() = default;
    AttributeLocations(
        const std::optional<GLint>& position_attrib_location,
        const std::optional<GLint>& normal_attrib_location,
        const std::optional<GLint>& color_attrib_location,
        const std::optional<GLint>& texcoord_attrib_location) :
        position_attrib_location(position_attrib_location),
        normal_attrib_location(normal_attrib_location),
        color_attrib_location(color_attrib_location),
        texcoord_attrib_location(texcoord_attrib_location)
    {
    }

    std::optional<GLint> position_attrib_location;
    std::optional<GLint> normal_attrib_location;
    std::optional<GLint> color_attrib_location;
    std::optional<GLint> texcoord_attrib_location;
};

