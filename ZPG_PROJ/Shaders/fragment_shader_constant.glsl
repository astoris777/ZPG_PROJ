#version 330

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    int hasTexture;
    sampler2D diffuseTexture;
};

uniform Material material;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

void main() {
    vec3 color = material.diffuse;
    
    if (material.hasTexture == 1) {
        vec4 texColor = texture(material.diffuseTexture, TexCoords);
        color = texColor.rgb;
    }
    
    FragColor = vec4(color, 1.0);
}