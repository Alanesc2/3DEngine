#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader() : programID(0), vertexShaderID(0), fragmentShaderID(0) {
    programID = glCreateProgram();
}

Shader::~Shader() {
    if (vertexShaderID) glDeleteShader(vertexShaderID);
    if (fragmentShaderID) glDeleteShader(fragmentShaderID);
    if (programID) glDeleteProgram(programID);
}

bool Shader::compileShader(const std::string& source, GLuint shaderID) {
    const char* src = source.c_str();
    glShaderSource(shaderID, 1, &src, nullptr);
    glCompileShader(shaderID);

    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar infoLog[512];
        glGetShaderInfoLog(shaderID, 512, nullptr, infoLog);
        std::cerr << "Shader compilation error: " << infoLog << std::endl;
        return false;
    }
    return true;
}

bool Shader::linkProgram() {
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);

    glLinkProgram(programID);
    GLint success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);

    if (!success) {
        GLchar infoLog[512];
        glGetProgramInfoLog(programID, 512, nullptr, infoLog);
        std::cerr << "Program linking error: " << infoLog << std::endl;
        return false;
    }
    return true;
}

bool Shader::loadFromFiles(const std::string& vertexPath, const std::string& fragmentPath) {
    std::cout << "Trying to load vertex shader from: " << vertexPath << std::endl;
    std::cout << "Trying to load fragment shader from: " << fragmentPath << std::endl;

    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    std::ifstream vertexFile(vertexPath);
    if (!vertexFile.is_open()) {
        std::cerr << "Failed to open vertex shader file" << std::endl;
        return false;
    }
    std::stringstream vertexStream;
    vertexStream << vertexFile.rdbuf();
    std::string vertexCode = vertexStream.str();
    std::cout << "Vertex shader code:\n" << vertexCode << std::endl;

    std::ifstream fragmentFile(fragmentPath);
    if (!fragmentFile.is_open()) {
        std::cerr << "Failed to open fragment shader file" << std::endl;
        return false;
    }
    std::stringstream fragmentStream;
    fragmentStream << fragmentFile.rdbuf();
    std::string fragmentCode = fragmentStream.str();
    std::cout << "Fragment shader code:\n" << fragmentCode << std::endl;

    if (!compileShader(vertexCode, vertexShaderID)) return false;
    if (!compileShader(fragmentCode, fragmentShaderID)) return false;

    return linkProgram();
}

void Shader::use() const {
    glUseProgram(programID);
}

void Shader::setFloat(const std::string& name, float value) const {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setInt(const std::string&name, int value) const {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::setVec3(const std::string&name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(programID, name.c_str()), x, y, z);
}

void Shader::setMat4(const std::string&name, const float* matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, matrix);
}