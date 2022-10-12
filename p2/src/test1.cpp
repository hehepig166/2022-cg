#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_m.h>

#include <cmath>
#include <iostream>
#include <vector>

// consts
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
const float PI = 3.14159265;


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

    Circle(): R(.5), X_SEG(50), Y_SEG(50) {Init();}
};

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

    Shader shader("resources/1.vs", "resources/1.fs");

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
        glClearColor(.2f, .3f, .3f, .0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glBindVertexArray(VAO);

        // xiankuang
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, a.X_SEG*a.Y_SEG*6, GL_UNSIGNED_INT, 0);

        // dianzhen
        //glPointSize(5);
        //glDrawElements(GL_POINTS, a.X_SEG*a.Y_SEG*6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}