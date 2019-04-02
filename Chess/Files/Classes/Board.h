//TODO pieceOnSquare values properly set on piece position changes during castling and en passant

/*-----------------------------------------------------------------------------------------
  Class sets up board squares position, handles mouse hits on pieces and draws entire board
  ----------------------------------------------------------------------------------------- */
#ifndef BOARD_H
#define BOARD_H

#include "Pieces.h"
#include <Mouse.h>
#include <vector>

 //------------
 //Enum classes
 //------------

//Piece status containing all possible states of a piece
enum class PieceStatus { NOT_SELECTED, SELECTED, MOVING };
//Player enum containing white and black
enum class Player { WHITE, BLACK };

class Board
{
public:

	//Enum variables
	static Player player;
	static PieceStatus status;
		
	//Square textures
	static unsigned int lightSquareTexture;
	static unsigned int darkSquareTexture;

	static float squareSizeX;
	static float squareSizeY;

	//Constructor
	//-----------
	Board()
	{
		setupSquarePositions();
	}

	void loadTexture()
	{
		lightSquareTexture = Graphics::loadTexture("Files/Textures/Board/square gray light.png", false);
		darkSquareTexture = Graphics::loadTexture("Files/Textures/Board/square gray dark.png", false);
	}

	//Draw pieces with color for mouse hit detection
	//----------------------------------------------
	void drawColor(std::vector<Pieces*> &pieces)
	{
		for (size_t i = 0; i < pieces.size(); i++)
		{
			pieces[i]->drawColor();
		}
	}

	//Perform various functions based on mouse click and mouse cursor position
	//------------------------------------------------------------------------
	void handleMouse(std::vector<Pieces*> &pieces, const glm::vec3 &color, float deltaTime, GLFWwindow* window)
	{
		draw(pieces);

		if (player == Player::WHITE)
		{
			if (!squaresSetup)
			{
				for (size_t i{ 0 }; i < pieces.size(); i++)
				{
					pieces[i]->setupPieceOnSquareValues();
				}
				for (size_t i{ 0 }; i < pieces.size(); i++)
				{
					pieces[i]->calcTargetSquares(false);
				}
				squaresSetup = true;
			}
			for (size_t i{ 0 }; i < pieces.size(); i++)
			{
				pieces[i]->drawKingInCheck(true);
			}
			switch (status)
			{
			case PieceStatus::NOT_SELECTED:
				drawPiecesOutline(pieces, &color, true);
				break;
			case PieceStatus::SELECTED:
				pieceSelected(pieces, window, true);
				break;
			case PieceStatus::MOVING:
				movePiece(pieces, window, selectedPieceIndex, deltaTime, true);
				break;
			}
		}
		else
		{
			if (!squaresSetup)
			{
				for (size_t i{ 0 }; i < pieces.size(); i++)
				{
					pieces[i]->setupPieceOnSquareValues();
				}
				for (size_t i{ 0 }; i < pieces.size(); i++)
				{
					pieces[i]->calcTargetSquares(true);
				}
				squaresSetup = true;
			}
			for (size_t i{ 0 }; i < pieces.size(); i++)
			{
				pieces[i]->drawKingInCheck(false);
			}
			switch (status)
			{
			case PieceStatus::NOT_SELECTED:
				drawPiecesOutline(pieces, &color, false);
				break;
			case PieceStatus::SELECTED:
				pieceSelected(pieces, window, false);
				break;
			case PieceStatus::MOVING:
				movePiece(pieces, window, selectedPieceIndex, deltaTime, false);
				break;
			}
		}
	}

