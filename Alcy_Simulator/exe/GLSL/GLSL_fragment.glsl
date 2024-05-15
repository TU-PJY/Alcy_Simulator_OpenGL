#version 330 core

in vec3 fragPos;
in vec2 TexCoord;

out vec4 fragColor;
uniform vec3 objectColor;

uniform vec3 viewPos;

uniform sampler2D outTexture;
uniform float transparency;

void main() {
    vec4 texColor = texture(outTexture, TexCoord);
    vec3 result = objectColor;
   
    if (texColor.a < 0.01)
        discard;

    else
        fragColor = vec4(result.rgb + texColor.rgb, texColor.a * transparency);
}