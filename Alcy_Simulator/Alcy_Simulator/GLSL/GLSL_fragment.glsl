#version 330 core

in vec3 fragPos;
in vec2 TexCoord;

out vec4 fragColor;

uniform vec3 viewPos;

uniform sampler2D outTexture;
uniform float transparency;

void main() {
    vec4 texColor = texture(outTexture, TexCoord);

    // 텍스처의 알파 값이 0.01보다 작으면 (거의 투명한 부분)
    if (texColor.a < 0.01)
        discard;  // 해당 픽셀을 출력하지 않음

    else
        fragColor = vec4(texColor.rgb, texColor.a * transparency);
}