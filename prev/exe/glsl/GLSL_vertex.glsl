#version 330 core

layout(location = 0) in vec3 inPosition; // 정점 위치 attribute
layout(location = 2) in vec2 vTexCoord;

out vec3 fragPos; // 프래그먼트 셰이더로 전달할 정점 위치
out vec2 TexCoord;

uniform mat4 model;      // 모델 행렬
uniform mat4 view;       // 뷰 행렬
uniform mat4 projection; // 투영 행렬


void main() {
    gl_Position = projection * view * model * vec4(inPosition, 1.0);
    fragPos = vec3(model * vec4(inPosition, 1.0));
    TexCoord = vTexCoord;
}
