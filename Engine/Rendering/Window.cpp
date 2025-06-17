#include "Window.h"
#include <iostream>

Window::Window(int width, int height, const std::string& title) 
    : width(width), height(height), title(title), window(nullptr) {
}

Window::~Window() {
    if (window) {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}

bool Window::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    glClearColor(0.1f, 0.12f, 0.15f, 1.0f);
    return true;
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::clear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
}

void Window::pollEvents() {
    glfwPollEvents();
} 