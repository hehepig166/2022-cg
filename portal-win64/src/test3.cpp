#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "camera.h"
#include "square.hpp"
#include "portal.hpp"

void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void curse_poscallback(GLFWwindow *window, double x, double y);
GLFWwindow* createWindow(int width, int height, const char* title);
int gladInit();

void drawSimplePortal(Portal &portal, GLFWwindow *window, const glm::mat4 &view, const glm::mat4 &projection);
void drawRecursivePortals(Portal &portal, GLFWwindow *window, const glm::mat4 &view, const glm::mat4 &projection, int maxRecursionLevel, int curRecursionLevel);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float PI = 3.1415926;

Camera camera(glm::vec3(0.0f, 0.0f, .4f));

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int anum=5;
square a[5];
Portal b[2];
Shader *shader;

float pitchang, yawang;

void tmp_generate()
{
    float w=2, h=4, d=.5;
    a[0].width = w;
    a[0].height = h;
    a[0].color = glm::vec3(.0f, 0.5f, 0.0f);
    a[0].model = glm::translate(a[0].model, glm::vec3(-w/2, -h/2, 0.0f));

    a[1].width = w;
    a[1].height = h;
    a[1].color = glm::vec3(.5f, 0.0f, 0.5f);
    a[1].model = glm::translate(a[1].model, glm::vec3(w/2+d, -h/2, 0.0f));

    a[2].width = .05;
    a[2].height = 1;
    a[2].color = glm::vec3(.0f, 1.0f, .25f);
    a[2].model = glm::translate(a[2].model, glm::vec3(w+d, -1.0f, 0.0f)) * glm::rotate(glm::mat4(1), PI/2, glm::vec3(1.0f, .0f, .0f));

    a[3].width = .2;
    a[3].height = .3;
    a[3].color = glm::vec3(.5f, 0.0f, 1.0f);
    a[3].model = glm::translate(a[3].model, glm::vec3(.0f, -1.0f, 0.0f)) * glm::rotate(glm::mat4(1), -PI/4, glm::vec3(.0f, 1.0f, .0f));

    a[anum-1].width = .3;
    a[anum-1].height = .3;
    a[anum-1].color = glm::vec3(0.0f, 0.0f, .3f);

    // red
    b[0].destPortal = &b[1];
    b[0].color = glm::vec3(1.0f, .5f, .25f);
    b[0].model = glm::translate(glm::mat4(1), glm::vec3(0.0f, 1.0f, Portal::height/2)) * glm::rotate(glm::mat4(1), PI/2, glm::vec3(1.0f, 0.0f, 0.0f));

    // blue
    b[1].destPortal = &b[0];
    b[1].color = glm::vec3(0.25f, .5f, 1.0f);
    b[1].model = glm::translate(glm::mat4(1), glm::vec3(w+d, 1.0f, Portal::height/2)) * glm::rotate(glm::mat4(1), PI/2, glm::vec3(1.0f, 0.0f, 0.0f));
}


void tmp_move()
{
    //a[0].model = glm::rotate(a[0].model, 0.02f, glm::vec3(1, 0, 0));
    //a[1].model = glm::rotate(a[1].model, 0.05f, glm::vec3(0, 1, 0));
}


