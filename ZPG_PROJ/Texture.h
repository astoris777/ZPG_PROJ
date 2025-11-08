#pragma once
#include <GL/glew.h>
#include <string>

class Texture
{
public:
    Texture(const char* filePath);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;

    GLuint getID() const { return textureID; }

private:
    GLuint textureID;
    int width, height, channels;
};