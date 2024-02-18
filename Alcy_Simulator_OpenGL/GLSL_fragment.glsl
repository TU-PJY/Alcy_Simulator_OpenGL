#version 330 core

in vec3 fragPos;
in vec2 TexCoord;

out vec4 fragColor;

uniform vec3 viewPos;

uniform sampler2D outTexture;
uniform float transparency;
uniform vec3 targetColor;
uniform vec3 colorThreshold;

void main() {
    vec4 texColor = texture(outTexture, TexCoord);
    if (distance(texColor.rgb, targetColor) < length(colorThreshold)) 
        discard;  // 배경색 제거

    fragColor = vec4(texColor.rgb, transparency);
}