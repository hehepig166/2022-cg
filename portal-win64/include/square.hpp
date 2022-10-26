#ifndef _HEHEPIG_SQUARE_
#define _HEHEPIG_SQUARE_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_m.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

struct square {
    float width, height;
    glm::vec3 color;
    glm::mat4 model;

    square(float w=.5, float h=.5, glm::vec3 col=glm::vec3(1.0f)): width(w), height(h), color(col), model(1.0f) {}

    void draw(GLFWwindow *window, const glm::mat4 &view, const glm::mat4 &proj, Shader &shader);
};


void printvec4(glm::vec4 o) {
    std::cout <<o.x <<" " <<o.y <<" " <<o.z <<std::endl;
}


void square::draw(GLFWwindow *window, const glm::mat4 &view, const glm::mat4 &proj, Shader &shader) {

    float vertices[] = {
        0,      0,      0,
        width,  0,      0,
        width,  height, 0,
        0,      height, 0
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)NULL);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    shader.use();

    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", proj);
    shader.setVec3("color", color);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

#endif