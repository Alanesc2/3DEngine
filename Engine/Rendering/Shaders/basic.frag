#version 330 core
in vec3 ourColor;
in vec3 ourNormal;
in vec3 ourTexCoords;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightDir;    // Directional light direction (normalized)
uniform vec3 viewPos;     // Camera position
uniform float time;       // Time for sun movement

void main() {
    // Calculate sun position based on time
    float sunHeight = sin(time * 0.1) * 0.5 + 0.5;
    vec3 sunDir = normalize(vec3(0.5, sunHeight, 0.5));
    
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * ourColor;

    // Diffuse
    vec3 norm = normalize(ourNormal);
    float diff = max(dot(norm, -sunDir), 0.0);
    vec3 diffuse = diff * ourColor;

    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(sunDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0, 1.0, 1.0);

    // Combine lighting components
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 