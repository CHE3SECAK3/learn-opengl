#version 330 core

struct Material {
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
	sampler2D emission;
	float intensity;
};

struct Light {
	//point light
	vec3 position;
	//attenuation constants
	float constant;
	float linear;
	float quadratic;

	//directional light
	vec3 direction;
	
	vec3 color;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 fNormal;
in vec3 fPos;
in vec2 fTexCoord;

uniform bool usePosLight;
uniform Material material;
uniform Light light;

void main() {
	bool invertSpec = false;

	vec3 tDiffuse = texture(material.diffuse, fTexCoord).rgb;
	vec3 tSpecular = texture(material.specular, fTexCoord).rgb;
	vec3 tSpecInverse = tSpecular + (vec3(0.5) - tSpecular) * 2;

	vec3 lightToFrag = fPos - light.position;
	float lightToFragLength = length(lightToFrag);

	float attenuation = usePosLight ? 1.0 / (light.constant + light.linear * lightToFragLength + light.quadratic * lightToFragLength * lightToFragLength) : 1.0;

	//ambient
	vec3 ambient = material.ambient * tDiffuse * attenuation;

	//diffuse
	vec3 lightDir = usePosLight ? normalize(lightToFrag) : normalize(light.direction);

	float diffuseFactor = max(dot(fNormal, -lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffuseFactor * tDiffuse * attenuation;

	//specular
	vec3 reflectDir = reflect(lightDir, fNormal);
	vec3 viewDir = normalize(-fPos);

	float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * specularFactor * (invertSpec ? tSpecInverse : tSpecular) * attenuation;

	//emission
	vec3 emission = material.intensity * texture(material.emission, fTexCoord).rgb;

	//result
	vec3 objectColor = (ambient + diffuse + specular) * light.color + emission;
	FragColor = vec4(objectColor, 1.0);
}