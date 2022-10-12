#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 ourColor;

uniform vec3 lightpos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    vec3 objpos = vec3(model * vec4(aPos, 1.0));
    vec3 centerpos = vec3(model * vec4(.0f, .0f, .0f, 1.0f));
    objpos.z = -objpos.z;
    centerpos.z = -centerpos.z;
    vec3 light = normalize(lightpos - objpos);
    //vec3 light = normalize(lightpos - centerpos);
    ourColor = vec3(1.0f, 1.0f, 1.0f) * max(dot(light, aPos), 0.1f);
    //ourColor = vec3(1.0f, 1.0f, 1.0f) * -aPos.z;
}