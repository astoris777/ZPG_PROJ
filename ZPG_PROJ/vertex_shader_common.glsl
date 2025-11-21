#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform float w = 1.0;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

void main() {
    vec4 worldPos = modelMatrix * vec4(aPos, w);
    FragPos = vec3(worldPos) / worldPos.w;  
    Normal = mat3(transpose(inverse(modelMatrix))) * aNormal;
    TexCoords = aTexCoords;
    
    gl_Position = projectionMatrix * viewMatrix * worldPos;
}