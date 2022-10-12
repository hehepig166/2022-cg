#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>

#include <cmath>
#include <iostream>
#include <vector>


// consts
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const float PI = 3.14159265;

float lightpos_x=0, lightpos_y=0, lightpos_z=-5.0;

float pos_x, pos_y, pos_z, radius=1.0;


struct Circle {
    float R;
    int X_SEG, Y_SEG;   // div
    std::vector<float> Vertices;
    std::vector<int> Indices;


    void Init() {

        Vertices.clear();
        Indices.clear();

        // Vertices
        for (int y=0; y<=Y_SEG; y++) {
            for (int x=0; x<=X_SEG; x++) {
                float a = 1.0f*x/X_SEG;
                float b = 1.0f*y/Y_SEG;
                float xPos = std::cos(a*2.0f*PI)*std::sin(b*PI)*R;
                float yPos = std::cos(b*PI)*R;
                float zPos = std::sin(a*2.0f*PI)*std::sin(b*PI)*R;

                Vertices.push_back(xPos);
                Vertices.push_back(yPos);
                Vertices.push_back(zPos);
            }
        }

        // Indices
        for (int i=0; i<Y_SEG; i++) {
            for (int j=0; j<X_SEG; j++) {
                Indices.push_back(i*(X_SEG+1)+j);
                Indices.push_back((i+1)*(X_SEG+1)+j);
                Indices.push_back((i+1)*(X_SEG+1)+j+1);

                Indices.push_back(i*(X_SEG+1)+j);
                Indices.push_back((i+1)*(X_SEG+1)+j+1);
                Indices.push_back(i*(X_SEG+1)+j+1);
            }
        }
    }

    Circle(): R(1), X_SEG(50), Y_SEG(50) {Init();}
};


void processInput(GLFWwindow *window);


int main()
{

    // init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "hehepig", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    Shader shader("resources/3.vs", "resources/3.fs");

    Circle a;
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    // bind vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, a.Vertices.size()*sizeof(float), &a.Vertices[0], GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, a.Indices.size()*sizeof(int), &a.Indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window)) {

        processInput(window);


        glClearColor(.2f, .3f, .3f, .0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        shader.use();

        // set transformations
        glm::mat4 model         = glm::mat4(1.0f);
        glm::mat4 view          = glm::mat4(1.0f);
        glm::mat4 projection    = glm::mat4(1.0f);

        model = glm::translate(model, glm::vec3(pos_x, pos_y, -pos_z));
        model = glm::scale(model, glm::vec3(radius, radius, radius));

        view = glm::translate(view, glm::vec3(.0f, .0f, -5.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        unsigned int modelLoc = glGetUniformLocation(shader.ID, "model");
        unsigned int viewLoc  = glGetUniformLocation(shader.ID, "view");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        shader.setMat4("projection", projection);


        // set lightpos
        glm::vec3 lightpos = glm::vec3(lightpos_x, lightpos_y, lightpos_z);
        shader.setVec3("lightpos", lightpos);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glBindVertexArray(VAO);

        // draw circle
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, a.X_SEG*a.Y_SEG*6, GL_UNSIGNED_INT, 0);

        // draw light
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(lightpos_x, lightpos_y, -lightpos_z));
        model = glm::scale(model, glm::vec3(0.1, 0.1, 0.05));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &model[0][0]);
        glDrawElements(GL_TRIANGLES, a.X_SEG*a.Y_SEG*6, GL_UNSIGNED_INT, 0);

        glm::vec3 objpos = glm::vec3(model * glm::vec4(0, 0, 1, 1));
        glm::vec3 ddd = lightpos-objpos;;
        std::cout <<"lightpos  :  " <<lightpos.x <<" " <<lightpos.y <<" " <<lightpos.z <<std::endl;
        std::cout <<"objpos    :  " <<pos_x <<" " <<pos_y <<" " <<pos_z <<std::endl;
        std::cout <<"objradius :  " <<radius <<std::endl;

        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow *window)
{
    static float eps = 0.02;
    static float eps2 = 0.01;
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        lightpos_x-=eps2;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        lightpos_x+=eps2;
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        lightpos_y+=eps2;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        lightpos_y-=eps2;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        lightpos_z-=eps2;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        lightpos_z+=eps2;
    
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        pos_x-=eps;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        pos_x+=eps;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        pos_y+=eps;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        pos_y-=eps;
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        pos_z+=eps;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        pos_z-=eps;
    
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        radius+=eps;
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        radius-=eps;
}