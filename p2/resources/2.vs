#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 ourColor;

uniform vec3 light;


void main()
{
    gl_Position = vec4(aPos.x, aPos.z, -aPos.y, 1.0f);
    ourColor = vec3(1.0f, 1.0f, 1.0f)*2*dot(light, aPos);
    //ourColor = aPos;
}