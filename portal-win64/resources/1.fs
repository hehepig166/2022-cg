#version 330 core
out vec4 FragColor;

in float dep;

void main()
{
    FragColor = vec4(vec3(1.0f, 1.0f, 1.0f)*dep, 1.0f);
}