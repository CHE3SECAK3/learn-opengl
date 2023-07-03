#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char* vertexShaderSource = "#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	" gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

const char* fragmentShaderSource1 = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"FragColor = vec4(1.0f, 0.5f, 1.0f, 1.0f);\n"
	"}\0";

const char* fragmentShaderSource2 = "#version 330 core\n"
	"out vec4 FragColor;\n"
	"uniform float timeGradColor;\n"
	"void main()\n"
	"{\n"
	"FragColor = vec4(timeGradColor, 0.0f, 0.0f, 1.0f);\n"
	"}\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return 1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return 1;
	}
	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	float vertices[] = {
		-.5f, -.5f, 0.0f,
		-.5f, .5f, 0.0f,
		.5f, .5f, 0.0f,
		.5f, -.5f, 0.0f,
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	unsigned int VAO[2], VBO[2], EBO[2];
	unsigned int vertexShader, fragmentShader, shaderProgram[2];
	
	// shader
	{
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource1, NULL);
		glCompileShader(fragmentShader);

		shaderProgram[0] = glCreateProgram();
		glAttachShader(shaderProgram[0], vertexShader);
		glAttachShader(shaderProgram[0], fragmentShader);
		glLinkProgram(shaderProgram[0]);
		glDeleteShader(fragmentShader);

		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource2, NULL);
		glCompileShader(fragmentShader);

		shaderProgram[1] = glCreateProgram();
		glAttachShader(shaderProgram[1], vertexShader);
		glAttachShader(shaderProgram[1], fragmentShader);
		glLinkProgram(shaderProgram[1]);

		glDeleteShader(fragmentShader);
		glDeleteShader(vertexShader);
	}

	
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);

	for (int i = 0; i < 2; i++) {
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices) / 2, indices + i * 3, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int i = 0;

	int vertexColorLocation = glGetUniformLocation(shaderProgram[1], "timeGradColor");
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		glClearColor(.2f, .3f, .3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram[0]);
		glBindVertexArray(VAO[i++]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glUseProgram(shaderProgram[1]);
		float val = abs(sinf(glfwGetTime()));
		glUniform1f(vertexColorLocation, val);
		glBindVertexArray(VAO[i--]);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(2, VAO);
	glDeleteBuffers(2, VBO);
	glDeleteProgram(shaderProgram[0]);
	glDeleteProgram(shaderProgram[1]);

	glfwTerminate();
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