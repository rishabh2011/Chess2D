/* ----------------
   Class plays chess
   ----------------- */
#ifndef GAME_H
#define GAME_H

#include "Pieces/Pawn/Pawn.h"
#include "Pieces/Rook/Rook.h"
#include "Pieces/Knight/Knight.h"
#include "Pieces/Bishop/Bishop.h"
#include "Pieces/Queen/Queen.h"
#include "Pieces/King/King.h"
#include "Board.h"
#include <Mouse.h>

class Game
{
public:
	
	//Constructor
	Game()
	{
		board = new Board;
		initializePieces();
	}


	//Starts game rendering loop
	void renderLoop(GLFWwindow* window) 
	{
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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
		for (size_t i{ 0 }; i < pieces.size(); i++)
		{
			delete pieces[i];
		}
	}
		
private:

	//---------------
	//Class Variables
	Board* board;
	std::vector<Pieces*>pieces;
	//Framerate variables
	float lastFrame = 0.0f;
	float deltaTime = 0.0f;

	bool zPressed{ false };
	//Process user inputs
	void processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		{
			if (!zPressed)
			{
				Board::restorePreviousMove(pieces);
				zPressed = true;
			}
		}
		else if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_RELEASE)
		{
			zPressed = false;
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
		pieces.shrink_to_fit();
	}
};

#endif
