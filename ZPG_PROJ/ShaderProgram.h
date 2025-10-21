#pragma once
#include "ShaderObserver.h"
#include "Light.h"

class ShaderProgram : public Shader, public ShaderObserver {
public:
    ShaderProgram(const char* vertexPath, const char* fragmentPath)
        : Shader(vertexPath, fragmentPath) {
    }

    void update(const glm::mat4& view, const glm::mat4& projection) override {
        use();
        setUniform("viewMatrix", view);
        setUniform("projectionMatrix", projection);
    }

    void setLight(const Light& light, const std::string& prefix = "light") {
        use();
        light.setUniforms(this, prefix);
    }
};