void mainloop(GLFWwindow *window)
{
    shader = new Shader("resources/pureColor.vs", "resources/pureColor.fs");
    while (!glfwWindowShouldClose(window)) {

        // per-frame time logic
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        if (deltaTime < 0.015) continue;
        lastFrame = currentFrame;
        //std::cout <<(int)(1.0/deltaTime) <<" fps" <<std::endl;

        // input
        processInput(window);


        // get view, projection
        glm::mat4 view          = camera.GetViewMatrix();
        glm::mat4 projection    = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // move
        a[anum-1].model = glm::translate(glm::mat4(1.0f), camera.Position)
            * glm::rotate(glm::mat4(1.0f), glm::radians(camera.Yaw), glm::vec3(0, 0, 1))
            * glm::rotate(glm::mat4(1.0f), glm::radians(camera.Pitch+90), glm::vec3(1, 0, 0))
            * glm::translate(glm::mat4(1.0f), glm::vec3(-.25, -.25, 0));
            //* glm::translate(glm::mat4(1.0f), glm::vec3(-.25, -.25, 0));
        b[1].model = glm::translate(glm::mat4(1), glm::vec3(2.5, 1.0f, Portal::height/2))
            * glm::rotate(glm::mat4(1), yawang, glm::vec3(0, 0, 1))
            * glm::rotate(glm::mat4(1), pitchang, glm::vec3(0, 1, 0))
            * glm::rotate(glm::mat4(1), PI/2, glm::vec3(1.0f, 0.0f, 0.0f));

        // render
        glClearColor(.2f, .3f, .3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        // draw

        if (glm::distance(glm::vec3(b[0].model*glm::vec4(0, 0, 0, 1)), camera.Position) < glm::distance(glm::vec3(b[1].model*glm::vec4(0, 0, 0, 1)), camera.Position)) {
            if ((glm::inverse(b[0].model) * glm::vec4(camera.Position, 1.0f)).z >= 0)
                drawSimplePortal(b[0], window, view, projection);
            glClear(GL_STENCIL_BUFFER_BIT);
            if ((glm::inverse(b[1].model) * glm::vec4(camera.Position, 1.0f)).z >= 0)
                drawSimplePortal(b[1], window, view, projection);
        }
        else {
            if ((glm::inverse(b[1].model) * glm::vec4(camera.Position, 1.0f)).z >= 0)
                drawSimplePortal(b[1], window, view, projection);
            glClear(GL_STENCIL_BUFFER_BIT);
            if ((glm::inverse(b[0].model) * glm::vec4(camera.Position, 1.0f)).z >= 0)
                drawSimplePortal(b[0], window, view, projection);
        }
        //b[1].draw(window, view, projection, *shader);
        //drawRecursivePortals(b[0], window, view, projection, 3, 1);
        //glClear(GL_STENCIL_BUFFER_BIT);
        //drawRecursivePortals(b[1], window, view, projection, 3, 1);


        for (int i=0; i<anum; i++)
            a[i].draw(window, view, projection, *shader);
        for (int i=0; i<2; i++) if ((glm::inverse(b[i].model) * glm::vec4(camera.Position, 1.0f)).z < 0) {
            b[i].draw(window, view, projection, *shader);
        }

        //drawSimplePortal(b[0], window, view, projection);

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

    tmp_generate();

    mainloop(window);

    glfwTerminate();

    return 0;
}


void tmpfff(glm::vec3 x) {
    std::cout <<x.x <<" " <<x.y <<" " <<x.z <<std::endl;
}


//========================================================================================
//========================================================================================

void processInput(GLFWwindow *window)
{
    glm::vec3 old_pos = camera.Position;

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
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        yawang+=.02;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        pitchang+=.02;
    
    glm::vec3 new_pos = camera.Position;
    for (int i=0; i<2; i++) if (b[i].test_through(old_pos, new_pos)) {
        std::cout <<camera.Position.x <<" " <<camera.Position.y <<" " <<camera.Position.z <<std::endl;
        glm::mat4 tmpmodel = b[i].destPortal->model
            * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f))
            * glm::inverse(b[i].model);

        camera.Position = glm::vec3(tmpmodel * glm::vec4(camera.Position, 1.0f));

        glm::vec3 Base = glm::vec3(tmpmodel * glm::vec4(0, 0, 0, 1));
        glm::vec3 Up = glm::vec3(tmpmodel * glm::vec4(camera.Up, 1.0f)) - Base;
        glm::vec3 Right = glm::vec3(tmpmodel * glm::vec4(camera.Right, 1.0f)) - Base;

        camera.Front = glm::normalize(glm::cross(Up, Right));
        camera.updateCameraVectors(false);
        std::cout <<camera.Position.x <<" " <<camera.Position.y <<" " <<camera.Position.z <<std::endl;
        break;
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        std::cout <<camera.Yaw <<" " <<camera.Pitch <<std::endl;
    }
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
    
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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


//========================================================================================
//========================================================================================

void drawSimplePortal(Portal &portal, GLFWwindow *window, const glm::mat4 &view, const glm::mat4 &projection)
{
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilMask(0xff);

    portal.draw(window, view, projection, *shader);

    glm::mat4 destView = view
        * portal.model
        * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::inverse(portal.destPortal->model);

    glStencilFunc(GL_EQUAL, 1, 0xff);
    glStencilMask(0x00);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);

    for (int i=0; i<anum; i++) {
        a[i].draw(window, destView, projection, *shader);
    }

    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    portal.draw(window, view, projection, *shader);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    glStencilMask(0xff);
    glDisable(GL_STENCIL_TEST);
}