	//Draw the target square that a piece can move to
	//-----------------------------------------------
	static void drawTargetSquare(glm::vec3& targetSquareColor, const glm::vec2 &targetSquare, GLFWwindow* window, Pieces* &enemyPiece, int targetPieceIndex, bool isWhite)
	{
		//Get the squareIndex of the currently selected targetSquare
		squareIndex = getSquareIndex(targetSquare);

		//If targetSquare is outside the board
		if (squareIndex == -1)
		{
			return;
		}

		//Draw the targetSquare with a color from the Pieces::squareColors array using the squareIndex value
		Graphics::drawBoard(Graphics::getPiecesColorShader(), &lightSquareTexture, Pieces::squarePositions, squareIndex, squareIndex, 1.0, false, glm::vec3(1.0, 1.0, 1.0), Pieces::squareColors);
		//If mouse is clicked
		if (Mouse::getMouseClicked())
		{
			//Get the color of the pixel under the mouse
			glm::vec3 color = Mouse::getPixelColorUnderMouse(window);
			Graphics::drawBoard(Graphics::getBoardShader(), getSquareTexture(squareIndex), Pieces::squarePositions, squareIndex, squareIndex, 1.0);
			//Get the index of the color from the Pieces::squareColors array that matches the pixel color
			int squareColorIndex = getSquareIndex(color);
			//If mouse clicked on invalid square
			if (squareColorIndex == -1)
			{
				return;
			}
			if (Pieces::squarePositions[squareColorIndex] == targetSquare)
			{
				status = PieceStatus::MOVING;
				piece->calcDifferenceBetweenSquares(targetSquare, selectedPieceIndex, isWhite);
				selectedTargetSquare = targetSquare;
			}
		}
		Graphics::drawBoard(Graphics::getPiecesColorShader(), &lightSquareTexture, Pieces::squarePositions, squareIndex, squareIndex, 1.0, true, targetSquareColor);
		//User doesn't want to see the colored square
		//Therefore draw the specific square with the proper texture again and then draw a small blue square on top to indicate a targetSquare
		Graphics::drawBoard(Graphics::getBoardShader(), getSquareTexture(squareIndex), Pieces::squarePositions, squareIndex, squareIndex, 0.95);
		//If the target square contains an enemy piece, draw that enemy piece over the square again
		if (enemyPiece)
		{
			enemyPiece->draw(targetPieceIndex, !isWhite);
		}
	}
	
	static void switchPlayer(const std::vector<Pieces*> &pieces)
	{
		squaresSetup = false;
		Pieces::kingAttacked = false;
		Pieces::pieceOnSquare.clear();
		Pieces::squaresAttacked.clear();
		//Set status to not selected
		status = PieceStatus::NOT_SELECTED;

		//Invert piece positions since both players should be able to play the game from the same perspective
		for (int i = 0; i < pieces.size(); i++)
		{
			pieces[i]->switchPiecePositions();
		}

		//Switch square positions
		//If this is not done then code to set pieceOnSquare values breaks
		/*for (size_t i = 0; i < Pieces::squarePositions.size(); i++)
		{
			Pieces::squarePositions[i] = -Pieces::squarePositions[i];
		}
*/
		//Switch player
		if (player == Player::WHITE)
		{
			player = Player::BLACK;
		}
		else if (player == Player::BLACK)
		{
			player = Player::WHITE;
		}
	}

	//Get the index value of the position of the square that matches the piece position from Pieces::squarePositions array
	//------------------------------------------------------------------------------------------------------------
	static int getSquareIndex(const glm::vec2& position)
	{
		int index;
		for (int i = 0; i < 64; i++)
		{
			if (Pieces::squarePositions[i] == position)
			{
				index = i;
				return index;
			}
		}
		return -1;
	}

	static void gameOver(bool isWhite)
	{
		isGameWon = true;
		if (isWhite)
		{
			isWinningPlayerWhite = true;
		}
		else
		{
			isWinningPlayerWhite = false;
		}
	}

	bool isGameOver()
	{
		return isGameWon;
	}

	bool winningPlayer()
	{
		return isWinningPlayerWhite;
	}

	~Board()
	{}

private:
	//---------------
	//Class Variables
	//---------------
	static Pieces* piece;
	static bool squaresSetup;
		
	//Square positions
	static std::vector<glm::vec2> lightSquarePositions;
	static std::vector<glm::vec2> darkSquarePositions;
			
