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

	virtual void calcTargetSquares(bool isWhite, int index = -1) override
	{
		if (index != -1)
		{
			if (isWhite)
			{
				moves.calcKnightTargetSquares(targetSquares, index, knightMoves, whiteKnightPositions, isWhite);
			}
			else
			{
				moves.calcKnightTargetSquares(targetSquares, index, knightMoves, blackKnightPositions, isWhite);
			}
		}
		else
		{
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whiteKnightPositions.size(); i++)
				{
					moves.calcKnightAttackedSquares(targetSquares, i, knightMoves, whiteKnightPositions, isWhite, King::blackKingPositions[0]);
				}
				squaresAttacked.insert(squaresAttacked.end(), targetSquares.begin(), targetSquares.end());
			}
			else
			{
				for (size_t i{ 0 }; i < blackKnightPositions.size(); i++)
				{
					moves.calcKnightAttackedSquares(targetSquares, i, knightMoves, blackKnightPositions, isWhite, King::whiteKingPositions[0]);
				}
				squaresAttacked.insert(squaresAttacked.end(), targetSquares.begin(), targetSquares.end());
			}
			targetSquares.clear();
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

	virtual void drawMoves(GLFWwindow* window, int selectedPieceIndex, bool isWhite) override
	{
		if (targetSquares.size() == 0)
		{
			calcTargetSquares(isWhite, selectedPieceIndex);
		}

		//checkIfPieceIsPinned(targetSquares, selectedPieceIndex, pieces, isWhite);

		//Loop through the targetSquares vector
		for (size_t i = 0; i < targetSquares.size(); i++)
		{
			if (targetSquares[i].piece)
			{
				Board::drawTargetSquare(enemySquareColor, targetSquares[i].position, window, targetSquares[i].piece, targetSquares[i].index, isWhite);
			}
			else
			{
				Board::drawTargetSquare(targetSquareColor, targetSquares[i].position, window, targetSquares[i].piece, targetSquares[i].index, isWhite);
			}
			if (Board::status == PieceStatus::MOVING)
			{
				targetSquares.clear();
				return;
			}
		}
		//If mouse is clicked on an invalid square
		if (Mouse::getMouseClicked())
		{
			Board::status = PieceStatus::NOT_SELECTED;
			targetSquares.clear();
		}
	}

	/*void checkIfPieceIsPinned(std::vector<glm::vec2> &targetSquares, int selectedPieceIndex, const std::vector<Pieces *> &pieces, bool isWhite)
	{
		if (King::kingAttacked)
		{
			return;
		}

		Pieces::squaresAttacked.clear();
		int squareIndex;
		if (isWhite)
		{
			squareIndex = Pieces::getSquareIndex(whiteKnightPositions[selectedPieceIndex]);
		}
		else
		{
			squareIndex = Pieces::getSquareIndex(blackKnightPositions[selectedPieceIndex]);
		}
		Pieces::pieceOnSquare[squareIndex] = false;

		for (size_t i = 0; i < pieces.size(); i++)
		{
			pieces[i]->checkIfAttackingKing(isWhite);
		}

		if (King::kingAttacked)
		{
			moveAllowed[selectedPieceIndex] = false;
			isPinned[selectedPieceIndex] = true;
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				bool squareAttacked = false;
				for (size_t j = 0; j < Pieces::squaresAttacked.size(); j++)
				{
					if (targetSquares[i] == squaresAttacked[j])
					{
						moveAllowed[selectedPieceIndex] = true;
						squareAttacked = true;
					}
				}
				if (!squareAttacked)
				{
					targetSquares[i] = glm::vec2(2.0, 2.0);
				}
			}
			King::kingAttacked = false;
			Pieces::pieceOnSquare[squareIndex] = true;
			if (Mouse::getMouseClicked())
			{
				Board::status = PieceStatus::NOT_SELECTED;
			}
			return;
		}

		isPinned[selectedPieceIndex] = false;
		Pieces::pieceOnSquare[squareIndex] = true;
	}
	*/
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

	void generateNewKnight(const glm::vec2* targetSquare, bool isWhite)
	{
		if (isWhite)
		{
			whiteKnightPositions.push_back(*targetSquare);
			whiteKnightColors.push_back(Graphics::genRandomColor());
		}
		else
		{
			blackKnightPositions.push_back(*targetSquare);
			blackKnightColors.push_back(Graphics::genRandomColor());
		}
		moveAllowed.push_back(true);
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

	virtual void setMoveAllowed(bool val) override
	{
		for (size_t i = 0; i < moveAllowed.size(); i++)
		{
			moveAllowed[i] = val;
		}
	}

	virtual bool getMoveAllowed(int pieceIndex) override
	{
		return moveAllowed[pieceIndex];
	}

	virtual bool noPieceCanMove() override
	{
		for (size_t i = 0; i < moveAllowed.size(); i++)
		{
			if (moveAllowed[i])
			{
				return false;
			}
		}
		return true;
	}

	virtual ~Knight() = default;

private:
	unsigned int blackKnight;
	unsigned int whiteKnight;

	std::vector<glm::vec2> whiteKnightPositions;
	std::vector<glm::vec3> whiteKnightColors;
	std::vector<glm::vec2> blackKnightPositions;
	std::vector<glm::vec3> blackKnightColors;

	std::vector<PieceAttribs> targetSquares;
	glm::vec2 diffBetweenSquares;
	Moves moves;
	std::vector<glm::vec2> knightMoves;
	std::vector<bool> moveAllowed;

	std::vector<bool> isPinned;
	int targetPieceIndex;
	glm::vec3 targetSquareColor{ 0.0, 0.8, 1.0 };
	glm::vec3 enemySquareColor{ 1.0, 0.0, 0.0 };
	bool pieceExists;
	
	void setInitKnightPositions()
	{
		setKnightMoves();
		Pieces::setupPositions(whiteKnightPositions, whiteKnightColors, 2, 1, 6);
		Pieces::setupPositions(blackKnightPositions, blackKnightColors, 2, 57, 62);
		for (size_t i = 0; i < 4; i++)
		{
			moveAllowed.push_back(true);
			isPinned.push_back(false);
		}
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
