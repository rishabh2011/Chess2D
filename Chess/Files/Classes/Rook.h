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

	virtual void calcTargetSquares(bool isWhite, int index = -1) override
	{
		if (index != -1)
		{
			if (isWhite)
			{
				moves.calcRookTargetSquares(targetSquares, index, rookMoves, whiteRookPositions, isWhite);
			}
			else
			{
				moves.calcRookTargetSquares(targetSquares, index, rookMoves, blackRookPositions, isWhite);
			}
		}
		else
		{
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whiteRookPositions.size(); i++)
				{
					moves.calcRookAttackedSquares(targetSquares, i, rookMoves, whiteRookPositions, isWhite, King::blackKingPositions[0]);
				}
				squaresAttacked.insert(squaresAttacked.end(), targetSquares.begin(), targetSquares.end());
			}
			else
			{
				for (size_t i{ 0 }; i < blackRookPositions.size(); i++)
				{
					moves.calcRookAttackedSquares(targetSquares, i, rookMoves, blackRookPositions, isWhite, King::whiteKingPositions[0]);
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

	virtual void drawMoves(GLFWwindow* window, int selectedPieceIndex, bool isWhite) override
	{
		if (targetSquares.size() == 0)
		{
			calcTargetSquares(isWhite, selectedPieceIndex);
		}
		//checkIfPieceIsPinned(targetSquares, selectedPieceIndex, pieces, isWhite);
				
		//Loop through the targetSquares vector
		for(size_t i = 0; i < targetSquares.size(); i++)
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
			squareIndex = Pieces::getSquareIndex(whiteRookPositions[selectedPieceIndex]);
		}
		else
		{
			squareIndex = Pieces::getSquareIndex(blackRookPositions[selectedPieceIndex]);
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
	}*/
	
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

	void generateNewRook(const glm::vec2* targetSquare, bool isWhite)
	{
		if (isWhite)
		{
			whiteRookPositions.push_back(*targetSquare);
			whiteRookColors.push_back(Graphics::genRandomColor());
		}
		else
		{
			blackRookPositions.push_back(*targetSquare);
			blackRookColors.push_back(Graphics::genRandomColor());
		}
		moveAllowed.push_back(true);
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

	virtual ~Rook() = default;
	
private:
	unsigned int blackRook;
	unsigned int whiteRook;
		
	std::vector<glm::vec2> whiteRookPositions;
	std::vector<glm::vec3> whiteRookColors;
	std::vector<glm::vec2> blackRookPositions;
	std::vector<glm::vec3> blackRookColors;
	std::vector<PieceAttribs> targetSquares;
	glm::vec2 diffBetweenSquares;
	Moves moves;
	std::vector<glm::vec2> rookMoves;
	std::vector<bool> moveAllowed;

	std::vector<bool> isPinned;
	int targetPieceIndex;
	glm::vec3 targetSquareColor{ 0.0, 0.8, 1.0 };
	glm::vec3 enemySquareColor{ 1.0, 0.0, 0.0 };
	bool pieceExists;
		
	void setInitRookPositions()
	{
		setRookMoves();
		Pieces::setupPositions(whiteRookPositions, whiteRookColors, 2, 0, 7);
		Pieces::setupPositions(blackRookPositions, blackRookColors, 2, 56, 63);
		for (size_t i = 0; i < 4; i++)
		{
			moveAllowed.push_back(true);
			isPinned.push_back(false);
		}
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
