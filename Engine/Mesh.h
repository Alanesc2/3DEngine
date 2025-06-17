#pragma once

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>
#include <vector>
#include <string>
#include "Vector3.h"

struct Vertex {
    Vector3 position;
    Vector3 color;
    Vector3 normal;
    Vector3 texCoords;  // Using Vector3 for future flexibility (3D textures)
};

class Mesh {
private:
    // Render data
    unsigned int VAO, VBO, EBO;
    
    // Mesh data
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Setup mesh on GPU
    void setupMesh();

public:
    // Constructor
    Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices);
    
    // Destructor
    ~Mesh();
    
    // Render the mesh
    void draw() const;
    
    // Getters
    const std::vector<Vertex>& getVertices() const { return vertices; }
    const std::vector<unsigned int>& getIndices() const { return indices; }
    
    // Static helper methods
    static Mesh createCube(float size = 1.0f);
    static Mesh createPlane(float size = 1.0f, Vector3 color = Vector3(1.0f, 1.0f, 1.0f));
    static Mesh createSphere(float radius = 1.0f, int sectors = 20, int stacks = 20);
};
