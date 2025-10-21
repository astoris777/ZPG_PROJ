#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);
    const char* vShaderSource = vertexCode.c_str();
    const char* fShaderSource = fragmentCode.c_str();

    std::cout << "Loading shaders: " << vertexPath << " and " << fragmentPath << std::endl;

    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderSource, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderSource, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    programID = glCreateProgram();
    glAttachShader(programID, vertex);
    glAttachShader(programID, fragment);
    glLinkProgram(programID);
    checkCompileErrors(programID, "PROGRAM");


    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader()
{
    glDeleteProgram(programID);
}

void Shader::use()
{
    glUseProgram(programID);
}

std::string Shader::loadShaderSource(const std::string& filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Could not open file" << filePath << std::endl;
        return "";
    }
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

void Shader::checkCompileErrors(GLuint shader, const std::string& type)
{
    GLint success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "FAILED TO COMPILE " << type << ": " << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "LINK ERROR " << infoLog << std::endl;
        }
    }
}

void Shader::setUniform(const std::string& name, float value)
{
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) return;
    glUniform1f(location, value);
}

void Shader::setUniform(const std::string& name, int value)
{
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) return;
    glUniform1i(location, value);
}

void Shader::setUniform(const std::string& name, const glm::vec3& vec)
{
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) {
        return;
    }
    glUniform3fv(location, 1, glm::value_ptr(vec));
}

void Shader::setUniform(const std::string& name, const glm::vec4& vec)
{
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) {
        return;
    }
    glUniform4fv(location, 1, glm::value_ptr(vec));
}

void Shader::setUniform(const std::string& name, const glm::mat4& mat)
{
    int location = glGetUniformLocation(programID, name.c_str());
    if (location == -1) return;
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}