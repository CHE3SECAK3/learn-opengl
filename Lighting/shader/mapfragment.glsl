#version 330 core

struct Material {
	vec3 ambient;
	sampler2D diffuse;
	sampler2D specular;
	float shininess;
	sampler2D emission;
	float intensity;
};

int L_DIRECTIONAL	= 0;
int L_POINT			= 1;
int L_SPOT			= 2;
int L_PLANE			= 3;

struct Light {
	int type;

	//directional
	vec3 direction;

	//point/spot
	vec3 position;

	//spot
	float innerConeAngle;
	float outerConeAngle;

	//attenuation constants
	float constant;
	float linear;
	float quadratic;

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

	vec3 lightToFrag = fPos - light.position;
	vec3 lightDir = light.type == L_DIRECTIONAL ? normalize(light.direction) : normalize(lightToFrag);
	float lightToFragDot = dot(fNormal, -lightDir);

	//ambient
	vec3 tDiffuse = texture(material.diffuse, fTexCoord).rgb;
	vec3 ambient = light.ambient * material.ambient * tDiffuse;
	
	//spot light
	float spotLightDot = dot(normalize(light.direction), lightDir);
	float intensity = light.type != L_SPOT ? 1.0 : clamp( (spotLightDot - light.outerConeAngle) / (light.innerConeAngle - light.outerConeAngle) , 0.0, 1.0);

	float lightToFragLength = length(lightToFrag);
	float attenuation = light.type == L_DIRECTIONAL ? 1.0 : 1.0 / (light.constant + light.linear * lightToFragLength + light.quadratic * lightToFragLength * lightToFragLength);

	//diffuse
	float diffuseFactor = max(lightToFragDot, 0.0);
	vec3 diffuse = light.diffuse * diffuseFactor * tDiffuse * attenuation;

	//specular
	vec3 tSpecular = texture(material.specular, fTexCoord).rgb;
	vec3 reflectDir = reflect(lightDir, fNormal);
	vec3 viewDir = normalize(-fPos);

	float specularFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * specularFactor * tSpecular * attenuation;

	//emission
	vec3 tEmission = texture(material.emission, fTexCoord).rgb;
	vec3 emission = material.intensity * tEmission.rgb;

	//result
	vec3 objectColor = (ambient + diffuse + specular) * light.color * intensity + emission;
	FragColor = vec4(objectColor, 1.0);
}