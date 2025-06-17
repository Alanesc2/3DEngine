#pragma once

#include <GLFW/glfw3.h>
#include <string>

class Window {
private:
    GLFWwindow* window;
    int width;
    int height;
    std::string title;

public:
    Window(int width, int height, const std::string& title);
    ~Window();

    bool init();
    bool shouldClose() const;
    void clear();
    void swapBuffers();
    void pollEvents();
    GLFWwindow* getWindow() const { return window; }
}; 