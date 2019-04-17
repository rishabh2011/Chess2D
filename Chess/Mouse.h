/* ----------------------------------------- 
   Class handles all mouse related functions
   ----------------------------------------- */
#pragma once
#ifndef MOUSE_H
#define MOUSE_H

#include <Shader.h>
#include "Files/Classes/Board.h"

class Mouse
{
public:
	//Calculates mouse cursor position
	static glm::vec2 calcMousePos(GLFWwindow* window)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		glm::vec2 position;
		position.x = x;
		position.y = y;
		return position;
	}

	//Gets color of the pixel at mouse cursor 
	static glm::vec3 getPixelColorUnderMouse(GLFWwindow* window, unsigned int colorBuffer)
	{
		glm::vec2 position = calcMousePos(window);
		int width, height;
		glfwGetWindowSize(window, &width, &height); //Returns current width and height of window
		GLubyte data[3];

		glBindFramebuffer(GL_FRAMEBUFFER, colorBuffer);
		//y position is from top to bottom hence reverse
		glReadPixels(position.x, height - position.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, data);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//Convert color values in opengl coordinates to 0-255 range 
		glm::vec3 color;
		color.r = static_cast<int>(data[0]);
		color.g = static_cast<int>(data[1]);
		color.b = static_cast<int>(data[2]);
		return color;
	}

	//Processes mouse input
	static void processMouse(GLFWwindow* window)
	{
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			if (!mouseAlreadyClicked)
			{
				mouseClicked = true;
				mouseAlreadyClicked = true;
			}
			else
			{
				mouseClicked = false;
			}
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			//Set mouse clicked to false
			mouseAlreadyClicked = false;
		}
	}

	//------------------------------------
	//Getter functions for class variables
	static bool getMouseClicked()
	{
		return mouseClicked;
	}

private:
	static bool mouseClicked;
	static bool mouseAlreadyClicked;
};

bool Mouse::mouseClicked = false;
bool Mouse::mouseAlreadyClicked = false;

#endif
