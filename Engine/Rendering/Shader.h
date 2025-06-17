#ifndef SHADER_H
#define SHADER_H

#include <string>
#include </opt/homebrew/include/GLFW/glfw3.h>

class Shader {
private:
    GLuint programID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;

    bool compileShader(const std::string& source, GLuint shaderID);
    bool linkProgram();

public:
    Shader();
    ~Shader();

    bool loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath);
    void use() const;
    
    // Uniform setters
    void setFloat(const std::string& name, float value) const;
    void setInt(const std::string& name, int value) const;
    void setVec3(const std::string& name, float x, float y, float z) const;
    void setMat4(const std::string& name, const float* matrix) const;
};

#endif