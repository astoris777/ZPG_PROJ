#version 330 core

in vec3 FragPos; 
in vec3 Normal;  

#define MAX_LIGHTS 4

struct Light {
    vec4 position;
    vec4 diffuse;
    float intensity;
};

uniform Light lights[MAX_LIGHTS];
uniform int numberOfLights;
uniform vec3 objectColor;   

out vec4 FragColor;

void main() {
    vec3 norm = normalize(Normal);
    vec3 ambient = vec3(0.1);
    vec3 totalDiffuse = vec3(0.0);
    
    for (int i = 0; i < numberOfLights && i < MAX_LIGHTS; ++i) {
        vec3 lightPos = lights[i].position.xyz;
        vec3 lightColor = lights[i].diffuse.xyz;
        float lightIntensity = lights[i].intensity;
        
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        totalDiffuse += diff * lightColor * lightIntensity;
    }
    
    vec3 result = (ambient + totalDiffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}