/* -------------------------------------------------------------------------------------------------------------------
  Universal rules functions have been commented in pawn rules class. Only piece specific rules commented in this class
  -------------------------------------------------------------------------------------------------------------------- */
#ifndef KINGTRULES_H
#define KINGTRULES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Files/Classes/Pieces.h"
#include "Files/Classes/Board.h"

class KingRules
{
public:

	bool attackingSquare(glm::vec2 &targetSquare, bool isWhite)
	{
		if (insideBoard(targetSquare))
		{
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (targetSquare == Pieces::pieceOnSquare[i].position)
				{
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
						if (squareSafe(targetSquare))
						{
							enemyPiece = Pieces::pieceOnSquare[i].piece;
							return true;
						}
					}
					else
					{
						return false;
					}
				}
			}
			if (squareSafe(targetSquare))
			{
				return true;
			}
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

	//Functions checks if the target square is safe for the king to move to 
	//i.e. the target square is not being attacked by an opponent piece
	bool squareSafe(const glm::vec2 &targetSquare)
	{
		for (size_t i{ 0 }; i < Pieces::squaresAttacked.size(); i++)
		{
			if (targetSquare == Pieces::squaresAttacked[i].position)
			{
				return false;
			}
		}
		return true;
	}
};

#endif
