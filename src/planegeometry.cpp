#include "planegeometry.hpp"

#include "attributelocations.hpp"

#define LOG_MODULE_NAME ("PlaneGeometry")
#include "log.hpp"

PlaneGeometry::PlaneGeometry()
{
    LOG_INFO << "instance created. " << this << std::endl;

    GLfloat vertices[]
    {
        -1.0f, -1.0f,
         1.0f, -1.0f,
        -1.0f,  1.0f,
         1.0f,  1.0f
    };

    GLuint indices[]
    {
        0, 1, 2,
        1, 3, 2
    };

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof (GLfloat), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &ibo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof (GLuint), indices, GL_STATIC_DRAW);
}

void PlaneGeometry::draw(const AttributeLocations& attribute_locations)
{
    glBindVertexArray(vao_);

    if (attribute_locations.position_attrib_location.has_value())
    {
        GLint position_attrib_location = attribute_locations.position_attrib_location.value();
        glEnableVertexAttribArray(position_attrib_location);
        glBindBuffer(GL_ARRAY_BUFFER, vbo_);
        glVertexAttribPointer(position_attrib_location, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

