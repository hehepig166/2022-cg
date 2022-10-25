#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "square.hpp"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void curse_poscallback(GLFWwindow *window, double x, double y);
GLFWwindow* createWindow(int width, int height, const char* title);
int gladInit();


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float PI = 3.1415926;


void mainloop(GLFWwindow *window)
{

    square a[3];

    a[0].width = 4;
    a[0].height = 8;
    a[0].model = glm::rotate(a[0].model, PI/4, glm::vec3(1, 0, 0));

    a[1].width = 5;
    a[1].height = 5;
    a[1].model = glm::rotate(a[1].model, PI/2, glm::vec3(0, 1, 0));

    a[2].width = a[2].height = 3;

    while (!glfwWindowShouldClose(window)) {

        // input
        processInput(window);


        // get objects
        a[0].width = 4;
        a[0].height = 8;
        a[0].model = glm::rotate(a[0].model, 0.02f, glm::vec3(1, 0, 0));

        a[1].width = 5;
        a[1].height = 5;
        a[1].model = glm::rotate(a[1].model, 0.05f, glm::vec3(0, 1, 0));

        // get view, projection
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(-2.0f, -2.0f, -10.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        // render
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw
        for (auto &o: a) {
            o.draw(window, view, projection);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


int main()
{
    glfwInit();

    GLFWwindow* window = createWindow(SCR_WIDTH, SCR_HEIGHT, "hehepig");

    gladInit();

    mainloop(window);

    glfwTerminate();

    return 0;
}







//========================================================================================
//========================================================================================

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void curse_poscallback(GLFWwindow *window, double x, double y)
{
    //std::cout <<"pos: " <<x <<", " <<y <<")" <<std::endl;
    //glfwSetCursorPos(window, 0, 0);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


//========================================================================================
//========================================================================================

GLFWwindow* createWindow(int width, int height, const char* title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return NULL;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //glfwSetCursorPosCallback(window, curse_poscallback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    return window;
}

int gladInit()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    return 0;
}


