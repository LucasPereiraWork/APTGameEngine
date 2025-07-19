#version 330 core
uniform mat4 model;
uniform mat4 projection;

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 vertexTexCoords;

out vec2 TexCoords;

void main() {
    gl_Position = projection * model * vec4(vertexPosition, 1.0);
    TexCoords = vertexTexCoords;
}
