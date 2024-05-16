#version 330 core

in vec3 fragPos;
in vec2 TexCoord;

out vec4 fragColor;

uniform vec4 objectColor;
uniform vec3 viewPos;

uniform sampler2D outTexture;
uniform float transparency;

void main() {
    vec4 texColor = texture(outTexture, TexCoord);

    if (texColor.a < 0.01)
        discard;

    else
        fragColor = vec4(objectColor.rgb + texColor.rgb, texColor.a * objectColor.a * transparency);
}