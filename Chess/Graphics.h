/*------------------------------------------------------------
  Class sets up shaders, VAOs, and performs graphics rendering
  ------------------------------------------------------------ */
#pragma once
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <Shader.h>
#include <ctime>
#include <random>
#include <stb_image.h>

  //Random number generator in the range 0-255
std::default_random_engine engine{ static_cast<unsigned int>(time(0)) };
std::uniform_int_distribution<unsigned int> randInt{ 0, 255 };

class Graphics
{
public:

	//Loads texture from given file in SRGB format
	static unsigned int loadTexture(std::string name, bool flip)
	{
		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_set_flip_vertically_on_load(flip);
		int width, height, nrChannels;
		unsigned char* data = stbi_load(name.c_str(), &width, &height, &nrChannels, NULL);
		GLenum dataFormat;
		GLenum internalFormat;
		if (nrChannels == 1)
		{
			dataFormat = internalFormat = GL_RED;
		}
		else if (nrChannels == 3)
		{
			dataFormat = GL_RGB;
			internalFormat = GL_SRGB;
		}
		else if (nrChannels == 4)
		{
			dataFormat = GL_RGBA;
			internalFormat = GL_SRGB_ALPHA;
		}
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "ERROR LOADING TEXTURE FILE : " << stbi_failure_reason() << std::endl;
			stbi_image_free(data);
		}
		return texture;
	}

	//Draws chess pieces
	static void drawBoard(Shader* shader, const unsigned int* texture, const std::vector<glm::vec2> &positions, int startIndex, int endIndex, float scale, bool highlight = false,
		const glm::vec3& color = glm::vec3(1.0, 1.0, 1.0), const std::vector<glm::vec3> &colors = std::vector<glm::vec3>())
	{
		shader->use();
		shader->setFloat("scale", scale);
		glBindVertexArray(VAO);
		glBindTexture(GL_TEXTURE_2D, *texture);
		for (int i = startIndex; i <= endIndex; i++)
		{
			if (highlight)
			{
				shader->setVec3("color", color);
			}
			else if (colors.size() != 0)
			{
				glm::vec3 color;
				color = colors[i] / 255.0f;
				shader->setVec3("color", color);
			}
			shader->setVec2("offset", positions[i]);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		glBindVertexArray(0);
	}

	//Sets up chess board shaders and VAO
	static void setShadersAndVAO()
	{
		boardShader = new Shader("Files/Shaders/boardVertexShader.vs", "Files/Shaders/boardFragShader.fs");
		piecesColorShader = new Shader("Files/Shaders/boardVertexShader.vs", "Files/Shaders/piecesColorFragShader.fs");
		setupVAO(0.125);
	}

	//Generate a random number between 0-255 and store them in color array
	static glm::vec3 genRandomColor()
	{
		return glm::vec3(randInt(engine), randInt(engine), randInt(engine));
	}

	//----------------
	//Getter functions
	static Shader* getBoardShader() 
	{
		return boardShader;
	}

	static Shader* getPiecesColorShader() 
	{
		return piecesColorShader;
	}

	static unsigned int getVAO()
	{
		return VAO;
	}

	~Graphics()
	{
		delete boardShader;
		delete piecesColorShader;
	}
	
private:

	//Shader variables
	static Shader* boardShader;
	static Shader* piecesColorShader;
	
	static unsigned int VAO;

	//Sets up board VAO 
	static void setupVAO(float scale)
	{
		float vertices[] =
		{
			-scale, -scale, 0.0, 0.0,
			 scale, -scale, 1.0, 0.0,
			 scale,  scale, 1.0, 1.0,
			 scale,  scale, 1.0, 1.0,
			-scale,  scale, 0.0, 1.0,
			-scale, -scale, 0.0, 0.0,
		};
		unsigned int VBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}
};

Shader* Graphics::boardShader = nullptr;
Shader* Graphics::piecesColorShader = nullptr;
unsigned int Graphics::VAO = 0;

#endif
