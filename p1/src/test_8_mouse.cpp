#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void curse_poscallback(GLFWwindow *window, double x, double y)
{
    std::cout <<"pos: " <<x <<", " <<y <<")" <<std::endl;
    //glfwSetCursorPos(window, 0, 0);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    GLFWwindow *window = glfwCreateWindow(800, 600, "hehepig", NULL, NULL);
    
    glfwMakeContextCurrent(window);
    
    glfwSetCursorPosCallback(window, curse_poscallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(1.4f, 0.5f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
    }
}