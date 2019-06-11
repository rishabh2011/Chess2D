/* -------------------------------------------------------------------------------------------------------------------
  Universal rules functions have been commented in pawn rules class. Only piece specific rules commented in this class
  -------------------------------------------------------------------------------------------------------------------- */
#ifndef ROOKRULES_H
#define ROOKRULES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Pieces.h>
#include <Board.h>

class RookRules
{
public:

	bool attackingSquare(glm::vec2 &targetSquare, int pieceIndex, const std::vector<glm::vec2> &rookPositions, bool isWhite, const glm::vec2 &kingPosition, bool &kingAttacked)
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

	//Checks if movement is blocked by another piece along a direction
	//----------------------------------------------------------------
	bool movementBlocked(const glm::vec2 &targetSquare, const std::vector<glm::vec2> &rookPositions, int pieceIndex)
	{
		//Forward movement
		if (targetSquare.y > rookPositions[pieceIndex].y)
		{
			return pieceBlockingForwardMovement;
		}
		//Backward movement
		else if (targetSquare.y < rookPositions[pieceIndex].y)
		{
			return pieceBlockingBackwardMovement;
		}
		//Left movement
		else if (targetSquare.x < rookPositions[pieceIndex].x)
		{
			return pieceBlockingLeftMovement;
		}
		//Right movement
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
			Pieces::kingAttackingPieces++;
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

	//If rook is attacking opponent king, this function finds the direction of the king from the rook 
	//and sets all the squaresAttacked attackingOpponentKing values along that directions as true upto the king itself
	//----------------------------------------------------------------------------------------------------------------
	void setSquaresAttacked(std::vector<PieceAttribs> &targetSquares, const std::vector<glm::vec2> &rookPositions, int pieceIndex, const glm::vec2 &kingPosition)
	{
		if (kingPosition.x > rookPositions[pieceIndex].x && kingPosition.y == rookPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x > rookPositions[pieceIndex].x && targetSquares[i].position.y == rookPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if(kingPosition.x < rookPositions[pieceIndex].x && kingPosition.y == rookPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x < rookPositions[pieceIndex].x && targetSquares[i].position.y == rookPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x == rookPositions[pieceIndex].x && kingPosition.y > rookPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x == rookPositions[pieceIndex].x && targetSquares[i].position.y > rookPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
		else if (kingPosition.x == rookPositions[pieceIndex].x && kingPosition.y < rookPositions[pieceIndex].y)
		{
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				if (targetSquares[i].position.x == rookPositions[pieceIndex].x && targetSquares[i].position.y < rookPositions[pieceIndex].y && targetSquares[i].position != kingPosition)
				{
					targetSquares[i].attackingOpponentKing = true;
				}
			}
		}
	}

	bool isValidSquare(glm::vec2 &targetSquare, Pieces* &enemyPiece, int pieceIndex, const std::vector<glm::vec2> &rookPositions, bool isWhite)
	{
		//If movement is blocked along a direction then return false
		if (movementBlocked(targetSquare, rookPositions, pieceIndex))
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
						//If a piece is on the target square set movement blocked along that direction
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
	
	bool piecePinned(const std::vector<glm::vec2> &rookPositions, int pieceIndex, const std::vector<Pieces*> &pieces, bool isWhite)
	{
		tempSquaresAttacked.clear();
		tempPieceOnSquare.clear();
		tempKingAttackingPieces = Pieces::kingAttackingPieces;
		tempKingAttacked = Pieces::kingAttacked;
		tempPieceOnSquare.insert(tempPieceOnSquare.end(), Pieces::pieceOnSquare.begin(), Pieces::pieceOnSquare.end());
		tempSquaresAttacked.insert(tempSquaresAttacked.end(), Pieces::squaresAttacked.begin(), Pieces::squaresAttacked.end());
		for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
		{
			if (rookPositions[pieceIndex] == Pieces::pieceOnSquare[i].position)
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
				if (rookPositions[pieceIndex] == Pieces::squaresAttacked[i].position)
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
			
	void setDefaultPieceBlockValues(bool val)
	{
		pieceBlockingForwardMovement = val;
		pieceBlockingRightMovement = val;
		pieceBlockingLeftMovement = val;
		pieceBlockingBackwardMovement = val;
	}

private:
	//F : Forward, D : Diagonal, R : Right, L : Left
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
