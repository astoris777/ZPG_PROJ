#pragma once
#pragma once
#include <string>
#include <glm/glm.hpp>
#include <GL/glew.h>

class Shader
{
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();
    void use();
    void setUniform(const std::string& name, float value);
    void setUniform(const std::string& name, int value);
    void setUniform(const std::string& name, const glm::vec3& vec);
    void setUniform(const std::string& name, const glm::vec4& vec);
    void setUniform(const std::string& name, const glm::mat4& mat);

private:
    GLuint programID;
    void checkCompileErrors(GLuint shader, const std::string& type);
    std::string loadShaderSource(const std::string& filePath);
};