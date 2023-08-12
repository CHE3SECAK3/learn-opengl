#include <iostream>
#include <algorithm>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SHADER/shader.h>
#include <CAMERA/camera.h>
#include <VERTEXARRAY/VertexArray.h>
#include <ARRAYBUF/VertexBuffer.h>
#include <ARRAYBUF/ElementBuffer.h>
#include <TEXTURE/texture.h>
#include <LIGHT/light.h>
#include <MATERIAL/material.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#define STB_IMAGE_IMPLEMENTATION
//#include <STB/stb_image.h>

constexpr float PI = 3.141592653f;

constexpr float SCREEN_WIDTH = 800.0f;
constexpr float SCREEN_HEIGHT = 600.0f;
constexpr float ASPECT_RATIO = SCREEN_WIDTH / SCREEN_HEIGHT;

constexpr glm::vec3 YELLOW_LIGHT(1.0f, 0.98f, 0.82f);

typedef unsigned char BYTE;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
glm::vec3 capcolor(glm::vec3 rgb);

Camera camera;

float lastX = SCREEN_WIDTH / 2, lastY = SCREEN_HEIGHT / 2;
float deltaTime = 0.0f;
bool firstMouseCallback = true;

bool Texture::ActiveTextureUnits[] = { false };

int main() {

#pragma region SETUP

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	glEnable(GL_DEPTH_TEST);

	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f, 3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f, 2.0f, -2.5f),
		glm::vec3(1.5f, 0.2f, -1.5f),
		glm::vec3(-1.3f, 1.0f, -1.5f)
	};
	

	constexpr int NUM_LIGHTS = 3;
	glm::vec3 lightPos[NUM_LIGHTS] = {
		glm::vec3(0.0f, 0.0f, 2.0f),
		glm::vec3(0.0f, 0.0f, -2.0f),
		glm::vec3(0.0f),
	};

#pragma endregion

#pragma region VBUFS

	VertexArray va[2];
	VertexBufferLayout layout;
	VertexBuffer cube(vertices, sizeof(vertices));

	layout.DefineVertexAttribute();

	for (int i = 0; i < 2; i++) {
		va[i].Bind();
		va[i].AddVertexAttributes(cube, layout);
		va[i].Unbind();
	}

#pragma endregion

#pragma region SHADER AND TEXTURE 
	glm::vec3 objectColor = glm::vec3(0.4f, 0.27f, 0.0f);
	glm::vec3 lightColor = glm::vec3(1.0f);

	Shader objectShader("../shader/lighting/modelviewvertex.glsl", "../shader/lighting/multilightfragment.glsl");
	Shader lightShader("../shader/lighting/lightsourcevertex.glsl", "../shader/lighting/lightsourcefragment.glsl");

	Texture diffuseMap("../images/container2.png", TextureType::DIFFUSE);
	Texture specularMap("../images/container2_specular.png", TextureType::SPECULAR);
	Texture emissionMap("../images/matrix.jpg", TextureType::EMISSION);

#pragma endregion

#pragma region TRANSFORMATION MATRICES
	glm::mat4 model, view, modelView, projection;
	glm::mat3 normalMatrix;

#pragma endregion

