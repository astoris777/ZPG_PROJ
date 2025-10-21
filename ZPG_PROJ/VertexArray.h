#pragma once
#include <GL/glew.h>

class VertexArray
{
public:
    enum Format {
        POSITION_COLOR,      //  x,y,z, r,g,b
        POSITION_COLOR_NORMAL, //  x,y,z, r,g,b, nx,ny,nz
        POSITION_NORMAL // x, y, z, nx, ny, nz
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