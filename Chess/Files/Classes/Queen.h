#ifndef QUEEN_H
#define QUEEN_H

#include "Pieces.h"
#include "Board.h"
#include "King.h"
#include <Moves.h>

class Queen : public Pieces
{
public:

	Queen()
	{
		whiteQueen = Graphics::loadTexture("Files/Textures/Pieces/White/white queen.png", true);
		blackQueen = Graphics::loadTexture("Files/Textures/Pieces/Black/black queen.png", true);
		setInitQueenPositions();
	}

	void queenOutline(int index, bool isWhite) const
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
				moves.calcQueenTargetSquares(targetSquares, index, queenMoves, whiteQueenPositions, isWhite);
			}
			else
			{
				moves.calcQueenTargetSquares(targetSquares, index, queenMoves, blackQueenPositions, isWhite);
			}
		}
		else
		{
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whiteQueenPositions.size(); i++)
				{
					moves.calcQueenAttackedSquares(targetSquares, i, queenMoves, whiteQueenPositions, isWhite, King::blackKingPositions[0]);
				}
				squaresAttacked.insert(squaresAttacked.end(), targetSquares.begin(), targetSquares.end());
			}
			else
			{
				for (size_t i{ 0 }; i < blackQueenPositions.size(); i++)
				{
					moves.calcQueenAttackedSquares(targetSquares, i, queenMoves, blackQueenPositions, isWhite, King::whiteKingPositions[0]);
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
				Pieces::drawPieces(&whiteQueen, whiteQueenPositions, index, index, true);
			}
			else
			{
				Pieces::drawPieces(&blackQueen, blackQueenPositions, index, index, true);
			}
		}
		else
		{
			Pieces::drawPieces(&whiteQueen, whiteQueenPositions, 0, whiteQueenPositions.size() - 1);
			Pieces::drawPieces(&blackQueen, blackQueenPositions, 0, blackQueenPositions.size() - 1);
		}
	}

	virtual void drawColor(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index of the position value that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPiecesColor(&whiteQueen, whiteQueenPositions, whiteQueenColors, index, index, true);
			}
			else
			{
				Pieces::drawPiecesColor(&blackQueen, blackQueenPositions, blackQueenColors, index, index, true);
			}
		}
		else
		{
			Pieces::drawPiecesColor(&whiteQueen, whiteQueenPositions, whiteQueenColors, 0, whiteQueenPositions.size() - 1);
			Pieces::drawPiecesColor(&blackQueen, blackQueenPositions, blackQueenColors, 0, blackQueenPositions.size() - 1);
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
		}
		if (Board::status == PieceStatus::MOVING)
		{
			targetSquares.clear();
			return;
		}
		//If no moves are possible
		if (Mouse::getMouseClicked())
		{
			targetSquares.clear();
			Board::status = PieceStatus::NOT_SELECTED;
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
			squareIndex = Pieces::getSquareIndex(whiteQueenPositions[selectedPieceIndex]);
		}
		else
		{
			squareIndex = Pieces::getSquareIndex(blackQueenPositions[selectedPieceIndex]);
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
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, queenMoves, whiteQueenPositions);
		}
		else
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, queenMoves, blackQueenPositions);
		}
	}

	virtual void switchPiecePositions()
	{
		Pieces::switchPiecePositions(whiteQueenPositions);
		Pieces::switchPiecePositions(blackQueenPositions);
	}

	virtual int drawPieceOutline(const glm::vec3* color, bool isWhite) const override
	{
		int val;
		if (isWhite)
		{
			val = Pieces::pieceColorIndex(whiteQueenColors, whiteQueenColors.size(), color);
		}
		else
		{
			val = Pieces::pieceColorIndex(blackQueenColors, blackQueenColors.size(), color);
		}
		if (val != -1)
		{
			queenOutline(val, isWhite);
			return val;
		}
		return -1;
	}
		
	virtual const glm::vec2& getPiecePosition(int index, bool isWhite) const override
	{
		if (isWhite)
		{
			return whiteQueenPositions[index];
		}
		else
		{
			return blackQueenPositions[index];
		}
	}

	virtual void deletePiecePosition(int index, bool isWhite, bool isPawnPromote = false) override
	{
		if (isWhite)
		{
			blackQueenPositions[index] = glm::vec2(2.0, 2.0);
		}
		else
		{
			whiteQueenPositions[index] = glm::vec2(2.0, 2.0);
		}
	}

	void generateNewQueen(const glm::vec2* targetSquare, bool isWhite)
	{
		if (isWhite)
		{
			whiteQueenPositions.push_back(*targetSquare);
			whiteQueenColors.push_back(Graphics::genRandomColor());
		}
		else
		{
			blackQueenPositions.push_back(*targetSquare);
			blackQueenColors.push_back(Graphics::genRandomColor());
		}
		moveAllowed.push_back(true);
	}

	virtual void calcDifferenceBetweenSquares(const glm::vec2 &targetSquare, int index, bool isWhite) override
	{
		if (isWhite)
		{
			diffBetweenSquares = targetSquare - whiteQueenPositions[index];
		}
		else
		{
			diffBetweenSquares = targetSquare - blackQueenPositions[index];
		}
	}

	virtual bool getMoveAllowed(int pieceIndex) override
	{
		return moveAllowed[pieceIndex];
	}

	virtual void setMoveAllowed(bool val) override
	{
		for (size_t i = 0; i < moveAllowed.size(); i++)
		{
			moveAllowed[i] = val;
		}
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

	virtual ~Queen() = default;

private:
	unsigned int blackQueen;
	unsigned int whiteQueen;

	std::vector<glm::vec2> whiteQueenPositions;
	std::vector<glm::vec3> whiteQueenColors;
	std::vector<glm::vec2> blackQueenPositions;
	std::vector<glm::vec3> blackQueenColors;
	std::vector<PieceAttribs> targetSquares;
	glm::vec2 diffBetweenSquares;
	Moves moves;
	std::vector<glm::vec2> queenMoves;
	std::vector<bool> moveAllowed;

	std::vector<bool> isPinned;
	int targetPieceIndex;
	glm::vec3 targetSquareColor{ 0.0, 0.8, 1.0 };
	glm::vec3 enemySquareColor{ 1.0, 0.0, 0.0 };
	bool pieceExists;

	void setInitQueenPositions()
	{
		setQueenMoves();
		Pieces::setupPositions(whiteQueenPositions, whiteQueenColors, 1, 3);
		Pieces::setupPositions(blackQueenPositions, blackQueenColors, 1, 59);
		for (size_t i = 0; i < 1; i++)
		{
			moveAllowed.push_back(true);
			isPinned.push_back(false);
		}
	}

	virtual void setupPieceOnSquareValues() override
	{
		PieceAttribs square;
		for (size_t i = 0; i < whiteQueenPositions.size(); i++)
		{
			square.position = whiteQueenPositions[i];
			square.index = i;
			square.isWhite = true;
			square.piece = this;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}

		for (size_t i = 0; i < blackQueenPositions.size(); i++)
		{
			square.position = blackQueenPositions[i];
			square.index = i;
			square.isWhite = false;
			square.piece = this;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}
	}

	void setQueenMoves()
	{
		for (int i = 0; i < 7; i++) 
		{
			queenMoves.push_back(glm::vec2((i + 1) * 0.25, 0.0));
			queenMoves.push_back(-glm::vec2((i + 1) * 0.25, 0.0));
			queenMoves.push_back(glm::vec2(0.0, (i + 1) * 0.25));
			queenMoves.push_back(-glm::vec2(0.0, (i + 1) * 0.25));
			queenMoves.push_back(glm::vec2((i + 1) * 0.25, (i + 1) * 0.25));
			queenMoves.push_back(-glm::vec2((i + 1) * 0.25, (i + 1) * 0.25));
			queenMoves.push_back(glm::vec2(-((i + 1) * 0.25), (i + 1) * 0.25));
			queenMoves.push_back(-glm::vec2(-((i + 1) * 0.25), (i + 1) * 0.25));
		}
	}
};

#endif
