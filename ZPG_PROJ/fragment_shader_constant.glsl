#version 330 core

in vec3 FragPos;
in vec3 Normal;

uniform vec3 objectColor;

out vec4 fragColor;

void main()
{
    fragColor = vec4(objectColor, 1.0);
}