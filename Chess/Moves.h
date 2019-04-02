#ifndef MOVES_H
#define MOVES_H

#include "Files/Classes/Pieces.h"
#include "PawnRules.h"
#include "RookRules.h"
#include "KnightRules.h"
#include "BishopRules.h"
#include "QueenRules.h"
#include "KingRules.h"

class Moves
{
public:

	void calcPawnTargetSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, const std::vector<glm::vec2> &positions, std::vector<glm::vec2> initPawnPositions, bool isWhite)
	{
		//Invert x values for initial pawn positions when black as x values are switched for black pawns
		if(!isWhite)
		{
			for (size_t i = 0; i < initPawnPositions.size(); i++)
			{
				initPawnPositions[i].x = -initPawnPositions[i].x;
			}
		}
		PieceAttribs squares;
		pawnRules.setPieceBlockingMovement(false);
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (pawnRules.isValidSquare(targetSquare, enemyPiece, pieceIndex, positions, initPawnPositions, isWhite))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
	}

	void calcPawnAttackedSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, const std::vector<glm::vec2> &positions, bool isWhite, const glm::vec2 &kingPosition)
	{
		PieceAttribs squares;
		pawnRules.setPieceBlockingMovement(false);
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (pawnRules.squareUnderAttack(targetSquare, pieceIndex, positions, isWhite, kingPosition))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
	}

	void calcRookTargetSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite)
	{
		rookRules.setDefaultBlockValues(false);
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (rookRules.isValidSquare(targetSquare, enemyPiece, pieceIndex, positions, isWhite))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
	}

	void calcRookAttackedSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite, const glm::vec2 &kingPosition)
	{
		bool kingAttacked = false;
		rookRules.setDefaultBlockValues(false);
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (rookRules.squareUnderAttack(targetSquare, pieceIndex, positions, isWhite, kingPosition, kingAttacked))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
		if (kingAttacked)
		{
			rookRules.setSquaresAttacked(targetSquares, positions, pieceIndex, kingPosition);
		}
	}

	void calcKnightTargetSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite)
	{
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (knightRules.isValidSquare(targetSquare, enemyPiece, isWhite))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
	}

	void calcKnightAttackedSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite, const glm::vec2 &kingPosition)
	{
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (knightRules.squareUnderAttack(targetSquare, pieceIndex, positions, isWhite, kingPosition))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
	}

	void calcBishopTargetSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite)
	{
		bishopRules.setDefaultBlockValues(false);
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (bishopRules.isValidSquare(targetSquare, enemyPiece, pieceIndex, positions, isWhite))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
	}

	void calcBishopAttackedSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite, const glm::vec2 &kingPosition)
	{
		bool kingAttacked = false;
		bishopRules.setDefaultBlockValues(false);
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (bishopRules.squareUnderAttack(targetSquare, pieceIndex, positions, isWhite, kingPosition, kingAttacked))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
		if (kingAttacked)
		{
			bishopRules.setSquaresAttacked(targetSquares, positions, pieceIndex, kingPosition);
		}
	}

	void calcQueenTargetSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite)
	{
		queenRules.setDefaultBlockValues(false);
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (queenRules.isValidSquare(targetSquare, enemyPiece, pieceIndex, positions, isWhite))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
	}

	void calcQueenAttackedSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite, const glm::vec2 &kingPosition)
	{
		queenRules.setDefaultBlockValues(false);
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (queenRules.squareUnderAttack(targetSquare, pieceIndex, positions, isWhite, kingPosition))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
	}

	void calcKingTargetSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite)
	{
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (kingRules.isValidSquare(targetSquare, enemyPiece, isWhite))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
	}

	void calcKingAttackedSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite)
	{
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (kingRules.squareUnderAttack(targetSquare, isWhite))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
	}

	void movePiece(const glm::vec2& targetSquare, const glm::vec2 &diffBetweenSquares, int pieceIndex, float deltaTime, const std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions)
	{
		positions[pieceIndex] += diffBetweenSquares * deltaTime * moveSpeed;
		//If piece position exceeds targetSquare set piece position to targetSquare
		if (diffBetweenSquares.x >= 0)
		{
			if (positions[pieceIndex].x >= targetSquare.x)
			{
				positions[pieceIndex].x = targetSquare.x;
			}
		}
		if (diffBetweenSquares.x < 0)
		{
			if (positions[pieceIndex].x <= targetSquare.x)
			{
				positions[pieceIndex].x = targetSquare.x;
			}
		}
		if (diffBetweenSquares.y >= 0)
		{
			if (positions[pieceIndex].y >= targetSquare.y)
			{
				positions[pieceIndex].y = targetSquare.y;
			}
		}
		if (diffBetweenSquares.y < 0)
		{
			if (positions[pieceIndex].y <= targetSquare.y)
			{
				positions[pieceIndex].y = targetSquare.y;
			}
		}
	}

private:

	Pieces* enemyPiece;
	PawnRules pawnRules;
	RookRules rookRules;
	KnightRules knightRules;
	BishopRules bishopRules;
	QueenRules queenRules;
	KingRules kingRules;
	float moveSpeed = 2.0f;
};

#endif
