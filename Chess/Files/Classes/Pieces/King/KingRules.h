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
		if (targetSquare.x > Board::boardLimitX || targetSquare.x < -Board::boardLimitX || targetSquare.y > Board::boardLimitY || targetSquare.y < -Board::boardLimitY)
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

	//--------------------------------------
	//CASTLING SQUARES CALCULATING FUNCTIONS
	//--------------------------------------

	//The following functions check for possibility of castling and accordingly add the castling squares to the king available target squares vector
	//----------------------------------------------------------------------------------------------------------------------------------------------
	void calcCastlingSquares(std::vector<PieceAttribs> &targetSquares, int kingMoved, const std::vector<int> &rookMoved, bool isWhite, const glm::vec2 &kingPosition)
	{
		//If king is attacked castling not possible
		if (Pieces::kingAttacked)
		{
			return;
		}
		//If king has been moved already, castling not possible
		else if (kingMoved > 0)
		{
			return;
		}
		//If the above two conditions are false then check for possibility of castling on either side
		castlingQueenSide(targetSquares, rookMoved, isWhite, kingPosition);
		castlingKingSide(targetSquares, rookMoved, isWhite, kingPosition);
	}

	//Checks for possibility of castling on queen side
	//------------------------------------------------
	void castlingQueenSide(std::vector<PieceAttribs> &targetSquares, const std::vector<int> &rookMoved, bool isWhite, const glm::vec2 &kingPosition)
	{
		//If queen side rook has been moved, castling not possible
		if (rookMoved[0] > 0)
		{
			return;
		}

		std::vector<glm::vec2> squares;
		if (isWhite)
		{
			//Squares between king and rook
			squares.push_back(glm::vec2(kingPosition.x - squareSizeX, kingPosition.y));
			squares.push_back(glm::vec2(kingPosition.x - squareSizeX * 2.0, kingPosition.y));
			squares.push_back(glm::vec2(kingPosition.x - squareSizeX * 3.0, kingPosition.y));
		}
		else
		{
			squares.push_back(glm::vec2(kingPosition.x + squareSizeX, kingPosition.y));
			squares.push_back(glm::vec2(kingPosition.x + squareSizeX * 2.0, kingPosition.y));
			squares.push_back(glm::vec2(kingPosition.x + squareSizeX * 3.0, kingPosition.y));
		}

		//Check if squares are empty
		for (size_t i{ 0 }; i < squares.size(); i++)
		{
			for (size_t j = 0; j < Pieces::pieceOnSquare.size(); j++)
			{
				//If not empty, castling not possible
				if (Pieces::pieceOnSquare[j].position == squares[i])
				{
					return;
				}
			}
		}

		//Check if squares are not attacked by opponent pieces
		for (size_t i{ 0 }; i < squares.size(); i++)
		{
			for (size_t j = 0; j < Pieces::squaresAttacked.size(); j++)
			{
				//If attacked, castling not possible
				if (Pieces::squaresAttacked[j].position == squares[i])
				{
					return;
				}
			}
		}
		PieceAttribs castleSquare;
		castleSquare.position = squares[1];
		castleSquare.piece = nullptr;
		castleSquare.castlingQueenSideSquare = true;
		targetSquares.push_back(castleSquare);
	}

	//Checks for possibility of castling on king side
	//------------------------------------------------
	void castlingKingSide(std::vector<PieceAttribs> &targetSquares, const std::vector<int> &rookMoved, bool isWhite, const glm::vec2 &kingPosition)
	{
		//If king side rook has been moved, castling not possible
		if (rookMoved[1] > 0)
		{
			return;
		}

		std::vector<glm::vec2> squares;
		if (isWhite)
		{
			//Squares between king and rook
			squares.push_back(glm::vec2(kingPosition.x + squareSizeX, kingPosition.y));
			squares.push_back(glm::vec2(kingPosition.x + squareSizeX * 2.0, kingPosition.y));
		}
		else
		{
			squares.push_back(glm::vec2(kingPosition.x - squareSizeX, kingPosition.y));
			squares.push_back(glm::vec2(kingPosition.x - squareSizeX * 2.0, kingPosition.y));
		}

		//Check if squares are empty
		for (size_t i{ 0 }; i < squares.size(); i++)
		{
			for (size_t j = 0; j < Pieces::pieceOnSquare.size(); j++)
			{
				//If not empty, castling not possible
				if (Pieces::pieceOnSquare[j].position == squares[i])
				{
					return;
				}
			}
		}

		//Check if squares are not attacked by opponent pieces
		for (size_t i{ 0 }; i < squares.size(); i++)
		{
			for (size_t j = 0; j < Pieces::squaresAttacked.size(); j++)
			{
				//If attacked, castling not possible
				if (Pieces::squaresAttacked[j].position == squares[i])
				{
					return;
				}
			}
		}
		PieceAttribs castleSquare;
		castleSquare.position = squares[1];
		castleSquare.piece = nullptr;
		castleSquare.castlingKingSideSquare = true;
		targetSquares.push_back(castleSquare);
	}
};

#endif
