#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform vec3 sunDirection;
uniform float time;

void main() {
    // Normalize the direction vector
    vec3 dir = normalize(TexCoords);
    
    // Calculate sun position based on time
    float sunHeight = sin(time * 0.1) * 0.5 + 0.5; // Sun moves up and down
    vec3 sunPos = normalize(vec3(0.5, sunHeight, 0.5));
    
    // Calculate distance to sun
    float sunDist = dot(dir, sunPos);
    
    // Sky gradient
    vec3 skyColor = mix(
        vec3(0.5, 0.7, 1.0),  // Zenith color (light blue)
        vec3(0.8, 0.9, 1.0),  // Horizon color (lighter blue)
        dir.y * 0.5 + 0.5     // Blend based on height
    );
    
    // Add sun
    float sunGlow = pow(max(0.0, sunDist), 32.0) * 2.0;
    vec3 sunColor = vec3(1.0, 0.9, 0.7) * sunGlow;
    
    // Add atmospheric scattering
    float scatter = pow(max(0.0, 1.0 - abs(dir.y)), 4.0) * 0.3;
    vec3 scatterColor = vec3(0.8, 0.9, 1.0) * scatter;
    
    // Combine all effects
    vec3 finalColor = skyColor + sunColor + scatterColor;
    
    FragColor = vec4(finalColor, 1.0);
} 