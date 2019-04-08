/* ---------------------------------------------------------------------------------------------------------------
  Universal piece functions have been commented in pawn class. Only piece specific functions commented in this class
  ---------------------------------------------------------------------------------------------------------------- */
#ifndef ROOK_H
#define ROOK_H

#include "Board.h"
#include "Pieces.h"
#include "King.h"
#include <Moves.h>

class Rook : public Pieces
{
public:

	Rook()
	{
		whiteRook = Graphics::loadTexture("Files/Textures/Pieces/White/white rook.png", true);
		blackRook = Graphics::loadTexture("Files/Textures/Pieces/Black/black rook.png", true);
		setInitRookPositions();
	}
	
	void rookOutline(int index, bool isWhite) const
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		draw(index, isWhite);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		drawColor(index, isWhite);
	}

	virtual void calcTargetSquares(bool isWhite, bool currentPlayer, const std::vector<Pieces*> &pieces = std::vector<Pieces*>{}) override
	{
		std::vector<PieceAttribs> targetSquares;
		if (currentPlayer)
		{
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whiteRookPositions.size(); i++)
				{
					moves.calcRookTargetSquares(targetSquares, i, rookMoves, whiteRookPositions, isWhite, pieces);
					availableTargetSquares.push_back(targetSquares);
					targetSquares.clear();
				}
			}
			else
			{
				for (size_t i{ 0 }; i < blackRookPositions.size(); i++)
				{
					moves.calcRookTargetSquares(targetSquares, i, rookMoves, blackRookPositions, isWhite, pieces);
					availableTargetSquares.push_back(targetSquares);
					targetSquares.clear();
				}
			}
		}
		else
		{
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whiteRookPositions.size(); i++)
				{
					moves.calcRookAttackedSquares(targetSquares, i, rookMoves, whiteRookPositions, isWhite, King::getKingPosition(!isWhite));
				}
			}
			else
			{
				for (size_t i{ 0 }; i < blackRookPositions.size(); i++)
				{
					moves.calcRookAttackedSquares(targetSquares, i, rookMoves, blackRookPositions, isWhite, King::getKingPosition(!isWhite));
				}
			}
			squaresAttacked.insert(squaresAttacked.end(), targetSquares.begin(), targetSquares.end());
		}
	}
	
	virtual void draw(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index value of the position that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPieces(&whiteRook, whiteRookPositions, index, index, true);
			}
			else
			{
				Pieces::drawPieces(&blackRook, blackRookPositions, index, index, true);
			}
		}
		else
		{
			Pieces::drawPieces(&whiteRook, whiteRookPositions, 0, whiteRookPositions.size() - 1);
			Pieces::drawPieces(&blackRook, blackRookPositions, 0, blackRookPositions.size() - 1);
		}
	}

	virtual void drawColor(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index of the position value that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPiecesColor(&whiteRook, whiteRookPositions, whiteRookColors, index, index, true);
			}
			else
			{
				Pieces::drawPiecesColor(&blackRook, blackRookPositions, blackRookColors, index, index, true);
			}
		}
		else
		{
			Pieces::drawPiecesColor(&whiteRook, whiteRookPositions, whiteRookColors, 0, whiteRookPositions.size() - 1);
			Pieces::drawPiecesColor(&blackRook, blackRookPositions, blackRookColors, 0, blackRookPositions.size() - 1);
		}
	}

	virtual void drawMoves(GLFWwindow* window, int selectedPieceIndex, bool isWhite, const std::vector<Pieces*> &pieces) override
	{
		//Loop through the targetSquares vector and draw the target squares
		for (size_t i = 0; i < availableTargetSquares[selectedPieceIndex].size(); i++)
		{
			//If an opponent piece exists on target square, draw the square in red
			if (availableTargetSquares[selectedPieceIndex][i].piece)
			{
				Board::drawTargetSquare(enemySquareColor, availableTargetSquares[selectedPieceIndex][i].position, window, availableTargetSquares[selectedPieceIndex][i].piece, availableTargetSquares[selectedPieceIndex][i].index, isWhite);
			}
			else
			{
				Board::drawTargetSquare(targetSquareColor, availableTargetSquares[selectedPieceIndex][i].position, window, availableTargetSquares[selectedPieceIndex][i].piece, availableTargetSquares[selectedPieceIndex][i].index, isWhite);
			}
			//If user has selected a square for moving then break 
			if (Board::status == PieceStatus::MOVING)
			{
				return;
			}
		}
		//If mouse is clicked on an invalid square
		if (Mouse::getMouseClicked())
		{
			Board::status = PieceStatus::NOT_SELECTED;
		}
	}

	virtual void movePiece(const glm::vec2 &targetSquare, int pieceIndex, float deltaTime, bool isWhite, const std::vector<Pieces*> &pieces, GLFWwindow* window) override
	{
		if (isWhite)
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, rookMoves, whiteRookPositions);
		}
		else
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, rookMoves, blackRookPositions);
		}
	}

	virtual void switchPiecePositions() override
	{ 
		Pieces::switchPiecePositions(whiteRookPositions);
		Pieces::switchPiecePositions(blackRookPositions);
	}

	virtual void clearAvailableTargetSquares() override
	{
		availableTargetSquares.clear();
	}
	
	virtual int drawPieceOutline(const glm::vec3* color, bool isWhite) const override
	{
		int val;
		if (isWhite)
		{
			val = Pieces::pieceColorIndex(whiteRookColors, whiteRookColors.size(), color);
		}
		else
		{
			val = Pieces::pieceColorIndex(blackRookColors, blackRookColors.size(), color);
		}
		if (val != -1)
		{
			rookOutline(val, isWhite);
			return val;
		}
		return -1;
	}
	
	virtual const glm::vec2& getPiecePosition(int index, bool isWhite) const override
	{
		if (isWhite)
		{
			return whiteRookPositions[index];
		}
		else
		{
			return blackRookPositions[index];
		}
	}

	virtual void deletePiecePosition(int index, bool isWhite, bool isPawnPromote = false) override
	{
		if (isWhite)
		{
			blackRookPositions[index] = glm::vec2(2.0, 2.0);
		}
		else
		{
			whiteRookPositions[index] = glm::vec2(2.0, 2.0);
		}
	}

	//Generates a new rook at the target square where pawn gets promoted
	//------------------------------------------------------------------
	void generateNewRook(const glm::vec2* targetSquare, bool isWhite)
	{
		if (isWhite)
		{
			//Add a new rook to the rook position vector 
			whiteRookPositions.push_back(*targetSquare);
			//Also generate a new color for the new rook
			whiteRookColors.push_back(Graphics::genRandomColor());
		}
		else
		{
			//Add a new rook to the rook position vector 
			blackRookPositions.push_back(*targetSquare);
			//Also generate a new color for the new rook
			blackRookColors.push_back(Graphics::genRandomColor());
		}
	}

	virtual void calcDifferenceBetweenSquares(const glm::vec2 &targetSquare, int index, bool isWhite) override
	{
		if (isWhite)
		{
			diffBetweenSquares = targetSquare - whiteRookPositions[index];
		}
		else
		{
			diffBetweenSquares = targetSquare - blackRookPositions[index];
		}
	}
		
	virtual ~Rook() = default;
	
