/* -------------------------------------------------------------------------------------------------------------------
  Universal rules functions have been commented in pawn rules class. Only piece specific rules commented in this class
  -------------------------------------------------------------------------------------------------------------------- */
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

	bool attackingSquare(glm::vec2 &targetSquare, int pieceIndex, const std::vector<glm::vec2> &bishopPositions, bool isWhite, const glm::vec2 &kingPosition, bool &kingAttacked)
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

	//Checks if movement is blocked by another piece along a direction
	//----------------------------------------------------------------
	bool movementBlocked(const glm::vec2 &targetSquare, const std::vector<glm::vec2> &bishopPositions, int pieceIndex)
	{
		//Forward Right diagonal movement
		if (targetSquare.x > bishopPositions[pieceIndex].x && targetSquare.y > bishopPositions[pieceIndex].y)
		{
			return pieceBlockingFDRMovement;
		}
		//Forward Left diagonal movement
		else if (targetSquare.x < bishopPositions[pieceIndex].x && targetSquare.y > bishopPositions[pieceIndex].y)
		{
			return pieceBlockingFDLMovement;
		}
		//Backward Left diagonal movement
		else if (targetSquare.x < bishopPositions[pieceIndex].x && targetSquare.y < bishopPositions[pieceIndex].y)
		{
			return pieceBlockingBDLMovement;
		}
		//Backward Right diagonal movement
		else if (targetSquare.x > bishopPositions[pieceIndex].x && targetSquare.y < bishopPositions[pieceIndex].y)
		{
			return pieceBlockingBDRMovement;
		}
	}

	void checkIfAttackingOpponentKing(const glm::vec2 &targetSquare, const std::vector<glm::vec2> bishopPositions, int pieceIndex, bool isWhite, const glm::vec2 &kingPosition, bool &kingAttacked)
	{
		if (targetSquare == kingPosition)
		{
			Pieces::kingAttacked = true;
			Pieces::kingAttackingPieces++;
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

	//If bishop is attacking opponent king, this function finds the direction of the king from the bishop 
	//and sets all the squaresAttacked attackingOpponentKing values along that directions as true upto the king itself
	//----------------------------------------------------------------------------------------------------------------
	void setSquaresAttacked(std::vector<PieceAttribs> &targetSquares, const std::vector<glm::vec2> &bishopPositions, int pieceIndex, const glm::vec2 &kingPosition)
	{
		if (kingPosition.x > bishopPositions[pieceIndex].x && kingPosition.y > bishopPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x > bishopPositions[pieceIndex].x && targetSquares[i].position.y > bishopPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x < bishopPositions[pieceIndex].x && kingPosition.y < bishopPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x < bishopPositions[pieceIndex].x && targetSquares[i].position.y < bishopPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x > bishopPositions[pieceIndex].x && kingPosition.y < bishopPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x > bishopPositions[pieceIndex].x && targetSquares[i].position.y < bishopPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x < bishopPositions[pieceIndex].x && kingPosition.y > bishopPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x < bishopPositions[pieceIndex].x && targetSquares[i].position.y > bishopPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
	}

	bool isValidSquare(glm::vec2 &targetSquare, Pieces* &enemyPiece, int pieceIndex, const std::vector<glm::vec2> &bishopPositions, bool isWhite)
	{
		//If movement is blocked along a direction then return false
		if (movementBlocked(targetSquare, bishopPositions, pieceIndex))
		{
			return false;
		}
		if (insideBoard(targetSquare))
		{
			//Check for a piece on the target square
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (targetSquare == Pieces::pieceOnSquare[i].position)
				{
					if (isWhite != Pieces::pieceOnSquare[i].isWhite)
					{
						enemyPiece = Pieces::pieceOnSquare[i].piece;
						//If a piece is on the target square set movement blocked along that direction
						setPieceBlock(targetSquare, bishopPositions, pieceIndex);
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
						//If a piece is on the target square set movement blocked along that direction
						setPieceBlock(targetSquare, bishopPositions, pieceIndex);
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
		if (targetSquare.x > 0.875 || targetSquare.x < -0.875 || targetSquare.y > 0.875 || targetSquare.y < -0.875)
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

	bool piecePinned(const std::vector<glm::vec2> &bishopPositions, int pieceIndex, const std::vector<Pieces*> &pieces, bool isWhite)
	{
		tempSquaresAttacked.clear();
		tempPieceOnSquare.clear();
		tempKingAttackingPieces = Pieces::kingAttackingPieces;
		tempKingAttacked = Pieces::kingAttacked;
		tempPieceOnSquare.insert(tempPieceOnSquare.end(), Pieces::pieceOnSquare.begin(), Pieces::pieceOnSquare.end());
		tempSquaresAttacked.insert(tempSquaresAttacked.end(), Pieces::squaresAttacked.begin(), Pieces::squaresAttacked.end());
		for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
		{
			if (bishopPositions[pieceIndex] == Pieces::pieceOnSquare[i].position)
			{
				Pieces::pieceOnSquare[i].position = glm::vec2(2.0, 2.0);
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
				if (bishopPositions[pieceIndex] == Pieces::squaresAttacked[i].position)
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
	}

private:
	//F : Forward, D : Diagonal, R : Right, L : Left
	bool pieceBlockingFDRMovement;
	bool pieceBlockingFDLMovement;
	bool pieceBlockingBDRMovement;
	bool pieceBlockingBDLMovement;

	int tempKingAttackingPieces;
	bool tempKingAttacked;
	std::vector<PieceAttribs> tempSquaresAttacked;
	std::vector<PieceAttribs> tempPieceOnSquare;
};

#endif
