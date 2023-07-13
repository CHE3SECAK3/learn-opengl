#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <SHADER/shader.h>

#define SHADER_CLASS 1

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {
	#pragma region SETUP
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	float RGBvertex[] = {
		-.5f, -.5f, 0,		1, 0, 0,
		  0 ,  .5f, 0,		0, 1, 0,
		 .5f, -.5f, 0,		0, 0, 1,
	};

	unsigned int indices[] = {
		0, 1, 2,
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

	glBufferData(GL_ARRAY_BUFFER, sizeof(RGBvertex), RGBvertex, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
#pragma endregion

	#pragma region SHADER
	#if SHADER_CLASS
	Shader shader("shader/vertex.glsl", "shader/fragment.glsl");

	#else
	const char* vertexShaderSource, * fragmentShaderSource;
	vertexShaderSource = R"vss(#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 fColor;
void main() {
	gl_Position = vec4(aPos, 1.0f);
	fColor = aColor;
}
	)vss";

	fragmentShaderSource = R"fss(#version 330 core
in vec3 fColor;
out vec4 FragColor;
void main() {
	FragColor = vec4(fColor, 1.0f);
}
	)fss";

	unsigned int vShader, fShader, shaderProgram;
	vShader = glCreateShader(GL_VERTEX_SHADER);
	fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vertexShaderSource, NULL);
	glShaderSource(fShader, 1, &fragmentShaderSource, NULL);

	glCompileShader(vShader);
	glCompileShader(fShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vShader);
	glAttachShader(shaderProgram, fShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vShader);
	glDeleteShader(fShader);
#endif
#pragma endregion

	#pragma region RENDER LOOP

	#if SHADER_CLASS
	shader.use();

	#else
	glUseProgram(shaderProgram);
#endif

	while (!glfwWindowShouldClose(window)) {
			processInput(window);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			glBindVertexArray(VAO);
			/*shader.setFloat("xOffset", 0.0f);*/
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL);
			glBindVertexArray(0);

			glfwSwapBuffers(window);
			glfwPollEvents();
	}
#pragma endregion

	#pragma region CLEANUP
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	#if SHADER_CLASS
	shader.deleteShader();

	#else
	glDeleteProgram(shaderProgram);
#endif
		
	glfwTerminate();
#pragma endregion

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}