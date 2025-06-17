#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;

uniform float time;
uniform vec3 sunDirection;

// Cloud noise functions
float random(vec2 st) {
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

float noise(vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    vec2 u = f * f * (3.0 - 2.0 * f);
    return mix(a, b, u.x) + (c - a)* u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

void main() {
    // Create cloud pattern
    vec2 st = TexCoord * 4.0;
    float n = noise(st);
    n += noise(st * 2.0) * 0.5;
    n += noise(st * 4.0) * 0.25;
    
    // Add some movement
    st += time * 0.1;
    float movement = noise(st) * 0.1;
    n += movement;
    
    // Create cloud shape
    float cloud = smoothstep(0.4, 0.6, n);
    
    // Add lighting based on sun direction
    float sunEffect = dot(normalize(FragPos), sunDirection);
    sunEffect = smoothstep(-0.5, 0.5, sunEffect);
    
    // Final cloud color
    vec3 cloudColor = mix(vec3(0.8, 0.8, 0.9), vec3(1.0), sunEffect);
    FragColor = vec4(cloudColor, cloud * 0.8);
} 