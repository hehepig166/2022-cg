#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "portal.hpp"

const float PI = 3.1415926;

int main()
{
    Portal portal;
    portal.model = glm::mat4(1.0f) * glm::rotate(glm::mat4(1.0f), PI/2, glm::vec3(1, 0, 0));
    glm::vec3 A(-1.0f, -1.0f, -1.0f);
    glm::vec3 B(2.0f, 1.0f, 2.0f);
    std::cout <<portal.test_through(A, B) <<std::endl;
    std::cout <<portal.test_through(B, A) <<std::endl;
}