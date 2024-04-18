#include "axisgeometry.hpp"

#include "attributelocations.hpp"

#define LOG_MODULE_NAME ("AxisGeometry")
#include "log.hpp"

AxisGeometry::AxisGeometry()
{
    LOG_INFO << "instance created. " << this << std::endl;

    GLfloat vertices[]
    {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
    };

    GLfloat colors[]
    {
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
    };

    GLuint indices[]
    {
        0, 1,
        2, 3,
        4, 5
    };

    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    constexpr int VERTEX_COUNT = 6;

    glGenBuffers(1, &vbo_);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * 3 * sizeof (GLfloat), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &cbo_);
    glBindBuffer(GL_ARRAY_BUFFER, cbo_);
    glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * 3 * sizeof (GLfloat), colors, GL_STATIC_DRAW);

    constexpr int INDEX_COUNT = 6;

    glGenBuffers(1, &ibo_);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, INDEX_COUNT * sizeof (GLuint), indices, GL_STATIC_DRAW);
}

void AxisGeometry::draw()
{
    glBindVertexArray(vao_);

    constexpr GLint POSITION_ATTRIBUTE = 0;
    constexpr GLint COLOR_ATTRIBUTE = 1;

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glVertexAttribPointer(POSITION_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(POSITION_ATTRIBUTE);

    glBindBuffer(GL_ARRAY_BUFFER, cbo_);
    glVertexAttribPointer(COLOR_ATTRIBUTE, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(COLOR_ATTRIBUTE);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, nullptr);
}

