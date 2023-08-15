#version 330 core

layout(location = 0) in vec3 vertex;

uniform mat4 M_MVP;

void main() {
	gl_Position = M_MVP * vec4(vertex, 1.0);
}