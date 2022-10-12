#version 330 core
layout (location = 0) in vec3 aPos;
out vec3 ourColor;
void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    ourColor = vec3(1.0f, 1.0f, 1.0f)*(aPos.x+aPos.y-aPos.z)*2*0.57735f;
}