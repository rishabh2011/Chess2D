/* -------------------------------------------------------------------------------------------------------------------
  Universal rules functions have been commented in pawn rules class. Only piece specific rules commented in this class
  -------------------------------------------------------------------------------------------------------------------- */
#ifndef KINGTRULES_H
#define KINGTRULES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Pieces.h>
#include <Board.h>

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

	bool isValidSquare(glm::vec2 &targetSquare, Pieces* &enemyPiece, const glm::vec2 &kingPosition, bool isWhite)
	{
		if (insideBoard(targetSquare))
		{
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (targetSquare == Pieces::pieceOnSquare[i].position)
				{
					if (isWhite != Pieces::pieceOnSquare[i].isWhite)
					{
						if (squareSafe(targetSquare, kingPosition))
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
			if (squareSafe(targetSquare, kingPosition))
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
	bool squareSafe(const glm::vec2 &targetSquare, const glm::vec2 &kingPosition)
	{
		for (size_t i{ 0 }; i < Pieces::squaresAttacked.size(); i++)
		{
			if (targetSquare == Pieces::squaresAttacked[i].position)
			{
				return false;
			}
		}
		//The following code will only return false in situations where the king is under attack
		for (size_t i{ 0 }; i < Pieces::pieceOnSquare.size(); i++)
		{
			//If piece is attacking king
			if (Pieces::pieceOnSquare[i].attackingOpponentKing)
			{
				switch (Pieces::pieceOnSquare[i].pieceEnum)
				{
				case Piece::BISHOP:
					if (targetSquareAlongAttackingBishopDiagonal(targetSquare, Pieces::pieceOnSquare[i].position, kingPosition))
					{
						return false;
					}
					break;
				case Piece::ROOK:
					if (targetSqureAlongAttackingRookLine(targetSquare, Pieces::pieceOnSquare[i].position, kingPosition))
					{
						return false;
					}
					break;
				case Piece::QUEEN:
					//Queen is a rook and bishop combined, therefore we can use the bishop and rook functions together
					if (targetSquareAlongAttackingBishopDiagonal(targetSquare, Pieces::pieceOnSquare[i].position, kingPosition))
					{
						return false;
					}
					else if (targetSqureAlongAttackingRookLine(targetSquare, Pieces::pieceOnSquare[i].position, kingPosition))
					{
						return false;
					}
					break;
				}
			}
		}
		return true;
	}

	//The squares attacked vector has no value for squares that are beyond the king's position but lie along the same diagonal as a bishop
	//(This is because for a piece, target squares are not calculated further along the same diagonal / row if another piece is blocking the path)
	//Hence this function takes into account, those squares in particular 
	//--------------------------------------------------------------------------------------------------------------------------------------------
	bool targetSquareAlongAttackingBishopDiagonal(const glm::vec2 &targetSquare, const glm::vec2 &attackingPiecePos, const glm::vec2 &kingPosition)
	{
		if (attackingPiecePos.x > kingPosition.x && attackingPiecePos.y > kingPosition.y)
		{
			if (targetSquare.x < kingPosition.x && targetSquare.y < kingPosition.y)
			{
				return true;
			}
		}
		else if (attackingPiecePos.x > kingPosition.x && attackingPiecePos.y < kingPosition.y)
		{
			if (targetSquare.x < kingPosition.x && targetSquare.y > kingPosition.y)
			{
				return true;
			}
		}
		else if (attackingPiecePos.x < kingPosition.x && attackingPiecePos.y < kingPosition.y)
		{
			if (targetSquare.x > kingPosition.x && targetSquare.y > kingPosition.y)
			{
				return true;
			}
		}
		else if (attackingPiecePos.x < kingPosition.x && attackingPiecePos.y > kingPosition.y)
		{
			if (targetSquare.x > kingPosition.x && targetSquare.y < kingPosition.y)
			{
				return true;
			}
		}
		return false;
	}

	//Function considers squares that lie beyond the king position but along the same line as the attacking Rook
	//----------------------------------------------------------------------------------------------------------
	bool targetSqureAlongAttackingRookLine(const glm::vec2 &targetSquare, const glm::vec2 &attackingPiecePos, const glm::vec2 &kingPosition)
	{
		if (attackingPiecePos.x > kingPosition.x && attackingPiecePos.y == kingPosition.y)
		{
			if (targetSquare.x < kingPosition.x && targetSquare.y == kingPosition.y)
			{
				return true;
			}
		}
		else if (attackingPiecePos.x < kingPosition.x && attackingPiecePos.y == kingPosition.y)
		{
			if (targetSquare.x > kingPosition.x && targetSquare.y == kingPosition.y)
			{
				return true;
			}
		}
		else if (attackingPiecePos.x == kingPosition.x && attackingPiecePos.y > kingPosition.y)
		{
			if (targetSquare.x == kingPosition.x && targetSquare.y < kingPosition.y)
			{
				return true;
			}
		}
		else if (attackingPiecePos.x == kingPosition.x && attackingPiecePos.y < kingPosition.y)
		{
			if (targetSquare.x == kingPosition.x && targetSquare.y > kingPosition.y)
			{
				return true;
			}
		}
		return false;
	}
};

#endif
