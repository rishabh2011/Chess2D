#ifndef QUEENRULES_H
#define QUEENRULES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Files/Classes/Pieces.h"
#include "Files/Classes/Board.h"

class QueenRules
{
public:

	bool squareUnderAttack(glm::vec2 &targetSquare, int pieceIndex, const std::vector<glm::vec2> &queenPositions, bool isWhite, const glm::vec2 &kingPosition)
	{
		if (movementBlocked(targetSquare, queenPositions, pieceIndex))
		{
			return false;
		}
		if (insideBoard(targetSquare))
		{
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (targetSquare == Pieces::pieceOnSquare[i].position)
				{
					checkIfAttackingOpponentKing(targetSquare, queenPositions, pieceIndex, isWhite, kingPosition);
					setPieceBlock(targetSquare, queenPositions, pieceIndex);
					return true;
				}
			}
			return true;
		}
		return false;
	}

	void setSquaresAttacked(std::vector<PieceAttribs> &targetSquares, const std::vector<glm::vec2> &queenPositions, int pieceIndex, const glm::vec2 &kingPosition)
	{
		if (kingPosition.x > queenPositions[pieceIndex].x && kingPosition.y == queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x > queenPositions[pieceIndex].x && targetSquares[i].position.y == queenPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x < queenPositions[pieceIndex].x && kingPosition.y == queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x < queenPositions[pieceIndex].x && targetSquares[i].position.y == queenPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x == queenPositions[pieceIndex].x && kingPosition.y > queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x == queenPositions[pieceIndex].x && targetSquares[i].position.y > queenPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x == queenPositions[pieceIndex].x && kingPosition.y < queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x == queenPositions[pieceIndex].x && targetSquares[i].position.y < queenPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x > queenPositions[pieceIndex].x && kingPosition.y > queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x > queenPositions[pieceIndex].x && targetSquares[i].position.y > queenPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x < queenPositions[pieceIndex].x && kingPosition.y < queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x < queenPositions[pieceIndex].x && targetSquares[i].position.y < queenPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x > queenPositions[pieceIndex].x && kingPosition.y < queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x > queenPositions[pieceIndex].x && targetSquares[i].position.y < queenPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x < queenPositions[pieceIndex].x && kingPosition.y > queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x < queenPositions[pieceIndex].x && targetSquares[i].position.y > queenPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
	}


	bool isValidSquare(glm::vec2 &targetSquare, Pieces* &enemyPiece, int pieceIndex, const std::vector<glm::vec2> &queenPositions, bool isWhite)
	{
		if (movementBlocked(targetSquare, queenPositions, pieceIndex))
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
						setPieceBlock(targetSquare, queenPositions, pieceIndex);
						return true;
					}
					else
					{
						setPieceBlock(targetSquare, queenPositions, pieceIndex);
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

	bool movementBlocked(const glm::vec2 &targetSquare, const std::vector<glm::vec2> &queenPositions, int pieceIndex)
	{
		if (diagonalMovementBlocked(targetSquare, queenPositions, pieceIndex))
		{
			return true;
		}
		if (straightMovementBlocked(targetSquare, queenPositions, pieceIndex))
		{
			return true;
		}
		return false;
	}

	void checkIfAttackingOpponentKing(const glm::vec2 &targetSquare, const std::vector<glm::vec2> queenPositions, int pieceIndex, bool isWhite, const glm::vec2 &kingPosition)
	{
		if (targetSquare == kingPosition)
		{
			Pieces::kingAttacked = true;
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (Pieces::pieceOnSquare[i].position == queenPositions[pieceIndex])
				{
					Pieces::pieceOnSquare[i].attackingOpponentKing = true;
					break;
				}
			}
		}
	}

	bool straightMovementBlocked(const glm::vec2 & targetSquare, const std::vector<glm::vec2> & queenPositions, int pieceIndex)
	{
		if (targetSquare.y > queenPositions[pieceIndex].y && targetSquare.x == queenPositions[pieceIndex].x)
		{
			return pieceBlockingForwardMovement;
		}
		else if (targetSquare.y < queenPositions[pieceIndex].y && targetSquare.x == queenPositions[pieceIndex].x)
		{
			return pieceBlockingBackwardMovement;
		}
		else if (targetSquare.x < queenPositions[pieceIndex].x && targetSquare.y == queenPositions[pieceIndex].y)
		{
			return pieceBlockingLeftMovement;
		}
		else if (targetSquare.x > queenPositions[pieceIndex].x && targetSquare.y == queenPositions[pieceIndex].y)
		{
			return pieceBlockingRightMovement;
		}
		return false;
	}

	bool diagonalMovementBlocked(const glm::vec2 & targetSquare, const std::vector<glm::vec2> & queenPositions, int pieceIndex)
	{
		if (targetSquare.x > queenPositions[pieceIndex].x && targetSquare.y > queenPositions[pieceIndex].y)
		{
			return pieceBlockingFDRMovement;
		}
		else if (targetSquare.x < queenPositions[pieceIndex].x && targetSquare.y > queenPositions[pieceIndex].y)
		{
			return pieceBlockingFDLMovement;
		}
		else if (targetSquare.x < queenPositions[pieceIndex].x && targetSquare.y < queenPositions[pieceIndex].y)
		{
			return pieceBlockingBDLMovement;
		}
		else if (targetSquare.x > queenPositions[pieceIndex].x && targetSquare.y < queenPositions[pieceIndex].y)
		{
			return pieceBlockingBDRMovement;
		}
		return false;
	}

	void setPieceBlock(const glm::vec2 &targetSquare, const std::vector<glm::vec2> &queenPositions, int pieceIndex)
	{
		setDiagonalSquaresPieceBlock(targetSquare, queenPositions, pieceIndex);
		setStraightSquaresPieceBlock(targetSquare, queenPositions, pieceIndex);
	}

	void setStraightSquaresPieceBlock(const glm::vec2 & targetSquare, const std::vector<glm::vec2> & queenPositions, int pieceIndex)
	{
		if (targetSquare.y > queenPositions[pieceIndex].y && targetSquare.x == queenPositions[pieceIndex].x)
		{
			pieceBlockingForwardMovement = true;
		}
		else if (targetSquare.y < queenPositions[pieceIndex].y && targetSquare.x == queenPositions[pieceIndex].x)
		{
			pieceBlockingBackwardMovement = true;
		}
		else if (targetSquare.x < queenPositions[pieceIndex].x && targetSquare.y == queenPositions[pieceIndex].y)
		{
			pieceBlockingLeftMovement = true;
		}
		else if (targetSquare.x > queenPositions[pieceIndex].x && targetSquare.y == queenPositions[pieceIndex].y)
		{
			pieceBlockingRightMovement = true;
		}
	}

	void setDiagonalSquaresPieceBlock(const glm::vec2 & targetSquare, const std::vector<glm::vec2> & queenPositions, int pieceIndex)
	{
		if (targetSquare.x > queenPositions[pieceIndex].x && targetSquare.y > queenPositions[pieceIndex].y)
		{
			pieceBlockingFDRMovement = true;
		}
		else if (targetSquare.x < queenPositions[pieceIndex].x && targetSquare.y > queenPositions[pieceIndex].y)
		{
			pieceBlockingFDLMovement = true;
		}
		else if (targetSquare.x < queenPositions[pieceIndex].x && targetSquare.y < queenPositions[pieceIndex].y)
		{
			pieceBlockingBDLMovement = true;
		}
		else if (targetSquare.x > queenPositions[pieceIndex].x && targetSquare.y < queenPositions[pieceIndex].y)
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

		pieceBlockingForwardMovement = val;
		pieceBlockingRightMovement = val;
		pieceBlockingLeftMovement = val;
		pieceBlockingBackwardMovement = val;
	}

private:
	//F : Forward, D : Diagonal, R : Right, L : Left
	bool pieceBlockingFDRMovement;
	bool pieceBlockingFDLMovement;
	bool pieceBlockingBDRMovement;
	bool pieceBlockingBDLMovement;

	bool pieceBlockingForwardMovement;
	bool pieceBlockingRightMovement;
	bool pieceBlockingLeftMovement;
	bool pieceBlockingBackwardMovement;
};

#endif
