#ifndef _HEHEPIG_PORTAL_
#define _HEHEPIG_PORTAL_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <learnopengl/shader_m.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "square.hpp"

struct Portal {
    constexpr static float width=1.0f;
    constexpr static float height=2.0f;
    Portal* destPortal;
    glm::mat4 model;
    glm::vec3 color;

    Portal(): destPortal(NULL), model(1.0f) {}

    void draw(GLFWwindow *window, const glm::mat4 &view, const glm::mat4 &proj, Shader &shader);

    bool test_through(glm::vec3 from, glm::vec3 to);
};


void Portal::draw(GLFWwindow *window, const glm::mat4 &view, const glm::mat4 &proj, Shader &shader) {
    square sq(width, height, color);
    sq.model = model * glm::translate(sq.model, glm::vec3(-width/2, -height/2, 0));
    sq.draw(window, view, proj, shader);
}

/*
static void printvec4(glm::vec4 x) {
    std::cout <<x.x <" " <<x.y <<" " <<x.z <<std::endl;
}
*/


bool Portal::test_through(glm::vec3 from_, glm::vec3 to_) {
    
    glm::vec4 from = glm::inverse(model) * glm::vec4(from_, 1.0f);
    glm::vec4 to = glm::inverse(model) * glm::vec4(to_, 1.0f);

    //printvec4(from);
    //printvec4(to);
    
    if (from.z > 0 && to.z < 0) {
        float t = to.z / (to.z-from.z);
        float x = to.x + t*(from.x-to.x);
        float y = to.y + t*(from.y-to.y);
        //std::cout <<x <<" " <<y <<std::endl;
        return fabs(x)<=width/2 && fabs(y)<=height/2;
    }
    else {
        return false;
    }
}




#endif