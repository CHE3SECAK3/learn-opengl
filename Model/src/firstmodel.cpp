#include <iostream>
#include <ostream>
#include <algorithm>
#include <vector>

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
#include <MODEL/model.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#define STB_IMAGE_IMPLEMENTATION
//#include <STB/stb_image.h>

#define TRIMESH 1

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

std::ostream& operator<<(std::ostream& stream, glm::vec3& vector) {
	return stream << "(" << vector.x << ", " << vector.y << ", " << vector.z << ")";
}

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

#pragma endregion

#pragma region DATA

	Model backpack("models/backpack/backpack.obj");
	Shader backpackShader("shader/bpvertex.glsl", "shader/bpfragment.glsl");
	Shader solidColorShader("shader/solidcolorvertex.glsl", "shader/solidcolorfragment.glsl");

#if TRIMESH
	std::vector<Vertex> vTri;
	Vertex v = { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f) };
	vTri.push_back(v);
	v = { glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f) };
	vTri.push_back(v);
	v = { glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec2(0.0f) };
	vTri.push_back(v);
	
	std::vector<unsigned int> iTri = { 0, 1, 2 };
	
	std::vector<Texture> tTri;
	
	Mesh tri(vTri, iTri, tTri);
#endif

#pragma endregion

#pragma region TRANSFORMATION MATRICES
	glm::mat4 model, view, modelview, projection;

#pragma endregion

#pragma region RENDER LOOP

	float	startTime		= glfwGetTime(),
			lastFrame		= 0.0f,
			currentFrame	= 0.0f,
			timeElapsed		= 0.0f;

	while (!glfwWindowShouldClose(window)) {
		currentFrame	= glfwGetTime();
		deltaTime		= currentFrame - lastFrame;
		lastFrame		= currentFrame;
		timeElapsed		= currentFrame - startTime;

		processInput(window);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_STENCIL_TEST);

		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		glClearColor(0.15f, 0.15f, 0.15f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(camera.getFOV()), ASPECT_RATIO, 0.1f, 100.0f);
		
		//- BACKPACK MODEL

		backpackShader.use();

		model = glm::mat4(1.0f);
		model = glm::rotate(model, timeElapsed, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		modelview = view * model;

		backpackShader.setMat4("M_modelview", modelview);
		backpackShader.setMat4("M_projection", projection);
		backpackShader.setMat3("M_normal", glm::mat3(glm::transpose(glm::inverse(modelview))));

		Light light;
		light.type = LIGHT_TYPE::L_SPOT;
		//light.position		= glm::vec3(modelview * glm::vec4(0.0f, 1.0f, 2.0f, 1.0f));
		//light.direction		= glm::normalize(glm::vec3(modelview * glm::vec4(1.0f, -1.0f, -1.0f, 0.0f)));
		light.position			= glm::vec3(0.0f);				// camera
		light.direction			= glm::vec3(0.0f, 0.0f, -1.0f);	// camera
		light.ambient			= glm::vec3(0.1f);
		light.diffuse			= glm::vec3(0.5f);
		light.specular			= glm::vec3(1.0f);
		light.constant			= 1;
		light.linear			= 0.22f;
		light.quadratic			= 0.2f;
		light.innerConeAngle	= glm::cos(glm::radians(10.0f));
		light.outerConeAngle	= glm::cos(glm::radians(15.0f));
		light.color				= YELLOW_LIGHT;

		backpackShader.setLight("lights[0]", light);
		backpackShader.setInt("backpack.shininess", 64);

		solidColorShader.use();
		model = glm::mat4(1.0f);
		model = glm::rotate(model, timeElapsed, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(0.21f));

		solidColorShader.setMat4("M_MVP", projection * view * model);
		solidColorShader.setVec3("color", glm::vec3(0.2f, 0.05f, 0.65f));

		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LESS);

		//glEnable(GL_STENCIL_TEST);
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		//glStencilMask(0xFF);
		//backpack.Draw(backpackShader, "backpack");
		//
		//glDisable(GL_DEPTH_TEST);
		//glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		//backpack.Draw(solidColorShader);

		backpack.Draw(backpackShader, "backpack", solidColorShader);
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

#pragma endregion

#pragma region CLEANUP
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
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) camera.processKeyboardInput(UP, sprintFactor);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) camera.processKeyboardInput(DOWN, sprintFactor);
}

glm::vec3 capcolor(glm::vec3 rgb) {
	return glm::vec3(std::min(rgb.r, 1.0f), std::min(rgb.g, 1.0f), std::min(rgb.b, 1.0f));
}

#pragma endregion