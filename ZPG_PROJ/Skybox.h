#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include "Texture.h"
#include "Shader.h"

class Skybox
{
public:
    Skybox(const std::vector<Texture*>& faceTextures, Shader* shader);
    ~Skybox();

    void draw(const glm::mat4& view, const glm::mat4& projection);
    GLuint getTextureID() const { return textureID; }
    void setBrightness(float brightness) { this->brightness = brightness; }

private:
    GLuint VAO, VBO;
    GLuint textureID;
    Shader* shader;
    float brightness = 1.0f;

    void setupMesh();
    void loadCubemap(const std::vector<Texture*>& faceTextures);
};