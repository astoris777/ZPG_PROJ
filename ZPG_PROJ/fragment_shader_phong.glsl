#version 330

#define MAX_LIGHTS 6

struct Light {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float constantFactor;
    float linearFactor;
    float quadraticFactor;
    float cutoff;
    float outerCutoff;
    int type;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    int hasTexture;
    sampler2D diffuseTexture;
};

uniform Light lights[MAX_LIGHTS];
uniform int numberOfLights;
uniform Material material;
uniform vec3 viewPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

float calculateAttenuation(float distance, float constant, float linear, float quadratic) {
    return 1.0 / (constant + linear * distance + quadratic * (distance * distance));
}

vec3 calculateAmbient(vec3 lightColor, vec3 materialAmbient) {
    return materialAmbient * lightColor;
}

vec3 calculateDiffuse(vec3 lightColor, vec3 lightDir, vec3 normal, float attenuation, vec3 materialDiffuse) {
    float diff = max(dot(normal, lightDir), 0.0);
    return diff * lightColor * materialDiffuse * attenuation;
}

vec3 calculateSpecular(vec3 lightColor, vec3 lightDir, vec3 viewDir, vec3 normal, float attenuation) {
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    return spec * lightColor * material.specular * attenuation;
}

vec3 calculateLightContribution(Light light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 materialDiffuse) {
    vec3 lightDir;
    float attenuation = 1.0;
    vec3 lightColor = light.color * light.intensity;

    // 0 = POINT, 1 = SPOT, 2 = DIRECTIONAL, 3 = AMBIENT  (????????????? LightType ? C++)
    if (light.type == 0) {
        // POINT
        lightDir = normalize(light.position - fragPos);
        float distance = length(light.position - fragPos);
        attenuation = calculateAttenuation(distance, light.constantFactor, light.linearFactor, light.quadraticFactor);
        
        vec3 diffuse = calculateDiffuse(lightColor, lightDir, normal, attenuation, materialDiffuse);
        vec3 specular = calculateSpecular(lightColor, lightDir, viewDir, normal, attenuation);
        return diffuse + specular;
    }
    else if (light.type == 1) {
        // SPOT
        lightDir = normalize(light.position - fragPos);
        float distance = length(light.position - fragPos);
        attenuation = calculateAttenuation(distance, light.constantFactor, light.linearFactor, light.quadraticFactor);

        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = light.cutoff - light.outerCutoff;
        float spotEffect = clamp((theta - light.outerCutoff) / epsilon, 0.0, 1.0);
        attenuation *= spotEffect;
        
        vec3 diffuse = calculateDiffuse(lightColor, lightDir, normal, attenuation, materialDiffuse);
        vec3 specular = calculateSpecular(lightColor, lightDir, viewDir, normal, attenuation);
        return diffuse + specular;
    }
    else if (light.type == 2) {
        // DIRECTIONAL
        lightDir = normalize(-light.direction);
        
        vec3 diffuse = calculateDiffuse(lightColor, lightDir, normal, 1.0, materialDiffuse);
        vec3 specular = calculateSpecular(lightColor, lightDir, viewDir, normal, 1.0);
        return diffuse + specular;
    }
    else if (light.type == 3) {
        // AMBIENT
        return calculateAmbient(lightColor, material.ambient);
    }

    return vec3(0.0);
}

void main() {
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 materialDiffuse = material.diffuse;
    
    if (material.hasTexture == 1) {
        vec4 texColor = texture(material.diffuseTexture, TexCoords);
        materialDiffuse *= texColor.rgb;
    }
    
    vec3 result = vec3(0.0);

    for (int i = 0; i < numberOfLights && i < MAX_LIGHTS; i++) {
        result += calculateLightContribution(lights[i], normal, FragPos, viewDir, materialDiffuse);
    }

    FragColor = vec4(result, 1.0);
}