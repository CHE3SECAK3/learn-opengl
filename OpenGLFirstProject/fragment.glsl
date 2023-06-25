#version 330 core

in vec3 fColor;
in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D t1, t2;
uniform float ratio;

void main() {
	FragColor = mix(texture(t1, fTexCoord) * vec4(fColor, 1.0), texture(t2, vec2(-fTexCoord.x, fTexCoord.y)), ratio);
}