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
    // bulb
    //vec3 light = normalize(lightpos - objpos);
    // sun
    vec3 light = normalize(lightpos - vec3(model * vec4(1.0f, 1.0f, 1.0f, 1.0f)));
    vec3 col1 = vec3(1.0, 1.0, 1.0) * max(dot(light, aPos), 0.1);
    ourColor = mix(vec3(.5, .0, 1.0), col1, .6);
}