#pragma region RENDER LOOP
	float	startTime		= glfwGetTime(),
			lastFrame		= 0.0f,
			currentFrame	= 0.0f,
			timeElapsed		= 0.0f;

	float radius = 1.5f;
	float rotateRate = 0.5f;
	int nCubes = sizeof(cubePositions) / sizeof(glm::vec3);

	bool EMIT = false;

	while (!glfwWindowShouldClose(window)) {

		currentFrame	= glfwGetTime();
		deltaTime		= currentFrame - lastFrame;
		lastFrame		= currentFrame;
		timeElapsed		= currentFrame - startTime;

		processInput(window);

		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(camera.getFOV()), ASPECT_RATIO, 0.1f, 100.0f);

		va[0].Bind();
		objectShader.use();
		
		objectShader.setMat4("projection", projection);

		Light lights[NUM_LIGHTS];
		Material material;

		lights[0].type = LIGHT_TYPE::L_POINT;

		lights[0].position = glm::vec3(view * glm::vec4(lightPos[0], 1.0f));

		lights[0].constant = 1.0f;
		lights[0].linear = 0.09f;
		lights[0].quadratic = 0.032f;

		lights[0].color = glm::vec3(1.0f, 0.0f, 0.0f);
		lights[0].ambient = glm::vec3(0.1f);
		lights[0].diffuse = glm::vec3(0.8f);
		

		lights[1].type = LIGHT_TYPE::L_SPOT;

		lightPos[1] = camera.Position - camera.Direction;
		lights[1].position = glm::vec3(view * glm::vec4(lightPos[1] + camera.Direction, 1.0f));
		lights[1].direction = glm::vec3(view * glm::vec4(camera.Direction, 0.0f));

		lights[1].innerConeAngle = glm::cos(glm::radians(10.0f));
		lights[1].outerConeAngle = glm::cos(glm::radians(12.5f));

		lights[1].constant = 1.0f;
		lights[1].linear = 0.09f;
		lights[1].quadratic = 0.032f;

		lights[1].color = glm::vec3(0.0f, 1.0f, 0.0f);
		lights[1].ambient = glm::vec3(0.1f);
		lights[1].diffuse = glm::vec3(0.8f);
		lights[1].specular = glm::vec3(1.0f);
		lights[0].specular = glm::vec3(1.0f);
		

		lights[2].type = LIGHT_TYPE::L_DIRECTIONAL;

		lights[2].direction = glm::vec3(view * glm::vec4(-1.0f, 0.0f, 0.0f, 0.0f));
			   
		lights[2].color = glm::vec3(0.0f, 0.0f, 1.0f);
		lights[2].ambient = glm::vec3(0.1f);
		lights[2].diffuse = glm::vec3(0.8f);
		lights[2].specular = glm::vec3(1.0f);


		material.ambient = glm::vec3(1.0f);
		material.diffuse = diffuseMap.getTextureUnit();
		material.specular = specularMap.getTextureUnit();
		material.shininess = 32.0f;
		material.emission = emissionMap.getTextureUnit();
		material.emissionIntensity = 0.0f;

		
		objectShader.setLight("lights[0]", lights[0]);
		objectShader.setLight("lights[1]", lights[1]);
		objectShader.setLight("lights[2]", lights[2]);
		objectShader.setMaterial("boxMaterial", material);
		
		diffuseMap.Bind();
		specularMap.Bind();
		emissionMap.Bind();
		
		for (int i = 0; i < nCubes; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, i * 2 * PI / nCubes + rotateRate * timeElapsed, glm::vec3(1.0f, 1.0f, -1.0f));
			modelView = view * model;
			normalMatrix = glm::transpose(glm::inverse(modelView));

			objectShader.setMat4("MV", modelView);
			objectShader.setMat3("MVN", normalMatrix);
			
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}		
		va[0].Unbind();

		
		//lighting
		va[1].Bind();
		lightShader.use();

		for (int i = 0; i < NUM_LIGHTS; i++) {
			if (lights[i].type != LIGHT_TYPE::L_DIRECTIONAL && lightPos[i] != camera.Position - camera.Direction) {
				model = glm::mat4(1.0f);
				model = glm::translate(model, lightPos[i]);
				model = glm::scale(model, glm::vec3(0.25f));

				lightShader.setMat4("MVP", projection * view * model);
				lightShader.setVec3("lightColor", lights[i].color);

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}
		}

		va[1].Unbind();
		
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
#pragma endregion

#pragma region CLEANUP
	va[0].~VertexArray();
	va[1].~VertexArray();
	cube.~VertexBuffer();
	objectShader.~Shader();
	lightShader.~Shader();

	glfwTerminate();
#pragma endregion

	return 0;
}

#pragma region AUX

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	float xoffset, yoffset;

	if (firstMouseCallback) {
		xoffset = yoffset = 0;
		firstMouseCallback = false;
	}
	else {
		xoffset = xpos - lastX;
		yoffset = lastY - ypos; // screen y-axis is reversed
	}

	lastX = xpos;
	lastY = ypos;

	camera.processMouseMovementInput(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.processScrollwheelInput(yoffset);
}

void processInput(GLFWwindow* window) {
	float sprintFactor = deltaTime * (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 2.5f : 1);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) camera.processKeyboardInput(SPEED_UP, sprintFactor);
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) camera.processKeyboardInput(SLOW_DOWN, sprintFactor);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.processKeyboardInput(FORWARD, sprintFactor);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.processKeyboardInput(BACKWARD, sprintFactor);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.processKeyboardInput(LEFT, sprintFactor);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.processKeyboardInput(RIGHT, sprintFactor);
}

glm::vec3 capcolor(glm::vec3 rgb) {
	return glm::vec3(std::min(rgb.r, 1.0f), std::min(rgb.g, 1.0f), std::min(rgb.b, 1.0f));
}

#pragma endregion