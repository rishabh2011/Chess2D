#include <iostream>
#include "Files/Classes/Game.h"

//----------------------------
//Functions initialize graphics libraries
void initLibraries();
void initGLFW();
GLFWwindow* createGLFWwindow();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//Output Windowasd
GLFWwindow* window;

int main()
{
	initLibraries();
	Graphics::setupSquaresColorBuffer();
	Graphics::setupPiecesColorBuffer();
	Game game;
	game.renderLoop(window);
	return 1;
}

void initLibraries()
{
	initGLFW();
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void initGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 8);
	window = createGLFWwindow();
}

GLFWwindow* createGLFWwindow()
{
	GLFWwindow*	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Chess", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	return window;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

