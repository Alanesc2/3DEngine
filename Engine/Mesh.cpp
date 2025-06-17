#include "Mesh.h"
#include <cmath>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    : vertices(vertices), indices(indices) {
    setupMesh();
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::setupMesh() {
    // Create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    
    // Load data into vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    // Load data into element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // Set vertex attribute pointers
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glEnableVertexAttribArray(1);
    
    // Normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(2);
    
    // Texture coordinate attribute
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}

void Mesh::draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Mesh Mesh::createCube(float size) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Define vertices for a cube
    float halfSize = size / 2.0f;
    
    // Front face
    vertices.push_back({{-halfSize, -halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f}});
    vertices.push_back({{ halfSize, -halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}});
    vertices.push_back({{ halfSize,  halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 0.0f}});
    vertices.push_back({{-halfSize,  halfSize,  halfSize}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}});
    
    // Back face
    vertices.push_back({{-halfSize, -halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 0.0f, 0.0f}});
    vertices.push_back({{-halfSize,  halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {1.0f, 1.0f, 0.0f}});
    vertices.push_back({{ halfSize,  halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f}});
    vertices.push_back({{ halfSize, -halfSize, -halfSize}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, 0.0f}});
    
    // Top face
    vertices.push_back({{-halfSize,  halfSize, -halfSize}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}});
    vertices.push_back({{-halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});
    vertices.push_back({{ halfSize,  halfSize,  halfSize}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
    vertices.push_back({{ halfSize,  halfSize, -halfSize}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}});
    
    // Bottom face
    vertices.push_back({{-halfSize, -halfSize, -halfSize}, {1.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}});
    vertices.push_back({{ halfSize, -halfSize, -halfSize}, {1.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}});
    vertices.push_back({{ halfSize, -halfSize,  halfSize}, {1.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});
    vertices.push_back({{-halfSize, -halfSize,  halfSize}, {1.0f, 1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
    
    // Right face
    vertices.push_back({{ halfSize, -halfSize, -halfSize}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
    vertices.push_back({{ halfSize,  halfSize, -halfSize}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}});
    vertices.push_back({{ halfSize,  halfSize,  halfSize}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}});
    vertices.push_back({{ halfSize, -halfSize,  halfSize}, {1.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});
    
    // Left face
    vertices.push_back({{-halfSize, -halfSize, -halfSize}, {0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});
    vertices.push_back({{-halfSize, -halfSize,  halfSize}, {0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
    vertices.push_back({{-halfSize,  halfSize,  halfSize}, {0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f}});
    vertices.push_back({{-halfSize,  halfSize, -halfSize}, {0.0f, 1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}});

    // Define indices for the cube (6 faces, 2 triangles per face, 3 vertices per triangle)
    for (int i = 0; i < 6; i++) {
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

    return Mesh(vertices, indices);
}

Mesh Mesh::createPlane(float size, Vector3 color) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    float halfSize = size / 2.0f;
    
    // Define vertices for a plane
    vertices.push_back({{-halfSize, 0.0f, -halfSize}, color, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 0.0f}});
    vertices.push_back({{ halfSize, 0.0f, -halfSize}, color, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}});
    vertices.push_back({{ halfSize, 0.0f,  halfSize}, color, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 0.0f}});
    vertices.push_back({{-halfSize, 0.0f,  halfSize}, color, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}});
    
    // Define indices for the plane (2 triangles)
    indices = {0, 1, 2, 0, 2, 3};
    
    return Mesh(vertices, indices);
}

Mesh Mesh::createSphere(float radius, int sectors, int stacks) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    
    // Generate vertices
    for (int i = 0; i <= stacks; ++i) {
        float stackAngle = M_PI / 2 - i * M_PI / stacks;
        float xy = radius * cosf(stackAngle);
        float z = radius * sinf(stackAngle);
        
        for (int j = 0; j <= sectors; ++j) {
            float sectorAngle = j * 2 * M_PI / sectors;
            float x = xy * cosf(sectorAngle);
            float y = xy * sinf(sectorAngle);
            
            // Position
            Vector3 position(x, y, z);
            // Normal (normalized position)
            Vector3 normal = position.normalize();
            // Color (based on position)
            Vector3 color((x + radius) / (2 * radius),
                         (y + radius) / (2 * radius),
                         (z + radius) / (2 * radius));
            // Texture coordinates
            Vector3 texCoords((float)j / sectors, (float)i / stacks, 0.0f);
            
            vertices.push_back({position, color, normal, texCoords});
        }
    }
    
    // Generate indices
    for (int i = 0; i < stacks; ++i) {
        int k1 = i * (sectors + 1);
        int k2 = k1 + sectors + 1;
        
        for (int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);
            }
            
            if (i != (stacks - 1)) {
                indices.push_back(k1 + 1);
                indices.push_back(k2);
                indices.push_back(k2 + 1);
            }
        }
    }
    
    return Mesh(vertices, indices);
} 