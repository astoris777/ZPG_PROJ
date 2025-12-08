#pragma once
#include <GL/glew.h>

class Texture
{
public:
    Texture(const char* filePath);
    Texture(const char* filePath, bool flipVertically);
    ~Texture();

    void bind(unsigned int slot = 0) const;
    void unbind() const;
    GLuint getTextureID() const { return textureID; }

private:
    GLuint textureID;
    int width, height, channels;
};