private:
	unsigned int blackRook;
	unsigned int whiteRook;
		
	std::vector<glm::vec2> whiteRookPositions;
	std::vector<glm::vec3> whiteRookColors;
	std::vector<glm::vec2> blackRookPositions;
	std::vector<glm::vec3> blackRookColors;

	glm::vec2 diffBetweenSquares;
	std::vector<std::vector<PieceAttribs>> availableTargetSquares;

	Moves moves;
	std::vector<glm::vec2> rookMoves;
		
	glm::vec3 targetSquareColor{ 0.0, 0.8, 1.0 };
	glm::vec3 enemySquareColor{ 1.0, 0.0, 0.0 };
			
	void setInitRookPositions()
	{
		setRookMoves();
		Pieces::setupPositions(whiteRookPositions, whiteRookColors, 2, 0, 7);
		Pieces::setupPositions(blackRookPositions, blackRookColors, 2, 56, 63);
		//whiteRookPositions[0] = squarePositions[20];
	}

	virtual void setupPieceOnSquareValues() override
	{
		PieceAttribs square;
		for (size_t i = 0; i < whiteRookPositions.size(); i++)
		{
			square.position = whiteRookPositions[i];
			square.index = i;
			square.isWhite = true;
			square.piece = this;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}

		for (size_t i = 0; i < blackRookPositions.size(); i++)
		{
			square.position = blackRookPositions[i];
			square.index = i;
			square.isWhite = false;
			square.piece = this;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}
	}

	void setRookMoves()
	{
		//Rook can move straight in 4 directions for a maximum of 7 squares
		for (int i = 0; i < 7; i++)
		{
			rookMoves.push_back(glm::vec2((i + 1) * 0.25, 0.0));
			rookMoves.push_back(-glm::vec2((i + 1) * 0.25, 0.0));
			rookMoves.push_back(glm::vec2(0.0, (i + 1) * 0.25));
			rookMoves.push_back(-glm::vec2(0.0, (i + 1) * 0.25));
		}
	}
};

#endif
