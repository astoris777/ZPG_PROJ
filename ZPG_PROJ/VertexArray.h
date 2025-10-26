#pragma once
#include <GL/glew.h>

class VertexArray
{
public:
    enum Format {
        POSITION_COLOR,      
        POSITION_COLOR_NORMAL, 
        POSITION_NORMAL 
    };

    VertexArray(const float* vertices, GLsizei count, Format format = POSITION_COLOR_NORMAL);
    ~VertexArray();

    void bind() const;
    void unbind() const;
    void draw() const;

private:
    GLuint VAO, VBO;
    GLsizei vertexCount;
};