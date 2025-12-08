#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"

class Skybox
{
public:
    Skybox(const std::vector<Texture*>& faceTextures);
    ~Skybox();

    void draw(const glm::mat4& view, const glm::mat4& projection);
    GLuint getTextureID() const { return textureID; }

private:
    GLuint VAO, VBO;
    GLuint textureID;
    GLuint shaderProgram;

    void setupMesh();
    void loadCubemap(const std::vector<Texture*>& faceTextures);
    GLuint compileShader(GLenum type, const char* source);
    GLuint createShaderProgram();
};