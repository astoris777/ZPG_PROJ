#version 330 core

in vec3 FragPos;
in vec3 Normal;

#define MAX_LIGHTS 4

struct Light {
    vec4 position;
    vec4 diffuse;
    vec4 specular;
    vec4 direction;    // ??? spot/directional light
    float intensity;
    float constant;    // ????????? ?????????
    float linear;      // ???????? ?????????
    float quadratic;   // ???????????? ?????????
    float cutOff;      // ??? spot light
    float outerCutOff; // ??? spot light
    int type;          // 0=point, 1=spot, 2=directional
};

uniform Light lights[MAX_LIGHTS];
uniform int numberOfLights;
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform vec3 ambientLight; // Ambient light

out vec4 fragColor;

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    
    vec3 ambient = ambientLight; // ?????????? ????????????? ambient
    vec3 totalDiffuse = vec3(0.0);
    vec3 totalSpecular = vec3(0.0);
    
    for (int i = 0; i < numberOfLights && i < MAX_LIGHTS; ++i) {
        vec3 lightPos = lights[i].position.xyz;
        vec3 lightColor = lights[i].diffuse.xyz;
        vec3 specularColor = lights[i].specular.xyz;
        float lightIntensity = lights[i].intensity;
        int lightType = lights[i].type;
        
        vec3 lightDir;
        float attenuation = 1.0;
        
        if (lightType == 2) { // Directional light
            lightDir = normalize(-lights[i].direction.xyz);
            // Directional light ?? ????? ?????????
            attenuation = 1.0;
        } else { // Point light ??? Spot light
            lightDir = normalize(lightPos - FragPos);
            
            // ???????????? attenuation (?????????) ?? ??????????
            float distance = length(lightPos - FragPos);
            attenuation = 1.0 / (lights[i].constant + 
                                lights[i].linear * distance + 
                                lights[i].quadratic * (distance * distance));
        }
        
        // Spot light ????????
        float spotEffect = 1.0;
        if (lightType == 1) { // Spot light
            vec3 spotDir = normalize(lights[i].direction.xyz);
            float theta = dot(lightDir, normalize(-spotDir));
            float epsilon = lights[i].cutOff - lights[i].outerCutOff;
            spotEffect = clamp((theta - lights[i].outerCutOff) / epsilon, 0.0, 1.0);
        }
        
        // ????????? ?????????
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor * lightIntensity * attenuation * spotEffect;
        totalDiffuse += diffuse;
        
        // ??????????? ????????? (Blinn-Phong)
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);
        vec3 specular = spec * specularColor * attenuation * spotEffect;
        totalSpecular += specular;
    }
    
    vec3 result = (ambient + totalDiffuse + totalSpecular) * objectColor;
    fragColor = vec4(result, 1.0);
}