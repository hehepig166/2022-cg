#ifndef _HEHEPIG_PORTAL_
#define _HEHEPIG_PORTAL_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_m.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

struct Portal {
    Portal* destPortal;
    glm::mat4 model;
    glm::vec3 color;

    Portal(): destPortal(NULL), model(1.0f) {}

    void draw(GLFWwindow *window, const glm::mat4 &view, const glm::mat4 &proj, Shader &shader);
};


void Portal::draw(GLFWwindow *window, const glm::mat4 &view, const glm::mat4 &proj, Shader &shader) {
    const float width=1, height=.6;
    square sq(width, height, color);
    sq.model = model * glm::translate(sq.model, glm::vec3(-width/2, -height/2, 0));
    sq.draw(window, view, proj, shader);
}


#endif