void drawRecursivePortals(Portal &portal, GLFWwindow *window, const glm::mat4 &view, const glm::mat4 &projection, int maxRecursionLevel, int curRecursionLevel)
{
    //std::cout <<curRecursionLevel <<std::endl;
    // 禁止颜色缓冲、深度缓冲
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glDisable(GL_DEPTH_TEST);

    // 启用模板测试
    glEnable(GL_STENCIL_TEST);

    // 模板测试不通过时加1
    glStencilFunc(GL_NOTEQUAL, curRecursionLevel, 0xff);
    glStencilOp(GL_INCR, GL_KEEP, GL_KEEP);
    glStencilMask(0xff);

    // 渲染传送门范围，更新区域模板数值
    portal.draw(window, view, projection, *shader);

    // 计算新的 view 变换矩阵
    glm::mat4 destView = view
        * portal.model
        * glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f))
        * glm::inverse(portal.destPortal->model);
    
    //std::cout <<"#" <<std::endl;
    
    // 最后一层门，只渲染景物
    if (curRecursionLevel == maxRecursionLevel) {
        // 打开颜色缓冲、深度缓冲
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);
        glClear(GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        // 模板测试检验但不可写
        glEnable(GL_STENCIL_TEST);
        glStencilMask(0x00);
        // 等于模板数值的通过测试
        glStencilFunc(GL_EQUAL, curRecursionLevel+1, 0xff);
        // 渲染所有普通景物
        for (int i=0; i<anum; i++)
            a[i].draw(window, destView, projection, *shader);
    }
    else {
        // 更新递归层数，用新的相机矩阵、投影矩阵进行渲染
        drawRecursivePortals(portal, window, destView, projection, maxRecursionLevel, curRecursionLevel+1);
    }

    //std::cout <<"." <<std::endl;

    // 关闭深度测试、颜色缓冲，重新把模板缓冲数值减一
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0xff);

    glStencilFunc(GL_NOTEQUAL, curRecursionLevel+1, 0xff);
    glStencilOp(GL_DECR, GL_KEEP, GL_KEEP);

    portal.draw(window, view, projection, *shader);

    if (curRecursionLevel > 0) {
        // 渲染景物
        // 关闭模板测试、颜色缓冲，在门的位置记录门的深度
        glDisable(GL_STENCIL_TEST);
        glStencilMask(0x00);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_ALWAYS);
        glClear(GL_DEPTH_BUFFER_BIT);

        portal.draw(window, view, projection, *shader);

        glDepthFunc(GL_LESS);

        // 打开模板测试，等于模板缓冲数值的通过测试，渲染这一层门内的内容
        glEnable(GL_STENCIL_TEST);
        glStencilMask(0x00);

        glStencilFunc(GL_EQUAL, curRecursionLevel, 0xff);

        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glDepthMask(GL_TRUE);

        glEnable(GL_DEPTH_TEST);

        for (int i=0; i<anum; i++)
            a[i].draw(window, destView, projection, *shader);
    }

    glDisable(GL_STENCIL_TEST);
}