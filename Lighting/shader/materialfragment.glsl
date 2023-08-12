#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light {
	vec3 position;
	vec3 color;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 fNormal;
in vec3 fPos;

uniform Material material;
uniform Light light;

void main() {
	//ambient
	vec3 ambient = light.ambient * material.ambient;

	//diffuse
	vec3 lightDir = normalize(light.position - fPos);

	float diffuseFactor = max(dot(fNormal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffuseFactor * material.diffuse;

	//specular
	vec3 reflectDir = reflect(-lightDir, fNormal);
	vec3 viewDir = normalize(-fPos);

	float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * specularFactor * material.specular;

	vec3 objectColor = (ambient + diffuse + specular) * light.color;

	FragColor = vec4(objectColor, 1.0);
}