#version 330 core

layout(location = 0) in vec3 vCoord;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tCoord;

out vec3 fPos;
out vec3 fNormal;
out vec2 fTexCoord;

uniform mat4 MV, projection;
uniform mat3 MVN;

void main() {
	fPos = vec3(MV * vec4(vCoord, 1.0));
	fNormal =  normalize(MVN * normal);
	fTexCoord = tCoord;

	gl_Position = projection * vec4(fPos, 1.0);
}