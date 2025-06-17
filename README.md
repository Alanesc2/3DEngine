# Simple 3D Engine

A lightweight, modern 3D engine built with OpenGL, featuring a flexible rendering pipeline, efficient resource management, and a clean architecture. The engine provides core 3D graphics capabilities and includes example implementations showcasing its features.

![Screenshot of the grass field demo](https://i.imgur.com/PLACEHOLDER1.png)
![Screenshot of the basic rendering demo](https://i.imgur.com/PLACEHOLDER2.png)

## Core Features

- **Modern Rendering Pipeline**
  - OpenGL 3.3+ based rendering
  - Efficient shader management system
  - Support for vertex and fragment shaders
  - Texture and material system
  - Instanced rendering support

- **Math and Geometry**
  - Vector and matrix operations
  - Transform hierarchy
  - Camera system with perspective/orthographic projections
  - Collision detection

- **Resource Management**
  - Efficient mesh handling
  - Shader program management
  - Texture loading and management
  - Memory-efficient resource pooling

- **Scene Management**
  - Entity component system
  - Transform hierarchy
  - Scene graph implementation
  - Efficient object culling

## Example Implementations

The engine includes several example implementations showcasing its capabilities:

### Grass Field Demo
- Procedural terrain generation using Perlin noise
- 10,000+ instanced grass blades with wind simulation
- Dynamic environment with moving sun and clouds
- First-person camera with terrain collision

### Basic Rendering Demo
- Rotating cube with proper lighting
- Basic shader implementation
- Texture mapping
- Camera controls

## Requirements

- C++17 or higher
- OpenGL 3.3+
- GLFW3
- GLM
- CMake 3.10+

## Building

1. Clone the repository:
```bash
git clone https://github.com/yourusername/3Dengine.git
cd 3Dengine
```

2. Create a build directory:
```bash
mkdir build
cd build
```

3. Configure and build:
```bash
cmake ..
make
```

## Running the Examples

After building, you can run the example implementations:

Grass Field Demo:
```bash
./test_grass
```

Basic Rendering Demo:
```bash
./test_shader
```

## Project Structure

```
3Dengine/
├── Engine/
│   ├── Rendering/
│   │   ├── Shaders/
│   │   │   ├── basic.vert
│   │   │   ├── basic.frag
│   │   │   ├── cloud.vert
│   │   │   └── cloud.frag
│   │   └── Shader.h
│   ├── Math/
│   │   ├── Vector3.h
│   │   └── Matrix4.h
│   └── Mesh.h
├── Examples/
│   ├── test_grass.cpp
│   └── test_shader.cpp
├── CMakeLists.txt
└── README.md
```

## Using the Engine

The engine is designed to be easily integrated into new projects. Here's a basic example of how to use it:

```cpp
#include "Engine/Rendering/Shader.h"
#include "Engine/Mesh.h"

// Create a shader program
Shader shader("path/to/vertex.shader", "path/to/fragment.shader");

// Create a mesh
auto mesh = Mesh::createCube();

// Main loop
while (!window.shouldClose()) {
    shader.use();
    // Set uniforms and render
    mesh.draw();
}
```

## Future Improvements

- [ ] Enhanced shader system with material support
- [ ] Model loading system (OBJ, FBX)
- [ ] Advanced lighting system (PBR)
- [ ] Post-processing effects
- [ ] Physics integration
- [ ] Audio system
- [ ] Networking capabilities
- [ ] Editor tools
- [ ] Documentation generation

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request. See [CONTRIBUTING.md](CONTRIBUTING.md) for more details.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- OpenGL and GLFW for graphics rendering
- GLM for mathematics
- Perlin noise implementation for terrain generation 