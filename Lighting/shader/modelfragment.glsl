#version 330 core

out vec4 FragColor;

in vec3 fNormal;
in vec3 fPos;

uniform vec3 lightPos, cameraPos;
uniform vec3 lightColor, objectColor;
uniform float ambientFactor;
uniform float specularFactor, specularShine;

void main() {
	vec3 lightDir = normalize(lightPos - fPos);
	vec3 reflectDir = reflect(-lightDir, fNormal);
	vec3 viewDir = normalize(cameraPos - fPos);

	float diffuseFactor = max(dot(fNormal, lightDir), 0.0);
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), specularShine);

	vec3 lighting = (ambientFactor + diffuseFactor + specularFactor * specular) * lightColor;

	FragColor = vec4(lighting * objectColor, 1.0);
}