#version 330 core
layout (location = 0) in vec3 aPos;

out float dep;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
    vec4 pos = model * vec4(aPos, 1.0f);
    dep = max(pos.z+1,  0.0f)/5;
}