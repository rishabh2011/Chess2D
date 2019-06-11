/* -------------------------------------------------------------------------------------------------------------------
  Universal rules functions have been commented in pawn rules class. Only piece specific rules commented in this class
  -------------------------------------------------------------------------------------------------------------------- */
#ifndef KNIGHTRULES_H
#define KNIGHTRULES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Pieces.h>
#include <Board.h>

class KnightRules
{
public:

	bool attackingSquare(glm::vec2 &targetSquare, int pieceIndex, const std::vector<glm::vec2> &knightPositions, bool isWhite, const glm::vec2 &kingPosition)
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

	void checkIfAttackingOpponentKing(const glm::vec2 &targetSquare, const std::vector<glm::vec2> knightPositions, int pieceIndex, bool isWhite, const glm::vec2 &kingPosition)
	{
		if (targetSquare == kingPosition)
		{
			Pieces::kingAttacked = true;
			Pieces::kingAttackingPieces++;
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

	bool piecePinned(const std::vector<glm::vec2> &knightPositions, int pieceIndex, const std::vector<Pieces*> &pieces, bool isWhite)
	{
		tempSquaresAttacked.clear();
		tempPieceOnSquare.clear();
		tempKingAttackingPieces = Pieces::kingAttackingPieces;
		tempKingAttacked = Pieces::kingAttacked;
		tempPieceOnSquare.insert(tempPieceOnSquare.end(), Pieces::pieceOnSquare.begin(), Pieces::pieceOnSquare.end());
		tempSquaresAttacked.insert(tempSquaresAttacked.end(), Pieces::squaresAttacked.begin(), Pieces::squaresAttacked.end());
		for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
		{
			if (knightPositions[pieceIndex] == Pieces::pieceOnSquare[i].position)
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
				if (knightPositions[pieceIndex] == Pieces::squaresAttacked[i].position)
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
			
private:
	int tempKingAttackingPieces;
	bool tempKingAttacked;
	std::vector<PieceAttribs> tempSquaresAttacked;
	std::vector<PieceAttribs> tempPieceOnSquare;
};

#endif
