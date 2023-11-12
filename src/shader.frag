#version 330 core
in vec3 ourColor;
in vec2 texCoord;

out vec4 fragColor;

uniform sampler2D inTexture;

void main() {
    fragColor = texture(inTexture, texCoord);
}