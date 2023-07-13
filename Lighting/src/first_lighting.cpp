#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SHADER/shader.h>
#include <CAMERA/camera.h>
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
int processInput(GLFWwindow* window);
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
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
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

#pragma endregion

#pragma region PIPELINE
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	VertexBuffer vb(vertices, sizeof(vertices));

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
#pragma endregion

#pragma region TEXTURE SHADER TRANSFORMATION
	unsigned int textures[2];
	int width, height, nChannels;
	BYTE* data;

	glGenTextures(2, textures);
	stbi_set_flip_vertically_on_load(true);
	for (int i = 0; i < 2; i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, textures[i]);

		data = stbi_load(i == 0 ? "../images/container.jpg" : "../images/awesomeface.png", &width, &height, &nChannels, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, i == 0 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glGenerateMipmap(textures[i]);
		stbi_image_free(data);
	}


	Shader shader("../shader/cubevertex.glsl", "../shader/cubefragment.glsl");
	shader.use();
	shader.setInt("t1", 0);
	shader.setInt("t2", 1);

#pragma endregion

#pragma region TRANSFORMATION AND CAMERA
	glm::mat4 model, view, projection;

#pragma endregion

#pragma region RENDER LOOP
	const float startTime = glfwGetTime();
	float lastFrame = 0.0f,
		currentFrame = 0.0f,
		timeElapsed = 0.0f;

	float rotateRate = 1.0f;
	int numCubes = sizeof(cubePositions) / sizeof(glm::vec3);
	float rotationOffset = 2 * PI / numCubes;

	float ratio = 0.0f;
	shader.setFloat("ratio", ratio);
	while (!glfwWindowShouldClose(window)) {

		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		timeElapsed = currentFrame - startTime;

		int process = processInput(window);
		if (process == 1) {
			ratio += 0.001f;
			shader.setFloat("ratio", (ratio = std::min(ratio, 1.0f)));
		}
		else if (process == 2) {
			ratio -= 0.001f;
			shader.setFloat("ratio", (ratio = std::max(ratio, 0.0f)));
		}

		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(camera.getFOV()), ASPECT_RATIO, 0.1f, 100.0f);

		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glBindVertexArray(VAO);

		for (int i = 0; i < numCubes; i++) {
			model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, rotationOffset + timeElapsed * rotateRate * (i + 1) / 4, glm::normalize(cubePositions[i] + glm::vec3(1.0f, 1.0f, 1.0f)));

			shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);

		}
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
#pragma endregion

#pragma region CLEANUP
	glDeleteVertexArrays(1, &VAO);
	vb.~VertexBuffer();

	glfwTerminate();
#pragma endregion

	return 0;
}

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

int processInput(GLFWwindow* window) {
	float sprintFactor = deltaTime * (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ? 2.5f : 1);
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); return -1; }

	else if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) return 1;
	else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) return 2;

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) camera.processKeyboardInput(SPEED_UP, sprintFactor);
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) camera.processKeyboardInput(SLOW_DOWN, sprintFactor);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) camera.processKeyboardInput(FORWARD, sprintFactor);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) camera.processKeyboardInput(BACKWARD, sprintFactor);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) camera.processKeyboardInput(LEFT, sprintFactor);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) camera.processKeyboardInput(RIGHT, sprintFactor);


	return 0;
}