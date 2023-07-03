#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SHADER/shader.h>

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
float clip(float value, float offset, float min, float max);

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f),
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f),
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f, lastFrame = 0.0f;
float cameraSpeed = 3.0f;
float lastX = SCREEN_WIDTH / 2, lastY = SCREEN_HEIGHT / 2;

glm::vec3 direction = glm::vec3(0.0f);
float pitch = 0.0f, yaw = 0.0f;

float fov = 45.0f;

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
	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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

		data = stbi_load(i == 0 ? "images/container.jpg" : "images/awesomeface.png", &width, &height, &nChannels, 0);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, i == 0 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glGenerateMipmap(textures[i]);
		stbi_image_free(data);
	}


	Shader shader("shader/cubevertex.glsl", "shader/cubefragment.glsl");
	shader.use();
	shader.setInt("t1", 0);
	shader.setInt("t2", 1);

#pragma endregion

	#pragma region TRANSFORMATION
	glm::mat4 model, view, projection;


#pragma endregion

	#pragma region CAMERA

	//glm::vec3 camera = glm::vec3(0.0f, 0.0f, 3.0f);
	//glm::vec3 cameraTarget = glm::vec3(0.0f);
	//glm::vec3 cameraPositiveZ = glm::normalize(camera - cameraTarget); // vector in direction of CAMERA'S positive z-axis (camera is pointed towards the negative z-axis)
	//glm::vec3 cameraUp = glm::vec3(0.0f, 0.0f, 1.0f);
	//glm::vec3 cameraRight = glm::cross(cameraUp, cameraPositiveZ);
	//glm::vec3 cameraRight = glm::cross(cameraUp, cameraPositiveZ);
	
#pragma endregion

#pragma region RENDER LOOP

	const float startTime = glfwGetTime();
	float timeElapsed, currentFrame;

	float rotateRate = 1.0f;
	int numCubes = sizeof(cubePositions) / sizeof(glm::vec3);
	float rotationOffset = 2 * PI / numCubes;

	float radius, camX, camZ;

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


		radius = 10.0f;
		camX = cos(timeElapsed) * radius;
		camZ = sin(timeElapsed) * radius;
		view = glm::mat4(1.0f);
		//view = glm::lookAt(glm::vec3(camX, 0.0f, camZ), glm::vec3(0.0f), cameraUp); //rotating camera

		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		projection = glm::perspective(glm::radians(fov), ASPECT_RATIO, 0.1f, 100.0f);

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
	glDeleteBuffers(1, &VBO);

	glfwTerminate();
#pragma endregion

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos; //screen y-axis is reversed

	lastX = xpos;
	lastY = ypos;

	const float sens = 0.075f;
	xOffset *= sens;
	yOffset *= sens;

	yaw += xOffset;
	pitch = clip(pitch, yOffset, -89.0f, 89.0f);

	direction.y = sin(glm::radians(pitch));
	direction.x = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	direction.z = cos(glm::radians(pitch)) * -cos(glm::radians(yaw));
	cameraFront = glm::normalize(direction);
}

float clip(float pitch, float offset, float min, float max) {
	float sum = pitch + offset;
	return sum < min ? min : sum > max ? max : sum;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	fov = clip(fov, -2 * (float)yoffset, 1.0f, 45.0f);
}

int processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { glfwSetWindowShouldClose(window, true); return -1; }

	else if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) return 1;
	else if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) return 2;

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) cameraSpeed += 0.25f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) cameraSpeed = std::max(0.0f, cameraSpeed - 0.25f);

	float cameraSpeedDelta = cameraSpeed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) cameraPos += cameraSpeedDelta * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) cameraPos -= cameraSpeedDelta * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeedDelta;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeedDelta;

	return 0;
}