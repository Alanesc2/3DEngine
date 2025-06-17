#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTexCoords;
layout (location = 4) in vec2 aTexCoord;

out vec3 ourColor;
out vec3 ourNormal;
out vec3 ourTexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 windDirection;
uniform float windStrength;
uniform float time;

void main() {
    // Calculate wind effect
    float windEffect = sin(time + aPos.y * 2.0) * windStrength;
    vec3 windOffset = vec3(windDirection.x, 0.0, windDirection.z) * windEffect * aPos.y;
    
    // Apply wind to vertex position
    vec3 windPos = aPos + windOffset;
    
    gl_Position = transform * vec4(windPos, 1.0);
    FragPos = vec3(model * vec4(windPos, 1.0));
    
    // Add subtle grid pattern for ground plane
    if (abs(aPos.y) < 0.01) {  // If this is a ground plane vertex
        float gridSize = 1.0;
        float gridLine = 0.02;
        float x = mod(abs(aPos.x), gridSize);
        float z = mod(abs(aPos.z), gridSize);
        if (x < gridLine || x > gridSize - gridLine || 
            z < gridLine || z > gridSize - gridLine) {
            ourColor = aColor * 1.2;  // Slightly brighter for grid lines
        } else {
            ourColor = aColor;
        }
    } else {
    ourColor = aColor;
    }
    
    Normal = mat3(transpose(inverse(model))) * aNormal;
    ourNormal = aNormal;
    ourTexCoords = aTexCoords;
    TexCoord = aTexCoord;
} 