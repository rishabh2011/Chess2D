#ifndef BISHOPRULES_H
#define BISHOPRULES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Files/Classes/Pieces.h"
#include "Files/Classes/Board.h"

class BishopRules
{
public:

	bool squareUnderAttack(glm::vec2 &targetSquare, int pieceIndex, const std::vector<glm::vec2> &bishopPositions, bool isWhite, const glm::vec2 &kingPosition, bool &kingAttacked)
	{
		if (movementBlocked(targetSquare, bishopPositions, pieceIndex))
		{
			return false;
		}
		if (insideBoard(targetSquare))
		{
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (targetSquare == Pieces::pieceOnSquare[i].position)
				{
					checkIfAttackingOpponentKing(targetSquare, bishopPositions, pieceIndex, isWhite, kingPosition, kingAttacked);
					setPieceBlock(targetSquare, bishopPositions, pieceIndex);
					return true;
				}
			}
			return true;
		}
		return false;
	}

	void setSquaresAttacked(std::vector<PieceAttribs> &targetSquares, const std::vector<glm::vec2> &bishopPositions, int pieceIndex, const glm::vec2 &kingPosition)
	{
		if (kingPosition.x > bishopPositions[pieceIndex].x && kingPosition.y > bishopPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x > bishopPositions[pieceIndex].x && targetSquares[i].position.y > bishopPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x < bishopPositions[pieceIndex].x && kingPosition.y < bishopPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x < bishopPositions[pieceIndex].x && targetSquares[i].position.y < bishopPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x > bishopPositions[pieceIndex].x && kingPosition.y < bishopPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x > bishopPositions[pieceIndex].x && targetSquares[i].position.y < bishopPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x < bishopPositions[pieceIndex].x && kingPosition.y > bishopPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x < bishopPositions[pieceIndex].x && targetSquares[i].position.y > bishopPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
	}

	bool isValidSquare(glm::vec2 &targetSquare, Pieces* &enemyPiece, int pieceIndex, const std::vector<glm::vec2> &bishopPositions, bool isWhite)
	{
		if (movementBlocked(targetSquare, bishopPositions, pieceIndex))
		{
			return false;
		}
		if (insideBoard(targetSquare))
		{
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (targetSquare == Pieces::pieceOnSquare[i].position)
				{
					if (isWhite != Pieces::pieceOnSquare[i].isWhite)
					{
						enemyPiece = Pieces::pieceOnSquare[i].piece;
						setPieceBlock(targetSquare, bishopPositions, pieceIndex);
						return true;
					}
					else
					{
						setPieceBlock(targetSquare, bishopPositions, pieceIndex);
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

	bool movementBlocked(const glm::vec2 &targetSquare, const std::vector<glm::vec2> &bishopPositions, int pieceIndex)
	{
		if (targetSquare.x > bishopPositions[pieceIndex].x && targetSquare.y > bishopPositions[pieceIndex].y)
		{
			return pieceBlockingFDRMovement;
		}
		else if (targetSquare.x < bishopPositions[pieceIndex].x && targetSquare.y > bishopPositions[pieceIndex].y)
		{
			return pieceBlockingFDLMovement;
		}
		else if (targetSquare.x < bishopPositions[pieceIndex].x && targetSquare.y < bishopPositions[pieceIndex].y)
		{
			return pieceBlockingBDLMovement;
		}
		else if (targetSquare.x > bishopPositions[pieceIndex].x && targetSquare.y < bishopPositions[pieceIndex].y)
		{
			return pieceBlockingBDRMovement;
		}
	}

	void checkIfAttackingOpponentKing(const glm::vec2 &targetSquare, const std::vector<glm::vec2> bishopPositions, int pieceIndex, bool isWhite, const glm::vec2 &kingPosition, bool kingAttacked)
	{
		if (targetSquare == kingPosition)
		{
			Pieces::kingAttacked = true;
			kingAttacked = true;
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (Pieces::pieceOnSquare[i].position == bishopPositions[pieceIndex])
				{
					Pieces::pieceOnSquare[i].attackingOpponentKing = true;
					break;
				}
			}
		}
	}

	void setPieceBlock(const glm::vec2 &targetSquare, const std::vector<glm::vec2> &bishopPositions, int pieceIndex)
	{
		if (targetSquare.x > bishopPositions[pieceIndex].x && targetSquare.y > bishopPositions[pieceIndex].y)
		{
			pieceBlockingFDRMovement = true;
		}
		else if (targetSquare.x < bishopPositions[pieceIndex].x && targetSquare.y > bishopPositions[pieceIndex].y)
		{
			pieceBlockingFDLMovement = true;
		}
		else if (targetSquare.x < bishopPositions[pieceIndex].x && targetSquare.y < bishopPositions[pieceIndex].y)
		{
			pieceBlockingBDLMovement = true;
		}
		else if (targetSquare.x > bishopPositions[pieceIndex].x && targetSquare.y < bishopPositions[pieceIndex].y)
		{
			pieceBlockingBDRMovement = true;
		}
	}

	void setDefaultBlockValues(bool val)
	{
		pieceBlockingFDRMovement = val;
		pieceBlockingFDLMovement = val;
		pieceBlockingBDRMovement = val;
		pieceBlockingBDLMovement = val;
	}

private:
	//F : Forward, D : Diagonal, R : Right, L : Left
	bool pieceBlockingFDRMovement;
	bool pieceBlockingFDLMovement;
	bool pieceBlockingBDRMovement;
	bool pieceBlockingBDLMovement;

};

#endif
