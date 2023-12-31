#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SHADER/shader.h>
#include <CAMERA/camera.h>
#include <VERTEXARRAY/VertexArray.h>
#include <ARRAYBUF/VertexBuffer.h>
#include <ARRAYBUF/ElementBuffer.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#define SCREEN_WIDTH 800.0f
#define SCREEN_HEIGHT 600.0f
#define ASPECT_RATIO SCREEN_WIDTH / SCREEN_HEIGHT
#define PI 3.141592653f

typedef unsigned char BYTE;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

Camera camera;

float lastX = SCREEN_WIDTH / 2, lastY = SCREEN_HEIGHT / 2;
float deltaTime = 0.0f;
bool firstMouseCallback = true;

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
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f),
		//glm::vec3(2.0f, 5.0f, -15.0f),
		//glm::vec3(-1.5f, -2.2f, -2.5f),
		//glm::vec3(-3.8f, -2.0f, -12.3f),
		//glm::vec3(2.4f, -0.4f, -3.5f),
		//glm::vec3(-1.7f, 3.0f, -7.5f),
		//glm::vec3(1.3f, -2.0f, -2.5f),
		//glm::vec3(1.5f, 2.0f, -2.5f),
		//glm::vec3(1.5f, 0.2f, -1.5f),
		//glm::vec3(-1.3f, 1.0f, -1.5f)
	};

	glm::vec3 lightPos(0.0f, -1.0f, 0.0f);
	glm::vec3 shapePos(0.0f, -1.0f, -2.0f);

#pragma endregion

#pragma region VBUFS

	VertexArray va[2];
	VertexBufferLayout layout;
	VertexBuffer cube(vertices, sizeof(vertices));

	//local coordinates
	layout.DefineVertexAttribute(3, GL_FLOAT, GL_FALSE);
	//normals
	layout.DefineVertexAttribute(3, GL_FLOAT, GL_FALSE);

	for (int i = 0; i < 2; i++) {
		va[i].Bind();
		va[i].AddVertexAttributes(cube, layout);
		va[i].Unbind();
	}

#pragma endregion

#pragma region SHADER AND LIGHTING
	constexpr glm::vec3 yellowLight(1.0f, 0.98f, 0.82f);
	glm::vec3 objectColor(1.0f, 0.5f, 0.31f);
	glm::vec3 lightColor = yellowLight;

	Shader objectShader("../shader/lighting/modelviewvertex.glsl", "../shader/lighting/modelviewfragment.glsl");
	Shader lightShader("../shader/lighting/lightsourcevertex.glsl", "../shader/lighting/lightsourcefragment.glsl");

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

	float radius = 2.0f;
	float rotateRate = 0.5f;

	while (!glfwWindowShouldClose(window)) {

		currentFrame	= glfwGetTime();
		deltaTime		= currentFrame - lastFrame;
		lastFrame		= currentFrame;
		timeElapsed		= currentFrame - startTime;

		processInput(window);

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(camera.getFOV()), ASPECT_RATIO, 0.1f, 100.0f);
		lightPos = shapePos + radius * glm::vec3(-sin(timeElapsed), cos(timeElapsed), 0.0f);

		va[0].Bind();
		objectShader.use();

		model = glm::mat4(1.0f);
		model = glm::translate(model, shapePos);
		model = glm::rotate(model, rotateRate * timeElapsed, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 0.15f, 2.0f));
		modelView = view * model;
		normalMatrix = glm::transpose(glm::inverse(modelView));

		objectShader.setMat4("MV", modelView);
		objectShader.setMat4("projection", projection);
		objectShader.setMat3("MVN", normalMatrix);
		
		objectShader.setVec3("lightPos", glm::vec3(view * glm::vec4(lightPos, 1.0f)));
		objectShader.setVec3("lightColor", lightColor);
		objectShader.setVec3("objectColor", objectColor);
		objectShader.setFloat("ambientFactor", 0.1f);
		objectShader.setFloat("specularFactor", 0.5f);
		objectShader.setFloat("specularShine", 32);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		va[0].Unbind();

		//lighting
		va[1].Bind();
		lightShader.use();

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, timeElapsed, glm::vec3(0.0f, 0.0f, 1.0f));
		
		lightShader.setMat4("MVP", projection * view * model);
		lightShader.setVec3("lightColor", lightColor);

		glDrawArrays(GL_TRIANGLES, 0, 36);
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

#pragma region INPUT

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
#pragma endregion