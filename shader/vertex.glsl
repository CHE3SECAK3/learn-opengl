#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 fColor;
out vec2 fTexCoord;

uniform float xOffset;
uniform mat4 model, view, projection;

void main() {
	gl_Position = projection * view * model * vec4(aPos.x + xOffset, aPos.yz, 1.0f);
	fColor = aColor;
	fTexCoord = aTexCoord;
}