/* -------------------------------------------------------------------------------------------------------------------
  Universal rules functions have been commented in pawn rules class. Only piece specific rules commented in this class
  -------------------------------------------------------------------------------------------------------------------- */
#ifndef QUEENRULES_H
#define QUEENRULES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Pieces.h>
#include <Board.h>

class QueenRules
{
public:

	bool attackingSquare(glm::vec2 &targetSquare, int pieceIndex, const std::vector<glm::vec2> &queenPositions, bool isWhite, const glm::vec2 &kingPosition, bool &kingAttacked)
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
					checkIfAttackingOpponentKing(targetSquare, queenPositions, pieceIndex, isWhite, kingPosition, kingAttacked);
					setPieceBlock(targetSquare, queenPositions, pieceIndex);
					return true;
				}
			}
			return true;
		}
		return false;
	}

	//Checks if movement is blocked by another piece along a direction
	//----------------------------------------------------------------
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

	//Check if movement is blocked along straight directions
	//------------------------------------------------------
	bool straightMovementBlocked(const glm::vec2 & targetSquare, const std::vector<glm::vec2> & queenPositions, int pieceIndex)
	{
		//Forward movement
		if (targetSquare.y > queenPositions[pieceIndex].y && targetSquare.x == queenPositions[pieceIndex].x)
		{
			return pieceBlockingForwardMovement;
		}
		//Backward movement
		else if (targetSquare.y < queenPositions[pieceIndex].y && targetSquare.x == queenPositions[pieceIndex].x)
		{
			return pieceBlockingBackwardMovement;
		}
		//Left movement
		else if (targetSquare.x < queenPositions[pieceIndex].x && targetSquare.y == queenPositions[pieceIndex].y)
		{
			return pieceBlockingLeftMovement;
		}
		//Right movement
		else if (targetSquare.x > queenPositions[pieceIndex].x && targetSquare.y == queenPositions[pieceIndex].y)
		{
			return pieceBlockingRightMovement;
		}
		return false;
	}

	//Check if movement is blocked along diagonal directions
	//------------------------------------------------------
	bool diagonalMovementBlocked(const glm::vec2 & targetSquare, const std::vector<glm::vec2> & queenPositions, int pieceIndex)
	{
		//Forward Right movement
		if (targetSquare.x > queenPositions[pieceIndex].x && targetSquare.y > queenPositions[pieceIndex].y)
		{
			return pieceBlockingFDRMovement;
		}
		//Forward Left movement
		else if (targetSquare.x < queenPositions[pieceIndex].x && targetSquare.y > queenPositions[pieceIndex].y)
		{
			return pieceBlockingFDLMovement;
		}
		//Backward Left movement
		else if (targetSquare.x < queenPositions[pieceIndex].x && targetSquare.y < queenPositions[pieceIndex].y)
		{
			return pieceBlockingBDLMovement;
		}
		//Backward Right movement
		else if (targetSquare.x > queenPositions[pieceIndex].x && targetSquare.y < queenPositions[pieceIndex].y)
		{
			return pieceBlockingBDRMovement;
		}
		return false;
	}

	void checkIfAttackingOpponentKing(const glm::vec2 &targetSquare, const std::vector<glm::vec2> queenPositions, int pieceIndex, bool isWhite, const glm::vec2 &kingPosition, bool &kingAttacked)
	{
		if (targetSquare == kingPosition)
		{
			Pieces::kingAttacked = true;
			Pieces::kingAttackingPieces++;
			kingAttacked = true;
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

	//If queen is attacking opponent king, this function finds the direction of the king from the queen 
	//and sets all the squaresAttacked attackingOpponentKing values along that directions as true upto the king itself
	//----------------------------------------------------------------------------------------------------------------
	void setSquaresAttacked(std::vector<PieceAttribs> &targetSquares, const std::vector<glm::vec2> &queenPositions, int pieceIndex, const glm::vec2 &kingPosition)
	{
		if (kingPosition.x > queenPositions[pieceIndex].x && kingPosition.y == queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x > queenPositions[pieceIndex].x && targetSquares[i].position.y == queenPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x < queenPositions[pieceIndex].x && kingPosition.y == queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x < queenPositions[pieceIndex].x && targetSquares[i].position.y == queenPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x == queenPositions[pieceIndex].x && kingPosition.y > queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x == queenPositions[pieceIndex].x && targetSquares[i].position.y > queenPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x == queenPositions[pieceIndex].x && kingPosition.y < queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x == queenPositions[pieceIndex].x && targetSquares[i].position.y < queenPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x > queenPositions[pieceIndex].x && kingPosition.y > queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x > queenPositions[pieceIndex].x && targetSquares[i].position.y > queenPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x < queenPositions[pieceIndex].x && kingPosition.y < queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x < queenPositions[pieceIndex].x && targetSquares[i].position.y < queenPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x > queenPositions[pieceIndex].x && kingPosition.y < queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x > queenPositions[pieceIndex].x && targetSquares[i].position.y < queenPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x < queenPositions[pieceIndex].x && kingPosition.y > queenPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x < queenPositions[pieceIndex].x && targetSquares[i].position.y > queenPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
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
						setPieceBlock(targetSquare, queenPositions, pieceIndex);
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

	bool insideBoard(const glm::vec2 &targetSquare)
	{
		//X and Y limits of board
		if (targetSquare.x > Board::boardLimitX || targetSquare.x < -Board::boardLimitX || targetSquare.y > Board::boardLimitY || targetSquare.y < -Board::boardLimitY)
		{
			return false;
		}
		return true;
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

	//Sets movement along a certain direction as blocked
	//--------------------------------------------------
	void setPieceBlock(const glm::vec2 &targetSquare, const std::vector<glm::vec2> &queenPositions, int pieceIndex)
	{
		setDiagonalSquaresPieceBlock(targetSquare, queenPositions, pieceIndex);
		setStraightSquaresPieceBlock(targetSquare, queenPositions, pieceIndex);
	}

	//Sets movement along straight directions as blocked
	//--------------------------------------------------
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

	//Sets movement along diagonal directions as blocked
	//--------------------------------------------------
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

	bool piecePinned(const std::vector<glm::vec2> &queenPositions, int pieceIndex, const std::vector<Pieces*> &pieces, bool isWhite)
	{
		tempSquaresAttacked.clear();
		tempPieceOnSquare.clear();
		tempKingAttackingPieces = Pieces::kingAttackingPieces;
		tempKingAttacked = Pieces::kingAttacked;
		tempPieceOnSquare.insert(tempPieceOnSquare.end(), Pieces::pieceOnSquare.begin(), Pieces::pieceOnSquare.end());
		tempSquaresAttacked.insert(tempSquaresAttacked.end(), Pieces::squaresAttacked.begin(), Pieces::squaresAttacked.end());
		for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
		{
			if (queenPositions[pieceIndex] == Pieces::pieceOnSquare[i].position)
			{
				Pieces::pieceOnSquare[i].position = glm::vec2(100.0, 100.0);
				break;
			}
		}
		Pieces::squaresAttacked.clear();
		for (size_t i = 0; i < pieces.size(); i++)
		{
			pieces[i]->calcTargetSquares(!isWhite, false);
		}
		Pieces::kingAttackingPieces = tempKingAttackingPieces;
		Pieces::kingAttacked = tempKingAttacked;
		for (size_t i = 0; i < Pieces::squaresAttacked.size(); i++)
		{
			if (Pieces::squaresAttacked[i].attackingOpponentKing)
			{
				if (queenPositions[pieceIndex] == Pieces::squaresAttacked[i].position)
				{
					return true;
				}
			}
		}
		setBackOriginalValues();
		return false;
	}

	bool pinnedPieceValidSquare(const glm::vec2 &targetSquare, bool isWhite, Pieces* &enemyPiece)
	{
		for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
		{
			if (Pieces::pieceOnSquare[i].attackingOpponentKing)
			{
				if (targetSquare == Pieces::pieceOnSquare[i].position)
				{
					enemyPiece = Pieces::pieceOnSquare[i].piece;
					return true;
				}
			}
		}
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
		return false;
	}

	void setBackOriginalValues()
	{
		Pieces::squaresAttacked.clear();
		Pieces::squaresAttacked.insert(Pieces::squaresAttacked.end(), tempSquaresAttacked.begin(), tempSquaresAttacked.end());
		Pieces::pieceOnSquare.clear();
		Pieces::pieceOnSquare.insert(Pieces::pieceOnSquare.end(), tempPieceOnSquare.begin(), tempPieceOnSquare.end());
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

	int tempKingAttackingPieces;
	bool tempKingAttacked;
	std::vector<PieceAttribs> tempSquaresAttacked;
	std::vector<PieceAttribs> tempPieceOnSquare;
};

#endif
