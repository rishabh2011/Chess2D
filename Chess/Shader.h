#pragma once

#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

const unsigned int SCR_WIDTH = 700;
const unsigned int SCR_HEIGHT = 700;

class Shader
{
public:
	Shader(const GLchar* vertexPath, const GLchar* fragPath, const GLchar* geoPath = nullptr)
	{
		std::string vStringCode;
		std::string fStringCode;
		std::string gStringCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragPath);
			
			std::ostringstream vShaderStream, fShaderStream, gShaderStream;
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			vShaderFile.close();
			fShaderFile.close();
			
			vStringCode = vShaderStream.str();
			fStringCode = fShaderStream.str();

			if (geoPath)
			{
				gShaderFile.open(geoPath);
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				gStringCode = gShaderStream.str();
			}
		}
		catch (std::ifstream::failure e)
		{
			std::cout << "ERROR WHILE READING SHADER FILE" << e.what() << std::endl;
		}

		const char* vCStringCode = vStringCode.c_str();
		const char* fCStringCode = fStringCode.c_str();
		const char* gCStringCode;

		if (geoPath)
		{
			gCStringCode = gStringCode.c_str();
		}
		int success;
		char infoLog[512];

		unsigned int vertexShader, fragShader, geoShader;

		vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vCStringCode, NULL);
		glCompileShader(vertexShader);
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR WHILE COMPILING VERTEX SHADER :\n" << infoLog << std::endl;
		}

		fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, &fCStringCode, NULL);
		glCompileShader(fragShader);
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
			std::cout << "ERROR WHILE COMPILING FRAGMENT SHADER :\n" << infoLog << std::endl;
		}

		if (geoPath)
		{
			geoShader = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geoShader, 1, &gCStringCode, NULL);
			glCompileShader(geoShader);
			glGetShaderiv(geoShader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(geoShader, 512, NULL, infoLog);
				std::cout << "ERROR WHILE COMPILING GEOMETRY SHADER :\n" << infoLog << std::endl;
			}
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertexShader);
		glAttachShader(ID, fragShader);

		if (geoPath)
		{
			glAttachShader(ID, geoShader);
		}
		glLinkProgram(ID);
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cout << "ERROR WHILE LINKING PROGRAM :\n" << infoLog << std::endl;
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragShader);
		if (geoPath)
		{
			glDeleteShader(geoShader);
		}
	}

	void use()
	{
		glUseProgram(ID);
	}

	void setMat4(const GLchar* name, glm::mat4 value)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
	}

	void setMat4(std::string name, glm::mat4 value)
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	void setVec2(const std::string name, glm::vec2 value)
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
	}

	void setVec3(const GLchar* name, float x, float y)
	{
		glUniform2f(glGetUniformLocation(ID, name), x, y);
	}

	void setVec3(const std::string name, glm::vec3 value)
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
	}

	void setVec4(const std::string name, glm::vec4 value)
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
	}

	void setVec3(const GLchar* name, float x, float y, float z)
	{
		glUniform3f(glGetUniformLocation(ID, name), x, y, z);
	}

	void setFloat(const GLchar* name, float value)
	{
		glUniform1f(glGetUniformLocation(ID, name), value);
	}

	void setInt(const GLchar* name, int value)
	{
		glUniform1i(glGetUniformLocation(ID, name), value);
	}

	unsigned int ID;
};


#endif
