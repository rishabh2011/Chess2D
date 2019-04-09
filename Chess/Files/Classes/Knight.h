/* ---------------------------------------------------------------------------------------------------------------
  Universal piece functions have been commented in pawn class. Only piece specific functions commented in this class
  ---------------------------------------------------------------------------------------------------------------- */
#ifndef KNIGHT_H
#define KNIGHT_H

#include "Pieces.h"
#include "Board.h"

class Knight : public Pieces
{
public:

	Knight()
	{
		whiteKnight = Graphics::loadTexture("Files/Textures/Pieces/White/white knight.png", true);
		blackKnight = Graphics::loadTexture("Files/Textures/Pieces/Black/black knight.png", true);
		setInitKnightPositions();
	}

	void knightOutline(int index, bool isWhite) const
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
				for (size_t i{ 0 }; i < whiteKnightPositions.size(); i++)
				{
					moves.calcKnightTargetSquares(targetSquares, i, knightMoves, whiteKnightPositions, isWhite, pieces);
					availableTargetSquares.push_back(targetSquares);
					targetSquares.clear();
				}
			}
			else
			{
				for (size_t i{ 0 }; i < blackKnightPositions.size(); i++)
				{
					moves.calcKnightTargetSquares(targetSquares, i, knightMoves, blackKnightPositions, isWhite, pieces);
					availableTargetSquares.push_back(targetSquares);
					targetSquares.clear();
				}
			}
		}
		else
		{
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whiteKnightPositions.size(); i++)
				{
					moves.calcKnightAttackedSquares(targetSquares, i, knightMoves, whiteKnightPositions, isWhite, King::getKingPosition(!isWhite));
				}
			}
			else
			{
				for (size_t i{ 0 }; i < blackKnightPositions.size(); i++)
				{
					moves.calcKnightAttackedSquares(targetSquares, i, knightMoves, blackKnightPositions, isWhite, King::getKingPosition(!isWhite));
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
				Pieces::drawPieces(&whiteKnight, whiteKnightPositions, index, index, true);
			}
			else
			{
				Pieces::drawPieces(&blackKnight, blackKnightPositions, index, index, true);
			}
		}
		else
		{
			Pieces::drawPieces(&whiteKnight, whiteKnightPositions, 0, whiteKnightPositions.size() - 1);
			Pieces::drawPieces(&blackKnight, blackKnightPositions, 0, blackKnightPositions.size() - 1);
		}
	}

	virtual void drawColor(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index of the position value that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPiecesColor(&whiteKnight, whiteKnightPositions, whiteKnightColors, index, index, true);
			}
			else
			{
				Pieces::drawPiecesColor(&blackKnight, blackKnightPositions, blackKnightColors, index, index, true);
			}
		}
		else
		{
			Pieces::drawPiecesColor(&whiteKnight, whiteKnightPositions, whiteKnightColors, 0, whiteKnightPositions.size() - 1);
			Pieces::drawPiecesColor(&blackKnight, blackKnightPositions, blackKnightColors, 0, blackKnightPositions.size() - 1);
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
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, knightMoves, whiteKnightPositions);
		}
		else
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, knightMoves, blackKnightPositions);
		}
	}

	virtual void switchPiecePositions() override
	{
		Pieces::switchPiecePositions(whiteKnightPositions);
		Pieces::switchPiecePositions(blackKnightPositions);
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
			val = Pieces::pieceColorIndex(whiteKnightColors, whiteKnightColors.size(), color);
		}
		else
		{
			val = Pieces::pieceColorIndex(blackKnightColors, blackKnightColors.size(), color);
		}
		if (val != -1)
		{
			knightOutline(val, isWhite);
			return val;
		}
		return -1;
	}
		
	virtual const glm::vec2& getPiecePosition(int index, bool isWhite) const override
	{
		if (isWhite)
		{
			return whiteKnightPositions[index];
		}
		else
		{
			return blackKnightPositions[index];
		}
	}

	virtual void deletePiecePosition(int index, bool isWhite, bool isPawnPromote = false) override
	{
		if (isWhite)
		{
			blackKnightPositions[index] = glm::vec2(2.0, 2.0);
		}
		else
		{
			whiteKnightPositions[index] = glm::vec2(2.0, 2.0);
		}
	}

	//Generates a new knight at the target square where pawn gets promoted
	//--------------------------------------------------------------------
	void generateNewKnight(const glm::vec2* targetSquare, bool isWhite)
	{
		if (isWhite)
		{
			//Add a new knight to the knight position vector 
			whiteKnightPositions.push_back(*targetSquare);
			//Also generate a new color for the new knight
			whiteKnightColors.push_back(Graphics::genRandomColor());
		}
		else
		{
			//Add a new knight to the knight position vector 
			blackKnightPositions.push_back(*targetSquare);
			//Also generate a new color for the new knight
			blackKnightColors.push_back(Graphics::genRandomColor());
		}
	}

	virtual void calcDifferenceBetweenSquares(const glm::vec2 &targetSquare, int index, bool isWhite) override
	{
		if (isWhite)
		{
			diffBetweenSquares = targetSquare - whiteKnightPositions[index];
		}
		else
		{
			diffBetweenSquares = targetSquare - blackKnightPositions[index];
		}
	}

	virtual ~Knight() = default;

private:
	unsigned int blackKnight;
	unsigned int whiteKnight;

	std::vector<glm::vec2> whiteKnightPositions;
	std::vector<glm::vec3> whiteKnightColors;
	std::vector<glm::vec2> blackKnightPositions;
	std::vector<glm::vec3> blackKnightColors;

	glm::vec2 diffBetweenSquares;
	std::vector<std::vector<PieceAttribs>> availableTargetSquares;

	Moves moves;
	std::vector<glm::vec2> knightMoves;
	
	glm::vec3 targetSquareColor{ 0.0, 0.8, 1.0 };
	glm::vec3 enemySquareColor{ 1.0, 0.0, 0.0 };
	
	void setInitKnightPositions()
	{
		setKnightMoves();
		Pieces::setupPositions(whiteKnightPositions, whiteKnightColors, 2, 1, 6);
		Pieces::setupPositions(blackKnightPositions, blackKnightColors, 2, 57, 62);
	}

	virtual void setupPieceOnSquareValues() override
	{
		PieceAttribs square;
		for (size_t i = 0; i < whiteKnightPositions.size(); i++)
		{
			square.position = whiteKnightPositions[i];
			square.index = i;
			square.isWhite = true;
			square.piece = this;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}

		for (size_t i = 0; i < blackKnightPositions.size(); i++)
		{
			square.position = blackKnightPositions[i];
			square.index = i;
			square.isWhite = false;
			square.piece = this;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}
	}

	void setKnightMoves()
	{
		//Knight can move in an L shape in 8 different directions
		knightMoves.push_back(glm::vec2(0.25, 0.5));
		knightMoves.push_back(glm::vec2(-0.25, 0.5));
		knightMoves.push_back(glm::vec2(0.5, 0.25));
		knightMoves.push_back(glm::vec2(-0.5, 0.25));
		knightMoves.push_back(-knightMoves[0]);
		knightMoves.push_back(-knightMoves[1]);
		knightMoves.push_back(-knightMoves[2]);
		knightMoves.push_back(-knightMoves[3]);
	}
};

#endif