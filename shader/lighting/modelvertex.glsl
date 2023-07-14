#version 330 core

layout(location = 0) in vec3 vCoord;
layout(location = 1) in vec3 normal;

out vec3 fPos;
out vec3 fNormal;

uniform mat4 model, view, projection;
uniform mat3 normalMatrix;

void main() {
	fPos = vec3(model * vec4(vCoord, 1.0));
	fNormal =  normalize(normalMatrix * normal);

	gl_Position = projection * view * vec4(fPos, 1.0);
}