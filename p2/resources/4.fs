#version 330 
in vec3 ourColor;
out vec4 FragColor;
void main()
{
    //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    FragColor = vec4(ourColor, 1.0f);
}