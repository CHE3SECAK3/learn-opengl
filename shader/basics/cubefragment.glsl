#version 330 core

in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D t1, t2;
uniform float ratio;

void main() {
	FragColor = mix(texture(t1, fTexCoord), texture(t2, fTexCoord), ratio);
}