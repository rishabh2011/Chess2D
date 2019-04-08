/* ----------------
   Class plays chess
   ----------------- */
#ifndef GAME_H
#define GAME_H

#include "Pawn.h"
#include "Rook.h"
#include "Knight.h"
#include "Bishop.h"
#include "Queen.h"
#include "King.h"
#include "Board.h"
#include <Mouse.h>

class Game
{
public:
	
	//Constructor
	Game()
	{
		board = new Board;
		board->loadTexture();
		initializePieces();
	}

	//Starts game rendering loop
	void renderLoop(GLFWwindow* window) 
	{
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
		
		while (!glfwWindowShouldClose(window))
		{
			float currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;
			processInput(window);

			glClearColor(0.1, 0.1, 0.1, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			//Render
			glStencilMask(0x00);
			board->playGame(pieces, deltaTime, window);
			glStencilMask(0xFF);
						
			glfwSwapBuffers(window);
			glfwPollEvents();
		}

		glfwTerminate();
	}
	
	~Game()
	{
		delete board;
	}
		
private:

	//---------------
	//Class Variables
	Board* board;
	std::vector<Pieces*>pieces;
	//Framerate variables
	float lastFrame = 0.0f;
	float deltaTime = 0.0f;
		
	//Process user inputs
	void processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		Mouse::processMouse(window);
	}
		
	void initializePieces()
	{
		pieces.push_back(new Pawn());
		pieces.push_back(new Rook());
		pieces.push_back(new Knight());
		pieces.push_back(new Bishop());
		pieces.push_back(new Queen());
		pieces.push_back(new King());
	}
};

#endif
