#define GL_SILENCE_DEPRECATION
#include "Engine/Rendering/Window.h"
#include "Engine/Rendering/Shader.h"
#include "Engine/Mesh.h"
#include "Engine/Camera.h"
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <vector>
#include <random>

// Camera settings
Camera camera;
float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;

// Timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Mouse callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.yaw += xoffset;
    camera.pitch += yoffset;

    // Constrain pitch
    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;

    camera.rotate(camera.yaw, camera.pitch);
}

// Process keyboard input
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = camera.movementSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveForward(cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveForward(-cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveRight(-cameraSpeed);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRight(cameraSpeed);
}

// Function to create a more realistic grass blade
std::vector<Vertex> createGrassBladeVertices() {
    std::vector<Vertex> vertices;
    
    // Base color for grass
    Vector3 baseColor(0.0f, 0.8f, 0.0f);
    
    // Create a curved blade shape with multiple segments
    const int segments = 4;
    const float width = 0.1f;
    const float height = 1.0f;
    
    for (int i = 0; i < segments; i++) {
        float t = static_cast<float>(i) / segments;
        float nextT = static_cast<float>(i + 1) / segments;
        
        // Calculate curved position
        float x1 = width * (1.0f - t);
        float x2 = width * (1.0f - nextT);
        float y1 = height * t;
        float y2 = height * nextT;
        
        // Add slight curve to the blade
        float curve = sin(t * M_PI) * 0.1f;
        float nextCurve = sin(nextT * M_PI) * 0.1f;
        
        // Calculate color variation (darker at bottom, lighter at top)
        Vector3 color1 = baseColor * (0.7f + t * 0.3f);
        Vector3 color2 = baseColor * (0.7f + nextT * 0.3f);
        
        // Add vertices for this segment
        vertices.push_back({{-x1, y1, curve}, color1, {0.0f, 1.0f, 0.0f}, {0.0f, t, 0.0f}});
        vertices.push_back({{ x1, y1, curve}, color1, {0.0f, 1.0f, 0.0f}, {1.0f, t, 0.0f}});
        vertices.push_back({{ x2, y2, nextCurve}, color2, {0.0f, 1.0f, 0.0f}, {1.0f, nextT, 0.0f}});
        vertices.push_back({{-x2, y2, nextCurve}, color2, {0.0f, 1.0f, 0.0f}, {0.0f, nextT, 0.0f}});
    }
    
    return vertices;
}

std::vector<unsigned int> createGrassBladeIndices() {
    std::vector<unsigned int> indices;
    const int segments = 4;
    
    // Create indices for each segment
    for (int i = 0; i < segments; i++) {
        unsigned int baseIndex = i * 4;
        // First triangle
        indices.push_back(baseIndex);
        indices.push_back(baseIndex + 1);
        indices.push_back(baseIndex + 2);
        // Second triangle
        indices.push_back(baseIndex);
        indices.push_back(baseIndex + 2);
        indices.push_back(baseIndex + 3);
    }
    
    return indices;
}

// Simple Perlin noise implementation
float noise(int x, int y) {
    int n = x + y * 57;
    n = (n << 13) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
}

float smoothNoise(float x, float y) {
    float corners = (noise(x-1, y-1) + noise(x+1, y-1) + noise(x-1, y+1) + noise(x+1, y+1)) / 16;
    float sides = (noise(x-1, y) + noise(x+1, y) + noise(x, y-1) + noise(x, y+1)) / 8;
    float center = noise(x, y) / 4;
    return corners + sides + center;
}

float interpolate(float a, float b, float x) {
    float ft = x * 3.1415927f;
    float f = (1 - cos(ft)) * 0.5f;
    return a * (1 - f) + b * f;
}

float interpolatedNoise(float x, float y) {
    int integer_X = int(x);
    float fractional_X = x - integer_X;
    int integer_Y = int(y);
    float fractional_Y = y - integer_Y;

    float v1 = smoothNoise(integer_X, integer_Y);
    float v2 = smoothNoise(integer_X + 1, integer_Y);
    float v3 = smoothNoise(integer_X, integer_Y + 1);
    float v4 = smoothNoise(integer_X + 1, integer_Y + 1);

    float i1 = interpolate(v1, v2, fractional_X);
    float i2 = interpolate(v3, v4, fractional_X);

    return interpolate(i1, i2, fractional_Y);
}

float perlinNoise(float x, float y, float persistence, int octaves) {
    float total = 0;
    float frequency = 1;
    float amplitude = 1;
    float maxValue = 0;

    for(int i = 0; i < octaves; i++) {
        total += interpolatedNoise(x * frequency, y * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }

    return total / maxValue;
}

// Function to create a terrain mesh with hills
Mesh createTerrain(float size, int resolution, float heightScale) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    float step = size / (resolution - 1);
    float halfSize = size / 2.0f;
    
    // Generate vertices with height variation
    for (int z = 0; z < resolution; z++) {
        for (int x = 0; x < resolution; x++) {
            float xPos = x * step - halfSize;
            float zPos = z * step - halfSize;
            
            // Generate height using Perlin noise
            float height = perlinNoise(xPos * 0.1f, zPos * 0.1f, 0.5f, 4) * heightScale;
            
            // Calculate normal (simplified)
            Vector3 normal(0.0f, 1.0f, 0.0f);
            
            // Base color with slight variation based on height
            Vector3 color(0.2f, 0.6f + height * 0.1f, 0.2f);
            
            vertices.push_back({{xPos, height, zPos}, color, normal, {xPos/size + 0.5f, zPos/size + 0.5f, 0.0f}});
        }
    }
    
    // Generate indices for triangles
    for (int z = 0; z < resolution - 1; z++) {
        for (int x = 0; x < resolution - 1; x++) {
            int topLeft = z * resolution + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * resolution + x;
            int bottomRight = bottomLeft + 1;
            
            // First triangle
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            
            // Second triangle
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
    
    return Mesh(vertices, indices);
}

// Function to create skybox vertices
std::vector<float> createSkyboxVertices() {
    float size = 1000.0f; // Make skybox much larger
    return {
        // positions          
        -size,  size, -size,
        -size, -size, -size,
         size, -size, -size,
         size, -size, -size,
         size,  size, -size,
        -size,  size, -size,

        -size, -size,  size,
        -size, -size, -size,
        -size,  size, -size,
        -size,  size, -size,
        -size,  size,  size,
        -size, -size,  size,

         size, -size, -size,
         size, -size,  size,
         size,  size,  size,
         size,  size,  size,
         size,  size, -size,
         size, -size, -size,

        -size, -size,  size,
        -size,  size,  size,
         size,  size,  size,
         size,  size,  size,
         size, -size,  size,
        -size, -size,  size,

        -size,  size, -size,
         size,  size, -size,
         size,  size,  size,
         size,  size,  size,
        -size,  size,  size,
        -size,  size, -size,

        -size, -size, -size,
        -size, -size,  size,
         size, -size, -size,
         size, -size, -size,
        -size, -size,  size,
         size, -size,  size
    };
}

// Wind parameters
struct Wind {
    float strength = 0.3f;
    float frequency = 0.4f;  // Slower frequency for more natural wind
    Vector3 direction = Vector3(1.0f, 0.0f, 0.5f).normalize();
    
    // Very subtle turbulence
    float turbulence = 0.02f;
    float turbulenceFrequency = 0.2f;
} wind;

// Sun parameters
struct Sun {
    Vector3 direction = Vector3(0.5f, 0.5f, 0.5f).normalize();
} sun;

// Cloud parameters
struct Cloud {
    float height = 100.0f;
    float size = 50.0f;
    int count = 20;
} cloud;

// Plant types
struct Plant {
    float height;
    Vector3 color;
    float scale;
};

// Create a variety of small plants
std::vector<Plant> plants = {
    {0.3f, Vector3(0.2f, 0.8f, 0.2f), 0.5f},  // Small bush
    {0.4f, Vector3(0.8f, 0.2f, 0.2f), 0.3f},  // Red flower
    {0.5f, Vector3(0.2f, 0.2f, 0.8f), 0.4f},  // Blue flower
    {0.6f, Vector3(0.8f, 0.8f, 0.2f), 0.6f}   // Yellow flower
};

int main() {
    Window window(800, 600, "Grass Field");
    if (!window.init()) {
        std::cerr << "Failed to initialize window" << std::endl;
        return -1;
    }

    // Set up mouse callback
    glfwSetCursorPosCallback(window.getWindow(), mouse_callback);
    glfwSetInputMode(window.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Enable depth testing and blending
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Create and compile shaders
    Shader shader;
    if (!shader.loadFromFiles("Engine/Rendering/Shaders/basic.vert", "Engine/Rendering/Shaders/basic.frag")) {
        std::cerr << "Failed to load shaders" << std::endl;
        return -1;
    }

    Shader skyboxShader;
    if (!skyboxShader.loadFromFiles("Engine/Rendering/Shaders/skybox.vert", "Engine/Rendering/Shaders/skybox.frag")) {
        std::cerr << "Failed to load skybox shaders" << std::endl;
        return -1;
    }

    // Create terrain with hills
    auto ground = createTerrain(100.0f, 100, 5.0f);

    // Create improved grass blade
    Mesh grassBlade(createGrassBladeVertices(), createGrassBladeIndices());

    // Create skybox
    std::vector<float> skyboxVertices = createSkyboxVertices();
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, skyboxVertices.size() * sizeof(float), &skyboxVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // Set up instanced grass positions with more natural distribution
    const int GRASS_COUNT = 10000;
    std::vector<glm::mat4> grassPositions;
    std::vector<float> grassHeights;
    std::vector<float> grassIntensities; // Store intensity variations
    
    // Create a more uniform distribution using a grid with random offsets
    const float GRID_SIZE = 80.0f;
    const int GRID_RESOLUTION = 40;
    const float CELL_SIZE = GRID_SIZE / GRID_RESOLUTION;
    
    for (int i = 0; i < GRASS_COUNT; i++) {
        // Generate position in grid
        int gridX = i % GRID_RESOLUTION;
        int gridZ = i / GRID_RESOLUTION;
        
        // Add random offset within cell
        float offsetX = (rand() % 100) * 0.01f * CELL_SIZE;
        float offsetZ = (rand() % 100) * 0.01f * CELL_SIZE;
        
        // Calculate final position
        float x = (gridX * CELL_SIZE - GRID_SIZE/2) + offsetX;
        float z = (gridZ * CELL_SIZE - GRID_SIZE/2) + offsetZ;
        
        // Skip if outside terrain bounds
        if (abs(x) > 40.0f || abs(z) > 40.0f) continue;
        
        // Calculate height at this position using Perlin noise
        float height = perlinNoise(x * 0.1f, z * 0.1f, 0.5f, 4) * 5.0f;
        
        // Add some height variation
        float bladeHeight = 0.8f + (rand() % 40) * 0.01f;
        grassHeights.push_back(bladeHeight);
        
        // Add slight intensity variation (0.8 to 1.2)
        float intensity = 0.8f + (rand() % 40) * 0.01f;
        grassIntensities.push_back(intensity);
        
        // Random rotation with slight forward tilt
        float rotation = rand() % 360;
        float tilt = (rand() % 20) - 10;
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x, height, z));
        model = glm::rotate(model, glm::radians(static_cast<float>(rotation)), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(static_cast<float>(tilt)), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, bladeHeight, 1.0f));
        grassPositions.push_back(model);
    }

    // Set initial camera position higher up to see more of the scene
    camera.position = Vector3(0.0f, 20.0f, 20.0f);
    camera.rotate(45.0f, -30.0f); // Look down at the scene

    // Create cloud mesh
    auto cloudMesh = Mesh::createPlane(cloud.size, Vector3(1.0f, 1.0f, 1.0f));
    std::vector<glm::mat4> cloudPositions;
    
    // Position clouds in the sky
    for (int i = 0; i < cloud.count; i++) {
        float x = (rand() % 200 - 100) * 0.5f;
        float z = (rand() % 200 - 100) * 0.5f;
        float y = cloud.height + (rand() % 20 - 10);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x, y, z));
        model = glm::rotate(model, glm::radians(static_cast<float>(rand() % 360)), glm::vec3(0.0f, 1.0f, 0.0f));
        cloudPositions.push_back(model);
    }

    // Create small plants
    std::vector<glm::mat4> plantPositions;
    const int PLANT_COUNT = 200;
    
    for (int i = 0; i < PLANT_COUNT; i++) {
        float x = (rand() % 160 - 80) * 0.5f;
        float z = (rand() % 160 - 80) * 0.5f;
        float height = perlinNoise(x * 0.1f, z * 0.1f, 0.5f, 4) * 5.0f;
        
        // Skip if too close to other plants
        bool tooClose = false;
        for (const auto& pos : plantPositions) {
            glm::vec3 existingPos = glm::vec3(pos[3]);
            if (glm::distance(glm::vec3(x, height, z), existingPos) < 2.0f) {
                tooClose = true;
                break;
            }
        }
        if (tooClose) continue;
        
        const Plant& plant = plants[rand() % plants.size()];
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x, height, z));
        model = glm::rotate(model, glm::radians(static_cast<float>(rand() % 360)), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(plant.scale, plant.height, plant.scale));
        plantPositions.push_back(model);
    }

    // Load cloud shader
    Shader cloudShader;
    if (!cloudShader.loadFromFiles("Engine/Rendering/Shaders/cloud.vert", "Engine/Rendering/Shaders/cloud.frag")) {
        std::cerr << "Failed to load cloud shaders" << std::endl;
        return -1;
    }

    while (!window.shouldClose()) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Update wind direction with more natural variation
        float windTime = currentFrame * 0.05f;  // Slower wind direction changes
        wind.direction.x = sin(windTime) * 0.5f + 0.5f;
        wind.direction.z = cos(windTime) * 0.5f + 0.5f;
        wind.direction = wind.direction.normalize();

        // Calculate base wind effect for all grass
        // Use a smoother curve for wind effect
        float baseWindEffect = (sin(currentFrame * wind.frequency) + 1.0f) * 0.5f * wind.strength;
        
        // Add very subtle turbulence
        float turbulence = sin(currentFrame * wind.turbulenceFrequency) * wind.turbulence;
        float currentWindStrength = wind.strength * (1.0f + turbulence);

        processInput(window.getWindow());

        float terrainHeight = perlinNoise(camera.position.x * 0.1f, camera.position.z * 0.1f, 0.5f, 4) * 5.0f;
        if (camera.position.y < terrainHeight + 1.0f) {
            camera.position.y = terrainHeight + 1.0f;
        }

        window.clear();

        // Create view and projection matrices
        glm::mat4 view = glm::lookAt(
            glm::vec3(camera.position.x, camera.position.y, camera.position.z),
            glm::vec3(camera.position.x + camera.direction.x,
                     camera.position.y + camera.direction.y,
                     camera.position.z + camera.direction.z),
            glm::vec3(camera.up.x, camera.up.y, camera.up.z)
        );

        glm::mat4 projection = glm::perspective(
            glm::radians(camera.fov),
            800.0f / 600.0f,
            camera.nearPlane,
            camera.farPlane
        );

        // Draw skybox first
        glDepthFunc(GL_LEQUAL);
        skyboxShader.use();
        skyboxShader.setMat4("view", glm::value_ptr(view));
        skyboxShader.setMat4("projection", glm::value_ptr(projection));
        skyboxShader.setFloat("time", currentFrame);
        glBindVertexArray(skyboxVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthFunc(GL_LESS);

        // Use main shader for terrain and grass
        shader.use();
        shader.setMat4("projection", glm::value_ptr(projection));
        shader.setMat4("view", glm::value_ptr(view));
        shader.setVec3("viewPos", camera.position.x, camera.position.y, camera.position.z);
        shader.setFloat("time", currentFrame);

        // Draw ground
        glm::mat4 groundModel = glm::mat4(1.0f);
        shader.setMat4("model", glm::value_ptr(groundModel));
        glm::mat4 groundTransform = projection * view * groundModel;
        shader.setMat4("transform", glm::value_ptr(groundTransform));
        ground.draw();

        // Draw grass blades with unified wind animation
        for (size_t i = 0; i < grassPositions.size(); i++) {
            const auto& baseModel = grassPositions[i];
            float height = grassHeights[i];
            float intensity = grassIntensities[i];
            
            // Apply the same base wind effect to all blades, with slight intensity variation
            float windEffect = baseWindEffect * intensity;
            
            // Scale effect by height and add a slight forward lean
            windEffect *= height;
            
            glm::mat4 model = baseModel;
            // Apply wind rotation with consistent direction and forward lean
            model = glm::rotate(model, windEffect, glm::vec3(wind.direction.x, 0.0f, wind.direction.z));
            // Add a slight forward lean based on wind strength
            model = glm::rotate(model, baseWindEffect * 0.2f, glm::vec3(1.0f, 0.0f, 0.0f));
            
            shader.setMat4("model", glm::value_ptr(model));
            glm::mat4 grassTransform = projection * view * model;
            shader.setMat4("transform", glm::value_ptr(grassTransform));
            shader.setVec3("windDirection", wind.direction.x, wind.direction.y, wind.direction.z);
            shader.setFloat("windStrength", currentWindStrength);
            grassBlade.draw();
        }

        // Draw clouds
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        cloudShader.use();
        cloudShader.setMat4("view", glm::value_ptr(view));
        cloudShader.setMat4("projection", glm::value_ptr(projection));
        cloudShader.setFloat("time", currentFrame);
        cloudShader.setVec3("sunDirection", sun.direction.x, sun.direction.y, sun.direction.z);
        
        for (const auto& model : cloudPositions) {
            cloudShader.setMat4("model", glm::value_ptr(model));
            cloudMesh.draw();
        }
        glDisable(GL_BLEND);

        // Draw small plants
        shader.use();
        for (size_t i = 0; i < plantPositions.size(); i++) {
            const auto& model = plantPositions[i];
            const Plant& plant = plants[i % plants.size()];
            
            shader.setMat4("model", glm::value_ptr(model));
            shader.setVec3("color", plant.color.x, plant.color.y, plant.color.z);
            glm::mat4 plantTransform = projection * view * model;
            shader.setMat4("transform", glm::value_ptr(plantTransform));
            grassBlade.draw(); // Reuse grass blade mesh for plants
        }

        window.swapBuffers();
        window.pollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);

    return 0;
} 