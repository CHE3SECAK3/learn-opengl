#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SHADER/shader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600


typedef unsigned char BYTE;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
int processInput(GLFWwindow* window);

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

	float vertices[] = {
		// vertex coords	// colors	// texture coords
		-.5f, -.5f, 0,		1, 0, 0,	0, 0,
		-.5f,  .5f, 0,		0, 1, 0,	0, 2,
		 .5f,  .5f, 0,		0, 0, 1,	2, 2,
		 .5f, -.5f, 0,		1, 1, 0,	2, 0,

	};

	unsigned int indices[] = {
		0, 1, 2,
		0, 2, 3,
	};
#pragma endregion

#pragma region PIPELINE
	unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
#pragma endregion

#pragma region TEXTURE AND SHADER
	unsigned int textures[2];
	int width, height, nChannels;
	BYTE *data;

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


	Shader shader("vertex.glsl", "fragment.glsl");
	shader.use();
	shader.setInt("t1", 0);
	shader.setInt("t2", 1);


#pragma endregion

#pragma region RENDER LOOP

	float ratio = 0.0f;
	shader.setFloat("ratio", ratio);
	while (!glfwWindowShouldClose(window)) {
		int process = processInput(window);
		if (process == 1) {
			ratio += 0.001f;
			shader.setFloat("ratio", (ratio = std::min(ratio, 1.0f)));
		}
		else if (process == 2) {
			ratio -= 0.001f;
			shader.setFloat("ratio", (ratio = std::max(ratio, 0.0f)));
		}

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(VAO);
		shader.setFloat("xOffset", 0.0f);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
#pragma endregion

#pragma region CLEANUP
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	shader.deleteShader();

	glfwTerminate();
#pragma endregion

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

int processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
		return -1;
	}

	else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		return 1;
	}

	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		return 2;
	}

	return 0;
}