	//Piece specific variables
	static int squareIndex;
	static int selectedPieceSquareIndex;
	static int selectedPieceIndex;
	static glm::vec2 selectedTargetSquare;
	bool pieceExists = false;
	bool movingPiece = false;
	
	//Game win variables
	static bool isGameWon;
	static bool isWinningPlayerWhite;

	//Draw board squares and pieces visible to user
	//---------------------------------------------
	void draw(std::vector<Pieces*> &pieces)
	{
		Graphics::drawBoard(Graphics::getBoardShader(), &lightSquareTexture, lightSquarePositions, 0, 31, 1.0);
		Graphics::drawBoard(Graphics::getBoardShader(), &darkSquareTexture, darkSquarePositions, 0, 31, 1.0);
		for (Pieces* piece : pieces)
		{
			piece->draw();
		}
	}

	//--------------------------------
	//Setup square positions functions
	//--------------------------------

	//Assigns alternating square positions to light and dark square position variables
	//and calls Graphics function to setup instancing 
	void setupSquarePositions()
	{
		calcSquarePositions();

		//calcSquarePositions stores square values one after the other from bottom left corner to top right corner
		//However right most square of a row has the same color as the left most square of the next row in a chess board
		//Hence a bool variable to check for row end and accordingly store the next row square position in the same array
		bool lightSquare = false;
		for (int i = 0; i < Pieces::squarePositions.size(); i++)
		{
			//Bottom left square in a chess board is always dark
			if (i % 8 == 0 && i != 0) //No need to execute for 1st row
			{
				lightSquare = !lightSquare;
			}
			if (lightSquare)
			{
				lightSquarePositions.push_back(Pieces::squarePositions[i]);
			}
			else
			{
				darkSquarePositions.push_back(Pieces::squarePositions[i]);
			}
			lightSquare = !lightSquare;
		}
	}
	//---------------------------
	//Calculates square positions
	void calcSquarePositions()
	{
		//Divide window coordinates by 8 as chess board consists of 64 squares
		//Convert from window coordinates to opengl coordinates
		squareSizeX = (((float)SCR_WIDTH / 8.0) / (float)SCR_WIDTH * 2.0);
		squareSizeY = (((float)SCR_HEIGHT / 8.0) / (float)SCR_HEIGHT * 2.0);

		//Start from the bottom corner of the window
		//Generate offset of center of each square from bottom corner :
		// as instance rendering
		//offsets the center position of an object by the given value after generating the object
		float x = -1, y = -1;
		for (int i = 0; i < 64; i++)
		{
			//If 8 square positions have been calculated in the current row move to next row
			if (i % 8 == 0 && i != 0)
			{
				y += squareSizeY;
				x = -1;
			}

			glm::vec2 centerOffset;
			//Divide squareSize by 2 as we are offseting center which is half of squareSize
			centerOffset.x = x + (squareSizeX / 2.0);
			centerOffset.y = y + (squareSizeY / 2.0);
			Pieces::squarePositions.push_back(centerOffset);
			Pieces::squareColors.push_back(Graphics::genRandomColor());
			x += squareSizeX;
		}
	}

	//------------------------------
	//Piece status related functions
	//------------------------------

	//Highlights the piece currently under mouse cursor
	//-------------------------------------------------
	void drawPiecesOutline(std::vector<Pieces*> &pieces, const glm::vec3* color, bool isWhite)
	{
		for (size_t i = 0; i < pieces.size(); i++)
		{
			selectedPieceIndex = pieces[i]->drawPieceOutline(color, isWhite);
			if (selectedPieceIndex != -1)
			{
				if (Mouse::getMouseClicked())
				{
					piece = pieces[i];
					status = PieceStatus::SELECTED;
				}
				break;
			}
		}
	}

