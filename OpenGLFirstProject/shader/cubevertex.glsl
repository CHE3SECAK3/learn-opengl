#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

out vec2 fTexCoord;

uniform mat4 model, view, projection;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	fTexCoord = aTexCoord;
}