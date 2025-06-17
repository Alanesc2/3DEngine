#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main() {
    // Add gentle movement to clouds
    vec3 cloudPos = aPos;
    cloudPos.x += sin(time * 0.1 + aPos.y) * 0.1;
    cloudPos.z += cos(time * 0.1 + aPos.x) * 0.1;
    
    FragPos = vec3(model * vec4(cloudPos, 1.0));
    TexCoord = aTexCoord;
    gl_Position = projection * view * model * vec4(cloudPos, 1.0);
} 