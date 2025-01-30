#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "ERROR: Failed to initialize GLFW." << std::endl;
        return -1;
    }

    // Create a window
    GLFWwindow* window = glfwCreateWindow(640, 480, "Pekan", nullptr, nullptr);
    if (!window)
    {
        std::cout << "ERROR: Failed to create a window with GLFW." << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}