#ifndef BISHOP_H
#define BISHOP_H

#include "Pieces.h"
#include "Board.h"
#include "King.h"
#include <Moves.h>

class Bishop : public Pieces
{
public:

	Bishop()
	{
		whiteBishop = Graphics::loadTexture("Files/Textures/Pieces/White/white bishop.png", true);
		blackBishop = Graphics::loadTexture("Files/Textures/Pieces/Black/black bishop.png", true);
		setInitBishopPositions();
	}
	
	void bishopOutline(int index, bool isWhite) const 
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
				moves.calcBishopTargetSquares(targetSquares, index, bishopMoves, whiteBishopPositions, isWhite);
			}
			else
			{
				moves.calcBishopTargetSquares(targetSquares, index, bishopMoves, blackBishopPositions, isWhite);
			}
		}
		else
		{
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whiteBishopPositions.size(); i++)
				{
					moves.calcBishopAttackedSquares(targetSquares, i, bishopMoves, whiteBishopPositions, isWhite, King::blackKingPositions[0]);
				}
				squaresAttacked.insert(squaresAttacked.end(), targetSquares.begin(), targetSquares.end());
			}
			else
			{
				for (size_t i{ 0 }; i < blackBishopPositions.size(); i++)
				{
					moves.calcBishopAttackedSquares(targetSquares, i, bishopMoves, blackBishopPositions, isWhite, King::whiteKingPositions[0]);
				}
				squaresAttacked.insert(squaresAttacked.end(), targetSquares.begin(), targetSquares.end());
			}
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (Pieces::pieceOnSquare[i].position == whiteBishopPositions[0])
				{
					std::cout << std::boolalpha << pieceOnSquare[i].attackingOpponentKing << std::endl;
				}
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
				Pieces::drawPieces(&whiteBishop, whiteBishopPositions, index, index, true);
			}
			else
			{
				Pieces::drawPieces(&blackBishop, blackBishopPositions, index, index, true);
			}
		}
		else
		{
			Pieces::drawPieces(&whiteBishop, whiteBishopPositions, 0, whiteBishopPositions.size() - 1);
			Pieces::drawPieces(&blackBishop, blackBishopPositions, 0, blackBishopPositions.size() - 1);
		}
	} 

	virtual void drawColor(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index of the position value that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPiecesColor(&whiteBishop, whiteBishopPositions, whiteBishopColors, index, index, true);
			}
			else
			{
				Pieces::drawPiecesColor(&blackBishop, blackBishopPositions, blackBishopColors, index, index, true);
			}
		}
		else
		{
			Pieces::drawPiecesColor(&whiteBishop, whiteBishopPositions, whiteBishopColors, 0, whiteBishopPositions.size() - 1);
			Pieces::drawPiecesColor(&blackBishop, blackBishopPositions, blackBishopColors, 0, blackBishopPositions.size() - 1);
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
			squareIndex = Pieces::getSquareIndex(whiteBishopPositions[selectedPieceIndex]);
		}
		else
		{
			squareIndex = Pieces::getSquareIndex(blackBishopPositions[selectedPieceIndex]);
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
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, bishopMoves, whiteBishopPositions);
		}
		else
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, bishopMoves, blackBishopPositions);
		}
	}

	virtual void switchPiecePositions() override
	{
		Pieces::switchPiecePositions(whiteBishopPositions);
		Pieces::switchPiecePositions(blackBishopPositions);
	}

	virtual int drawPieceOutline(const glm::vec3* color, bool isWhite) const override
	{
		int val;
		if (isWhite)
		{
			val = Pieces::pieceColorIndex(whiteBishopColors, whiteBishopColors.size(), color);
		}
		else
		{
			val = Pieces::pieceColorIndex(blackBishopColors, blackBishopColors.size(), color);
		}
		if (val != -1)
		{
			bishopOutline(val, isWhite);
			return val;
		}
		return -1;
	}
		
	virtual const glm::vec2& getPiecePosition(int index, bool isWhite) const override
	{
		if (isWhite)
		{
			return whiteBishopPositions[index];
		}
		else
		{
			return blackBishopPositions[index];
		}
	}

	virtual void deletePiecePosition(int index, bool isWhite, bool isPawnPromote = false) override
	{
		if (isWhite)
		{
			blackBishopPositions[index] = glm::vec2(2.0, 2.0);
		}
		else
		{
			whiteBishopPositions[index] = glm::vec2(2.0, 2.0);
		}
	}

	void generateNewBishop(const glm::vec2* targetSquare, bool isWhite)
	{
		if (isWhite)
		{
			whiteBishopPositions.push_back(*targetSquare);
			whiteBishopColors.push_back(Graphics::genRandomColor());
		}
		else
		{
			blackBishopPositions.push_back(*targetSquare);
			blackBishopColors.push_back(Graphics::genRandomColor());
		}
		moveAllowed.push_back(true);
	}

	virtual void calcDifferenceBetweenSquares(const glm::vec2 &targetSquare, int index, bool isWhite) override
	{
		if (isWhite)
		{
			diffBetweenSquares = targetSquare - whiteBishopPositions[index];
		}
		else
		{
			diffBetweenSquares = targetSquare - blackBishopPositions[index];
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

	virtual ~Bishop() = default;

private:
	unsigned int blackBishop;
	unsigned int whiteBishop;

	std::vector<glm::vec2> whiteBishopPositions;
	std::vector<glm::vec3> whiteBishopColors;
	std::vector<glm::vec2> blackBishopPositions;
	std::vector<glm::vec3> blackBishopColors;
	std::vector<PieceAttribs> targetSquares;
	glm::vec2 diffBetweenSquares;
	Moves moves;
	std::vector<glm::vec2> bishopMoves;
	std::vector<bool> moveAllowed;
	
	std::vector<bool> isPinned;
	int targetPieceIndex;
	glm::vec3 targetSquareColor{ 0.0, 0.8, 1.0 };
	glm::vec3 enemySquareColor{ 1.0, 0.0, 0.0 };
	bool pieceExists;

	void setInitBishopPositions()
	{
		setBishopMoves();
		Pieces::setupPositions(whiteBishopPositions, whiteBishopColors, 2, 2, 5);
		Pieces::setupPositions(blackBishopPositions, blackBishopColors, 2, 58, 61);
		for (size_t i = 0; i < 4; i++)
		{
			moveAllowed.push_back(true);
			isPinned.push_back(false);
		}
	}

	virtual void setupPieceOnSquareValues() override
	{
		PieceAttribs square;
		for (size_t i = 0; i < whiteBishopPositions.size(); i++)
		{
			square.position = whiteBishopPositions[i];
			square.index = i;
			square.isWhite = true;
			square.piece = this;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}

		for (size_t i = 0; i < blackBishopPositions.size(); i++)
		{
			square.position = blackBishopPositions[i];
			square.index = i;
			square.isWhite = false;
			square.piece = this;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}
	}

	void setBishopMoves()
	{
		for (int i = 0; i < 7; i++)
		{
			bishopMoves.push_back(glm::vec2((i + 1) * 0.25, (i + 1) * 0.25));
			bishopMoves.push_back(-glm::vec2((i + 1) * 0.25, (i + 1) * 0.25));
			bishopMoves.push_back(glm::vec2((i + 1) * 0.25, -((i + 1) * 0.25)));
			bishopMoves.push_back(-glm::vec2((i + 1) * 0.25, -((i + 1) * 0.25)));
		}
	}
};

#endif

