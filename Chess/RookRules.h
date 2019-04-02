#ifndef ROOKRULES_H
#define ROOKRULES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Files/Classes/Pieces.h"
#include "Files/Classes/Board.h"

class RookRules
{
public:

	bool squareUnderAttack(glm::vec2 &targetSquare, int pieceIndex, const std::vector<glm::vec2> &rookPositions, bool isWhite, const glm::vec2 &kingPosition, bool &kingAttacked)
	{
		if (movementBlocked(targetSquare, rookPositions, pieceIndex))
		{
			return false;
		}
		if (insideBoard(targetSquare))
		{
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (targetSquare == Pieces::pieceOnSquare[i].position)
				{
					checkIfAttackingOpponentKing(targetSquare, rookPositions, pieceIndex, isWhite, kingPosition, kingAttacked);
					setPieceBlock(targetSquare, rookPositions, pieceIndex);
					return true;
				}
			}
			return true;
		}
		return false;
	}

	void setSquaresAttacked(std::vector<PieceAttribs> &targetSquares, const std::vector<glm::vec2> &rookPositions, int pieceIndex, const glm::vec2 &kingPosition)
	{
		if (kingPosition.x > rookPositions[pieceIndex].x && kingPosition.y == rookPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x > rookPositions[pieceIndex].x && targetSquares[i].position.y == rookPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if(kingPosition.x < rookPositions[pieceIndex].x && kingPosition.y == rookPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x < rookPositions[pieceIndex].x && targetSquares[i].position.y == rookPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x == rookPositions[pieceIndex].x && kingPosition.y > rookPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x == rookPositions[pieceIndex].x && targetSquares[i].position.y > rookPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x == rookPositions[pieceIndex].x && kingPosition.y < rookPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x == rookPositions[pieceIndex].x && targetSquares[i].position.y < rookPositions[pieceIndex].y)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
	}

	bool isValidSquare(glm::vec2 &targetSquare, Pieces* &enemyPiece, int pieceIndex, const std::vector<glm::vec2> &rookPositions, bool isWhite)
	{
		if (movementBlocked(targetSquare, rookPositions, pieceIndex))
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
						setPieceBlock(targetSquare, rookPositions, pieceIndex);
						if (Pieces::kingAttacked)
						{
							if (preventingCheck(targetSquare))
							{
								return true;
							}
							else
							{
								return false;
							}
						}
						return true;
					}
					else
					{
						setPieceBlock(targetSquare, rookPositions, pieceIndex);
						return false;
					}
				}
			}
			if (Pieces::kingAttacked)
			{
				if (preventingCheck(targetSquare))
				{
					return true;
				}
				else
				{
					return false;
				}
			}
			return true;
		}
		return false;
	}

	bool preventingCheck(const glm::vec2 &targetSquare)
	{
		for (size_t i = 0; i < Pieces::squaresAttacked.size(); i++)
		{
			if (Pieces::squaresAttacked[i].attackingOpponentKing)
			{
				if (targetSquare == Pieces::squaresAttacked[i].position)
				{
					return true;
				}
			}
		}
		for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
		{
			if (Pieces::pieceOnSquare[i].attackingOpponentKing)
			{
				if (targetSquare == Pieces::pieceOnSquare[i].position)
				{
					return true;
				}
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

	bool movementBlocked(const glm::vec2 &targetSquare, const std::vector<glm::vec2> &rookPositions, int pieceIndex)
	{
		if (targetSquare.y > rookPositions[pieceIndex].y)
		{
			return pieceBlockingForwardMovement;
		}
		else if (targetSquare.y < rookPositions[pieceIndex].y)
		{
			return pieceBlockingBackwardMovement;
		}
		else if (targetSquare.x < rookPositions[pieceIndex].x)
		{
			return pieceBlockingLeftMovement;
		}
		else if (targetSquare.x > rookPositions[pieceIndex].x)
		{
			return pieceBlockingRightMovement;
		}
	}

	void checkIfAttackingOpponentKing(const glm::vec2 &targetSquare, const std::vector<glm::vec2> rookPositions, int pieceIndex, bool isWhite, const glm::vec2 &kingPosition, bool &kingAttacked)
	{
		if (targetSquare == kingPosition)
		{
			Pieces::kingAttacked = true;
			kingAttacked = true;
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (Pieces::pieceOnSquare[i].position == rookPositions[pieceIndex])
				{
					Pieces::pieceOnSquare[i].attackingOpponentKing = true;
					break;
				}
			}
		}
	}

	void setPieceBlock(const glm::vec2 &targetSquare, const std::vector<glm::vec2> &rookPositions, int pieceIndex)
	{
		if (targetSquare.y > rookPositions[pieceIndex].y)
		{
			pieceBlockingForwardMovement = true;
		}
		else if (targetSquare.y < rookPositions[pieceIndex].y)
		{
			pieceBlockingBackwardMovement = true;
		}
		else if (targetSquare.x < rookPositions[pieceIndex].x)
		{
			pieceBlockingLeftMovement = true;
		}
		else if (targetSquare.x > rookPositions[pieceIndex].x)
		{
			pieceBlockingRightMovement = true;
		}
	}

	void setDefaultBlockValues(bool val)
	{
		pieceBlockingForwardMovement = val;
		pieceBlockingRightMovement = val;
		pieceBlockingLeftMovement = val;
		pieceBlockingBackwardMovement = val;
	}

private:
	bool pieceBlockingForwardMovement;
	bool pieceBlockingRightMovement;
	bool pieceBlockingLeftMovement;
	bool pieceBlockingBackwardMovement;
};

#endif
