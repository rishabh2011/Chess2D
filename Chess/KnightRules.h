#ifndef KNIGHTRULES_H
#define KNIGHTRULES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Files/Classes/Pieces.h"
#include "Files/Classes/Board.h"

class KnightRules
{
public:

	bool squareUnderAttack(glm::vec2 &targetSquare, int pieceIndex, const std::vector<glm::vec2> &knightPositions, bool isWhite, const glm::vec2 &kingPosition)
	{
		if (insideBoard(targetSquare))
		{
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (targetSquare == Pieces::pieceOnSquare[i].position)
				{
					checkIfAttackingOpponentKing(targetSquare, knightPositions, pieceIndex, isWhite, kingPosition);
					return true;
				}
			}
			return true;
		}
		return false;
	}

	bool isValidSquare(glm::vec2 &targetSquare, Pieces* &enemyPiece, bool isWhite)
	{
		if (insideBoard(targetSquare))
		{
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (targetSquare == Pieces::pieceOnSquare[i].position)
				{
					if (isWhite != Pieces::pieceOnSquare[i].isWhite)
					{
						enemyPiece = Pieces::pieceOnSquare[i].piece;
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			return true;
		}
		return false;
	}

	bool insideBoard(const glm::vec2 &targetSquare)
	{
		//X and Y limits of board
		if (targetSquare.x > 0.875 || targetSquare.x < -0.875 || targetSquare.y > 0.875 || targetSquare.y < -0.875)
		{
			return false;
		}
		return true;
	}

	void checkIfAttackingOpponentKing(const glm::vec2 &targetSquare, const std::vector<glm::vec2> knightPositions, int pieceIndex, bool isWhite, const glm::vec2 &kingPosition)
	{
		if (targetSquare == kingPosition)
		{
			Pieces::kingAttacked = true;
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (Pieces::pieceOnSquare[i].position == knightPositions[pieceIndex])
				{
					Pieces::pieceOnSquare[i].attackingOpponentKing = true;
					break;
				}
			}
		}
	}
};

#endif
