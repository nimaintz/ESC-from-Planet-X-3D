#version 400 core
layout(location = 0) in vec3 aPos;

uniform mat4 MVP;

out vec3 TexCoords;

void main() {
    TexCoords = aPos; // Pass the vertex position to the fragment shader
    gl_Position = MVP * vec4(aPos, 1.0);
}