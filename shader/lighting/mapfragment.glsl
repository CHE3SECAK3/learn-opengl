#version 330 core

struct Material {
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
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
in vec2 fTexCoord;

uniform Material material;
uniform Light light;

void main() {
	bool invertSpec = false;

	vec3 tDiffuse = texture(material.diffuse, fTexCoord).rgb;
	vec3 tSpecular = texture(material.specular, fTexCoord).rgb;

	vec3 tSpecInverse = tSpecular + (vec3(0.5) - tSpecular) * 2;

	//ambient
	vec3 ambient = material.ambient * tDiffuse;

	//diffuse
	vec3 lightDir = normalize(light.position - fPos);

	float diffuseFactor = max(dot(fNormal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffuseFactor * tDiffuse;

	//specular
	vec3 reflectDir = reflect(-lightDir, fNormal);
	vec3 viewDir = normalize(-fPos);

	float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * specularFactor * (invertSpec ? tSpecInverse : tSpecular);

	vec3 objectColor = (ambient + diffuse + specular) * light.color;

	FragColor = vec4(objectColor, 1.0);
}