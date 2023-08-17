#version 330 core

struct Material {
	vec3 ambient;
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
	float shininess;
};

#define L_DIRECTIONAL	0
#define L_POINT			1
#define L_SPOT			2

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

in vec3 Pos;
in vec3 Norm;
in vec2 TCoord;

uniform Material model;
uniform bool useSpecular;

#define NUM_LIGHTS 1
uniform Light lights[NUM_LIGHTS];

vec3 calculateAmbientLight(Light light, Material material, vec3 ambientMap) {
	return material.ambient * light.ambient * ambientMap;
}

vec3 calculateDiffuseLight(Light light, vec3 lightDirection, vec3 diffuseMap, float attenuation) {
	float lightToFragDot = dot(Norm, -lightDirection);
	float diffuseFactor = max(lightToFragDot, 0.0);
	
	return diffuseMap * diffuseFactor * light.diffuse * attenuation;
}

vec3 calculateSpecularLight(Light light, vec3 lightDirection, Material material, vec3 specularMap, float attenuation) {
	if (!useSpecular) return vec3(0);

	vec3 reflectDir = reflect(lightDirection, Norm);
	vec3 viewDir = normalize(Pos);
	float specularFactor = pow(max(dot(viewDir, -reflectDir), 0.0), material.shininess);
	
	return specularMap * specularFactor * light.specular * attenuation;
}

vec3 calculateDirectionalLight(Light light, Material material, vec3 tDiffuse, vec3 tSpecular) {

	vec3 lightDir = normalize(light.direction);
	
	vec3 ambient = calculateAmbientLight(light, material, tDiffuse);
	vec3 diffuse = calculateDiffuseLight(light, lightDir, tDiffuse, 1.0);	
	vec3 specular = calculateSpecularLight(light, lightDir, material, tSpecular, 1.0);

	return (ambient + diffuse + specular) * light.color;
}

vec3 calculatePointLight(Light light, Material material, vec3 tDiffuse, vec3 tSpecular) {

	vec3 lightToFrag = Pos - light.position;
	vec3 lightDir = normalize(lightToFrag);
	float lightToFragLength = length(lightToFrag);
	float attenuation = 1.0 / (light.constant + light.linear * lightToFragLength + light.quadratic * lightToFragLength * lightToFragLength);
	
	float spotIntensity = 1.0;
	if (light.type == L_SPOT) {
		float spotLightDot = dot(normalize(light.direction), lightDir);
		spotIntensity = clamp( (spotLightDot - light.outerConeAngle) / (light.innerConeAngle - light.outerConeAngle) , 0.0, 1.0);
	}

	vec3 ambient	= calculateAmbientLight(light, material, tDiffuse);
	vec3 diffuse	= calculateDiffuseLight(light, lightDir, tDiffuse, attenuation);
	vec3 specular	= calculateSpecularLight(light, lightDir, material, tSpecular, attenuation);

	return (ambient + diffuse + specular) * light.color * spotIntensity;
}

void main() {

	vec3 objectColor = vec3(0);
	vec3 tDiffuse = texture(model.texture_diffuse0, TCoord).rgb;
	vec3 tSpecular = texture(model.texture_specular0, TCoord).rgb;
	
	for (int i = 0; i < NUM_LIGHTS; i++) {

		switch (lights[i].type) {
			case L_DIRECTIONAL:
				objectColor += calculateDirectionalLight(lights[i], model, tDiffuse, tSpecular);
				break;

			case L_SPOT:
			case L_POINT:
				objectColor += calculatePointLight(lights[i], model, tDiffuse, tSpecular);
				break;
			
			default:
				break;
		}
	}

	//result
	FragColor = vec4(objectColor, 1.0);
}