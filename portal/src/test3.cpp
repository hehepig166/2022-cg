#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "square.hpp"
#include "learnopengl/camera.h"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void curse_poscallback(GLFWwindow *window, double x, double y);
GLFWwindow* createWindow(int width, int height, const char* title);
int gladInit();


// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 800;
const float PI = 3.1415926;

Camera camera(glm::vec3(1.0f, 1.0f, 3.0f));

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;


void mainloop(GLFWwindow *window)
{

    square a[5];

    float w=5, h=8;

    a[0].width = w;
    a[0].height = h;
    a[1].width = w;
    a[1].height = 4;
    a[1].model = glm::rotate(a[1].model, PI/2, glm::vec3(1, 0, 0));
    a[2].width = 6;
    a[2].height = h;
    a[2].model = glm::rotate(a[2].model, -PI/2, glm::vec3(0, 1, 0));
    a[3].width = 8;
    a[3].height = h;
    a[3].model = glm::translate(a[3].model, glm::vec3(w, 0, 0));
    a[3].model = glm::rotate(a[3].model, -PI/2, glm::vec3(0, 1, 0));
    a[4].width = w;
    a[4].height = h;
    a[4].model = glm::translate(a[4].model, glm::vec3(0, h, 0));
    a[4].model = glm::rotate(a[4].model, PI/2, glm::vec3(1, 0, 0));

    

    while (!glfwWindowShouldClose(window)) {

        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        if (deltaTime < 0.02) continue;
        lastFrame = currentFrame;
        std::cout <<(int)(1.0/deltaTime) <<" fps" <<std::endl;

        // input
        processInput(window);


        // get objects
        //a[0].model = glm::rotate(a[0].model, 0.02f, glm::vec3(1, 0, 0));
        //a[1].model = glm::rotate(a[1].model, 0.05f, glm::vec3(0, 1, 0));

        // get view, projection
        glm::mat4 view          = camera.GetViewMatrix();
        //view = glm::rotate(view, -PI/2, glm::vec3(1, 0, 0));
        glm::mat4 projection    = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);


        // render
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw
        for (int i=0; i<5; i++) {
            a[i].draw(window, view, projection);
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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
}

void curse_poscallback(GLFWwindow *window, double xposIn, double yposIn)
{
    static float lastX = SCR_WIDTH / 2.0f;
    static float lastY = SCR_HEIGHT / 2.0f;
    static bool firstMouse = true;


    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    //std::cout <<lastX <<" " <<lastY <<"->" <<xpos <<" " <<ypos <<"(" <<xposIn <<" " <<yposIn <<")" <<std::endl;

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
    //std::cout <<"pos: " <<x <<", " <<y <<")" <<std::endl;
    //glfwSetCursorPos(window, 0, 0);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
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

    glfwSetCursorPosCallback(window, curse_poscallback);
    glfwSetScrollCallback(window, scroll_callback);
    
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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


