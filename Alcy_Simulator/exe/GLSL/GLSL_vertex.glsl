#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 2) in vec2 vTexCoord;

out vec3 fragPos;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main() {
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
    fragPos = vec3(model * vec4(inPosition, 1.0));
    TexCoord = vTexCoord;
}