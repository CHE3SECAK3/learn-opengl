#version 330 core

layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 tcoord;

out vec3 Pos;
out vec3 Norm;
out vec2 TCoord;

uniform mat4 M_modelview;
uniform mat4 M_projection;
uniform mat3 M_normal;

void main() {
	vec4 mvPos = M_modelview * vec4(vertex, 1.0);
	Pos = mvPos.xyz;
	gl_Position = M_projection * mvPos;

	Norm = normalize(M_normal * normal);
	TCoord = tcoord;
}