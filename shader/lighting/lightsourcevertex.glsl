#version 330 core

layout(location = 0) in vec3 vCoord;

uniform mat4 model, view, projection;

void main() {
	gl_Position = projection * view * model * vec4(vCoord, 1.0f);
}