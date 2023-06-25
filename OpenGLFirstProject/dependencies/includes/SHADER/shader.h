#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class Shader {
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath) {
		std::string vertexShaderString, fragmentShaderString;
		
		#pragma region SHADER CODE INIT
		std::ifstream vFile, fFile;
		vFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			vFile.open(vertexPath);
			fFile.open(fragmentPath);

			std::stringstream vStream, fStream;
			vStream << vFile.rdbuf();
			fStream << fFile.rdbuf();

			vertexShaderString = vStream.str();
			fragmentShaderString = fStream.str();

			vFile.close();
			fFile.close();

		}
		catch (std::ifstream::failure e) {
			std::cout << "SHADER FILE READ FAIL" << std::endl;
		}
#pragma endregion

		#pragma region SHADER COMPILE

		unsigned int vertexShader, fragmentShader;
		int success;
		char msg[512];
		const char* vcodestr, *fcodestr; 
		vcodestr = vertexShaderString.c_str();
		fcodestr = fragmentShaderString.c_str();
		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vcodestr, NULL);
		glCompileShader(vertexShader);

		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertexShader, 512, NULL, msg);
			std::cout << "VERTEX SHADER COMPILATION FAIL:\n" << msg << std::endl;
		}
		
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fcodestr, NULL);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragmentShader, 512, NULL, msg);
			std::cout << "FRAGMENT SHADER COMPILATION FAIL:\n" << msg << std::endl;
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragmentShader);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, msg);
			std::cout << "SHADER PROGRAM LINKING FAIL:\n" << msg << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

#pragma endregion
	}

	void use() {
		glUseProgram(ID);
	}

	void deleteShader() {
		glDeleteProgram(ID);
	}

	#pragma region SETTERS
	void setBool(const std::string &name, bool value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setInt(const std::string &name, int value) const {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string &name, float value) const {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
#pragma endregion
};

#endif
