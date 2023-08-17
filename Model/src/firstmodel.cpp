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

enum toRGB {
	R = 0x1,
	G = 0x2,
	B = 0x4
};

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
void setOutlineColor(const std::vector<int>& stateList, int& state, glm::vec3& color);

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
#define TEST 0
#if TEST
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

	VertexBuffer vb(vertices, sizeof(vertices));
	VertexBufferLayout vbl;
	vbl.DefineVertexAttribute();

	VertexArray va(vb, vbl);

	Texture diffuse("images/container2.png", DIFFUSE);
	diffuse.TextureUnit = 0;
	Texture specular("images/container2_specular.png", SPECULAR);
	specular.TextureUnit = 1;
#endif

	//Model backpack("models/backpack/backpack_sep_origins.obj");
	//Model test("models/testcube/testcube.obj");
	Model test("models/testcube/testcube1.obj");
	Shader modelShader("shader/modelvertex.glsl", "shader/modelfragment.glsl");
	Shader solidColorShader("shader/solidcolorvertex.glsl", "shader/solidcolorfragment.glsl");

	const std::vector<int> RGB_STATE = { R, R|G, G, G|B, B, R|B };
	int state = -1;
	glm::vec3 currentColor(0.0f);

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

		modelShader.use();

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.25f));
		//model = glm::rotate(model, 0.5f * timeElapsed, glm::vec3(0.0f, 1.0f, -1.0f));
		modelview = view * model;

		modelShader.setMat4("M_modelview", modelview);
		modelShader.setMat4("M_projection", projection);
		modelShader.setMat3("M_normal", glm::transpose(glm::inverse(modelview)));

		Light light;

		//light.type = LIGHT_TYPE::L_SPOT;
		//light.SetLightToCameraOrientation();
		light.type = LIGHT_TYPE::L_DIRECTIONAL;
		light.direction = view * glm::vec4(-1.0f, -1.0f, -1.0f, 0.0f);
		light.ambient			= glm::vec3(0.1f);
		light.diffuse			= glm::vec3(0.8f);
		light.specular			= glm::vec3(1.0f);
		light.SetAttenuation(1, 0.09f, 0.032f);
		light.SetConeAngles(10.0f, 12.5f);
		light.color				= YELLOW_LIGHT;

		modelShader.setLight("lights[0]", light);
		modelShader.setVec3("model.ambient", YELLOW_LIGHT);
		modelShader.setFloat("model.shininess", 32.0f);

		model = glm::mat4(1.0f);
		model = glm::scale(model, glm::vec3(0.26f));

		solidColorShader.use();
		solidColorShader.setMat4("M_MVP", projection * view * model);
		setOutlineColor(RGB_STATE, state, currentColor);
		solidColorShader.setVec3("color", currentColor);

#if TEST
		va.Bind();
		diffuse.Bind();
		specular.Bind();
		modelShader.setInt("model.texture_diffuse0", diffuse.TextureUnit);
		modelShader.setInt("model.texture_specular0", specular.TextureUnit);

		glDrawArrays(GL_TRIANGLES, 0, 36);
		va.Unbind();
#endif

		test.Draw(modelShader, "model", solidColorShader);
		//backpack.Draw(modelShader, "model", solidColorShader);
		
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

void setOutlineColor(const std::vector<int>& stateList, int& state, glm::vec3& color) {
	const float COLOR_CHANGE_DELTA = 0.001f;
	// R, R|G, G, G|B, B, R|B 
	switch (state) {
		case R:
			if (color.b <= 0.01f) {
				 
				color.b = 0.0f;
				state = R|G;
			}

			else {
				color.b -= COLOR_CHANGE_DELTA;
				break;
			}
		case R|G:
			if (color.g >= 0.99f) {
				 
				color.g = 1.0f;
				state = G;
			}

			else {
				color.g += COLOR_CHANGE_DELTA;
				break;
			}
		case G:
			if (color.r <= 0.01f) {
				 
				color.r = 0.0f;
				state = G|B;
			}

			else {
				color.r -= COLOR_CHANGE_DELTA;
				break;
			}
		case G|B:
			if (color.b >= 0.99f) {
				 
				color.b = 1.0f;
				state = B;
			}

			else {
				color.b += COLOR_CHANGE_DELTA;
				break;
			}
		case B:
			if (color.g <= 0.01f) {
				 
				color.g = 0.0f;
				state = R|B;
			}

			else {
				color.g -= COLOR_CHANGE_DELTA;
				break;
			}
		case R|B:
			if (color.r >= 0.99f) {
				 
				color.r = 1.0f;
				state = R;
			}

			else {
				color.r += COLOR_CHANGE_DELTA;
				break;
			}
		default:
			 
			color = glm::vec3(1, 0, 0);
			state = R;
	}
}

#pragma endregion