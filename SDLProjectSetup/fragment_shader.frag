#version 330 core
uniform sampler2D texture1;
uniform vec2 texOffset;
uniform vec2 texScale;

in vec2 TexCoords;

out vec4 FragColor;

void main() {
    vec2 adjustedTexCoords = TexCoords * texScale + texOffset;
    vec4 texColor = texture(texture1, adjustedTexCoords);
    FragColor = texColor * vec4(1.0, 0.0, 0.0, 1.0); // Tint red to see if rendering works
}