	//Selects the piece and displays all squares it can move to
	//---------------------------------------------------------
	void pieceSelected(std::vector<Pieces*> &pieces, GLFWwindow* window, bool isWhite)
	{
		//Draw the square of the currently selected piece in white indicating current piece selection
		//-------------------------------------------------------------------------------------------
		selectedPieceSquareIndex = getSquareIndex(piece->getPiecePosition(selectedPieceIndex, isWhite));
		Graphics::drawBoard(Graphics::getPiecesColorShader(), &lightSquareTexture, Pieces::squarePositions, selectedPieceSquareIndex, selectedPieceSquareIndex, 1.0, true);
		piece->draw(selectedPieceIndex, isWhite);

		//If selected piece is allowed to move
		if (piece->getMoveAllowed(selectedPieceIndex))
		{
			//Draw moves for the currently selected piece
			piece->drawMoves(window, selectedPieceIndex, isWhite);
		}
		else if (Mouse::getMouseClicked())
		{
			status = PieceStatus::NOT_SELECTED;
		}
	}

	//Moves the piece to the selected target square
	//---------------------------------------------
	void movePiece(std::vector<Pieces*> &pieces, GLFWwindow* window, int pieceIndex, float deltaTime, bool isWhite)
	{
		piece->movePiece(selectedTargetSquare, pieceIndex, deltaTime, isWhite, pieces, window);

		//If piece position matches target square position, player move has ended; switch player
		if (piece->getPiecePosition(pieceIndex, isWhite).x == selectedTargetSquare.x && piece->getPiecePosition(pieceIndex, isWhite).y == selectedTargetSquare.y)
		{
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				int targetPieceIndex;
				//If an enemy piece was on a target square, delete that piece from the board
				if (selectedTargetSquare == Pieces::pieceOnSquare[i].position)
				{
					Pieces::pieceOnSquare[i].piece->deletePiecePosition(Pieces::pieceOnSquare[i].index, isWhite);
				}
			}
			//Don't consider king pieces as king needs to be checked and not allowed to move along the same row/diagonal away from the attacking piece
			/*if (!piece->isKing(piece))
			{
				Pieces::pieceOnSquare[selectedPieceSquareIndex] = false;
				Pieces::pieceOnSquare[getSquareIndex(targetSquares[moveIndex])] = true;
			}*/
			switchPlayer(pieces);
		}
	}
	
	//Get the index value of the position of the square that matches the position of the target square selected
	//---------------------------------------------------------------------------------------------------------
	static int getSquareIndex(const glm::vec3& color)
	{
		int index;
		for (int i = 0; i < 64; i++)
		{
			if (Pieces::squareColors[i] == color)
			{
				index = i;
				return index;
			}
		}
		return -1;
	}

	//Get the square texture to be used to draw over the colored square
	//-----------------------------------------------------------------
	static unsigned int* getSquareTexture(int squareIndex)
	{
		for (int i = 0; i < 32; i++)
		{
			if (Pieces::squarePositions[squareIndex] == lightSquarePositions[i])
			{
				return &lightSquareTexture;
			}
			else if (Pieces::squarePositions[squareIndex] == darkSquarePositions[i])
			{
				return &darkSquareTexture;
			}
		}
	}

};

//static variables initialization
//-------------------------------
bool Board::squaresSetup{ false };
Pieces* Board::piece{ nullptr };
unsigned int Board::lightSquareTexture{ 0 };
unsigned int Board::darkSquareTexture{ 0 };
PieceStatus Board::status = PieceStatus::NOT_SELECTED;
Player Board::player = Player::WHITE;
std::vector<glm::vec2> Board::lightSquarePositions{};
std::vector<glm::vec2> Board::darkSquarePositions{};
int Board::squareIndex = -1;
int Board::selectedPieceSquareIndex = -1;
int Board::selectedPieceIndex = -1;
glm::vec2 Board::selectedTargetSquare{};
bool Board::isGameWon{ false };
bool Board::isWinningPlayerWhite{ false };
float Board::squareSizeX{ 0 };
float Board::squareSizeY{ 0 };

#endif
