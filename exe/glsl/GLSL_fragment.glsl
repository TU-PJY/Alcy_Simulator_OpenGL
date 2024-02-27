#version 330 core

in vec3 fragPos;
in vec2 TexCoord;

out vec4 fragColor;

uniform vec3 viewPos;

uniform sampler2D outTexture;
uniform float transparency;

void main() {
    vec4 texColor = texture(outTexture, TexCoord);

    // �ؽ�ó�� ���� ���� 0.01���� ������ (���� ������ �κ�)
    if (texColor.a < 0.01)
        discard;  // �ش� �ȼ��� ������� ����

    else
        fragColor = vec4(texColor.rgb, texColor.a * transparency);
}