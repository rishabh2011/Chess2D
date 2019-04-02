#ifndef KING_H
#define KING_H

#include "Pieces.h"
#include "Board.h"
#include <Moves.h>

class King : public Pieces
{
public:

	static std::vector<glm::vec2> whiteKingPositions;
	static std::vector<glm::vec2> blackKingPositions;
	static int attackingPieces;

	King()
	{
		whiteKing = Graphics::loadTexture("Files/Textures/Pieces/White/white king.png", true);
		blackKing = Graphics::loadTexture("Files/Textures/Pieces/Black/black king.png", true);
		setInitKingPositions();
	}

	void kingOutline(int index, bool isWhite) const
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
				moves.calcKingTargetSquares(targetSquares, index, kingMoves, whiteKingPositions, isWhite);
			}
			else
			{
				moves.calcKingTargetSquares(targetSquares, index, kingMoves, blackKingPositions, isWhite);
			}
		}
		else
		{
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whiteKingPositions.size(); i++)
				{
					moves.calcKingAttackedSquares(targetSquares, i, kingMoves, whiteKingPositions, isWhite);
				}
				squaresAttacked.insert(squaresAttacked.end(), targetSquares.begin(), targetSquares.end());
			}
			else
			{
				for (size_t i{ 0 }; i < blackKingPositions.size(); i++)
				{
					moves.calcKingAttackedSquares(targetSquares, i, kingMoves, blackKingPositions, isWhite);
				}
				squaresAttacked.insert(squaresAttacked.end(), targetSquares.begin(), targetSquares.end());
			}
			targetSquares.clear();
		}
	}

	virtual void drawKingInCheck(bool isWhite) override
	{
		if (kingAttacked)
		{
			if (isWhite)
			{
				int squareIndex = Pieces::getSquareIndex(whiteKingPositions[0]);
				if (squareIndex != -1)
				{
					Graphics::drawBoard(Graphics::getPiecesColorShader(), &Board::lightSquareTexture, Pieces::squarePositions, squareIndex, squareIndex, 1.0, true, glm::vec3(1.0, 0.0, 0.0));
				}
				draw(0, isWhite);
			}
			else
			{
				int squareIndex = Pieces::getSquareIndex(blackKingPositions[0]);
				if (squareIndex != -1)
				{
					Graphics::drawBoard(Graphics::getPiecesColorShader(), &Board::lightSquareTexture, Pieces::squarePositions, squareIndex, squareIndex, 1.0, true, glm::vec3(1.0, 0.0, 0.0));
				}
				draw(0, isWhite);
			}
		}
	}

	virtual void draw(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index value of the position that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPieces(&whiteKing, whiteKingPositions, index, index, true);
			}
			else
			{
				Pieces::drawPieces(&blackKing, blackKingPositions, index, index, true);
			}
		}
		else
		{
			Pieces::drawPieces(&whiteKing, whiteKingPositions, 0, whiteKingPositions.size() - 1);
			Pieces::drawPieces(&blackKing, blackKingPositions, 0, blackKingPositions.size() - 1);
		}
	}

	virtual void drawColor(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index of the position value that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPiecesColor(&whiteKing, whiteKingPositions, whiteKingColors, index, index, true);
			}
			else
			{
				Pieces::drawPiecesColor(&blackKing, blackKingPositions, blackKingColors, index, index, true);
			}
		}
		else
		{
			Pieces::drawPiecesColor(&whiteKing, whiteKingPositions, whiteKingColors, 0, whiteKingPositions.size() - 1);
			Pieces::drawPiecesColor(&blackKing, blackKingPositions, blackKingColors, 0, blackKingPositions.size() - 1);
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
	
	virtual void movePiece(const glm::vec2 &targetSquare, int pieceIndex, float deltaTime, bool isWhite, const std::vector<Pieces*> &pieces, GLFWwindow* window) override
	{
		if (isWhite)
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, kingMoves, whiteKingPositions);
		}
		else
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, kingMoves, blackKingPositions);
		}
	}

	virtual void switchPiecePositions() override
	{
		Pieces::switchPiecePositions(whiteKingPositions);
		Pieces::switchPiecePositions(blackKingPositions);
	}
	
	virtual int drawPieceOutline(const glm::vec3* color, bool isWhite) const override
	{
		int val;
		if (isWhite)
		{
			val = Pieces::pieceColorIndex(whiteKingColors, whiteKingColors.size(), color);
		}
		else
		{
			val = Pieces::pieceColorIndex(blackKingColors, blackKingColors.size(), color);
		}
		if (val != -1)
		{
			kingOutline(val, isWhite);
			return val;
		}
		return -1;
	}
	
	virtual const glm::vec2& getPiecePosition(int index, bool isWhite) const override
	{
		if (isWhite)
		{
			return whiteKingPositions[index];
		}
		else
		{
			return blackKingPositions[index];
		}
	}

	virtual void deletePiecePosition(int index, bool isWhite, bool isPawnPromote = false) override
	{
		if (isWhite)
		{
			blackKingPositions[index] = glm::vec2(2.0, 2.0);
		}
		else
		{
			whiteKingPositions[index] = glm::vec2(2.0, 2.0);
		}
	}

	virtual void calcDifferenceBetweenSquares(const glm::vec2 &targetSquare, int index, bool isWhite) override
	{
		if (isWhite)
		{
			diffBetweenSquares = targetSquare - whiteKingPositions[index];
		}
		else
		{
			diffBetweenSquares = targetSquare - blackKingPositions[index];
		}
	}
		
	virtual bool isKing(Pieces* piece) override
	{
		return true;
	}
	
	virtual ~King() = default;

private:
	unsigned int blackKing;
	unsigned int whiteKing;

	std::vector<glm::vec3> whiteKingColors;
	std::vector<glm::vec3> blackKingColors;
	std::vector<PieceAttribs> targetSquares;
	glm::vec2 diffBetweenSquares;
	Moves moves;
	std::vector<glm::vec2> kingMoves;

	int targetPieceIndex;
	glm::vec3 targetSquareColor{ 0.0, 0.8, 1.0 };
	glm::vec3 enemySquareColor{ 1.0, 0.0, 0.0 };
	bool pieceExists;

	void setInitKingPositions()
	{
		setKingMoves();
		Pieces::setupPositions(whiteKingPositions, whiteKingColors, 1, 4);
		Pieces::setupPositions(blackKingPositions, blackKingColors, 1, 60);
	}

	virtual void setupPieceOnSquareValues() override
	{
		PieceAttribs square;
		for (size_t i = 0; i < whiteKingPositions.size(); i++)
		{
			square.position = whiteKingPositions[i];
			square.index = i;
			square.isWhite = true;
			square.piece = this;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}

		for (size_t i = 0; i < blackKingPositions.size(); i++)
		{
			square.position = blackKingPositions[i];
			square.index = i;
			square.isWhite = false;
			square.piece = this;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}
	}
	
	void setKingMoves()
	{
		kingMoves.push_back(glm::vec2(0.25, 0.0));
		kingMoves.push_back(glm::vec2(0.0, 0.25));
		kingMoves.push_back(glm::vec2(0.25, 0.25));
		kingMoves.push_back(glm::vec2(-0.25, 0.25));
		kingMoves.push_back(-kingMoves[0]);
		kingMoves.push_back(-kingMoves[1]);
		kingMoves.push_back(-kingMoves[2]);
		kingMoves.push_back(-kingMoves[3]);
	}
};

std::vector<glm::vec2> King::whiteKingPositions{};
std::vector<glm::vec2> King::blackKingPositions{};
int King::attackingPieces = 0;

#endif
