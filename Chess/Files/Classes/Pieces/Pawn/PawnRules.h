/* ---------------------------------------------------------------------------------------------------------------------------------------------
  Class contains all the necessary conditions for determining valid moves for pawn. Universal piece function comments apply to all rules classes
  ---------------------------------------------------------------------------------------------------------------------------------------------- */
#ifndef PAWNRULES_H
#define PAWNRULES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Pieces.h>
#include <Board.h>
#include <UndoMoves.h>

class PawnRules
{
public:

	//Check if the target square is being threatened by the pawn
	//----------------------------------------------------------
	bool attackingSquare(glm::vec2 &targetSquare, int pieceIndex, const std::vector<glm::vec2> &pawnPositions, bool isWhite, const glm::vec2 &kingPosition)
	{
		//Target square must be inside the board; discard squares outside the board
		if (insideBoard(targetSquare))
		{
			//Only Killing moves for pawn can be considered as threatening
			if (targetSquare.x != pawnPositions[pieceIndex].x)
			{
				checkIfAttackingOpponentKing(targetSquare, pawnPositions, pieceIndex, isWhite, kingPosition);
				return true;
			}
			return false;
		}
		return false;
	}

	//Checks wether the opponent king is on the target square
	//-------------------------------------------------------
	void checkIfAttackingOpponentKing(const glm::vec2 &targetSquare, const std::vector<glm::vec2> pawnPositions, int pieceIndex, bool isWhite, const glm::vec2 &kingPosition)
	{
		if (targetSquare == kingPosition)
		{
			Pieces::kingAttacked = true;
			Pieces::kingAttackingPieces++;
			//Iterate through pieceOnSquare vector to get the index where the currently selected piece attribs are stored
			//and set that piece as attacking piece
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (Pieces::pieceOnSquare[i].position == pawnPositions[pieceIndex])
				{
					Pieces::pieceOnSquare[i].attackingOpponentKing = true;
					break;
				}
			}
		}
	}

	//Checks if the target square is valid i.e. can be moved to by checking all relevant conditions
	//---------------------------------------------------------------------------------------------
	bool isValidSquare(glm::vec2 &targetSquare, Pieces* &enemyPiece, int pieceIndex, const std::vector<glm::vec2> &pawnPositions, std::vector<glm::vec2> &initPawnPositions, bool isWhite)
	{
		//Target square must be inside board
		if (insideBoard(targetSquare))
		{
			//If target square is a straight move
			if (targetSquare.x == pawnPositions[pieceIndex].x)
			{
				//If straight move is possible
				if (straightMovePossible(targetSquare, pieceIndex, pawnPositions, initPawnPositions))
				{
					//If friend king is attacked
					if (Pieces::kingAttacked)
					{
						//Check if the straight move prevents king from being attacked
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
			}
			//If target square is a killing move
			else
			{
				//If kill move is possible
				if (killMovePossible(targetSquare, pieceIndex, pawnPositions, enemyPiece, isWhite))
				{			
					//If friend king attacked
					if (Pieces::kingAttacked)
					{
						//Check if kill move prevents king from being attacked
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
			}
		}
		return false;
	}

	//Determines if target square is inside board
	//-------------------------------------------
	bool insideBoard(glm::vec2 &targetSquare)
	{
		//X and Y limits of board
		if (targetSquare.x > Board::boardLimitX || targetSquare.x < -Board::boardLimitX || targetSquare.y > Board::boardLimitY || targetSquare.y < -Board::boardLimitY)
		{
			return false;
		}
		return true;
	}
	
	//Determines if the pawn can move straight
	//----------------------------------------
	bool straightMovePossible(glm::vec2 &targetSquare, int pieceIndex, const std::vector<glm::vec2> &pawnPositions, std::vector<glm::vec2> &initPawnPositions)
	{
		//If a piece is blocking pawn movement then return false
		if (pieceBlockingMovement)
		{
			return false;
		}
		//Check Double square move
		else if (targetSquare.y == (pawnPositions[pieceIndex].y + squareSizeY * 2.0))
		{
			//If pawn is at initial position
			if (pawnPositions[pieceIndex] == initPawnPositions[pieceIndex])
			{
				//If a piece is on target the target square
				if (pieceIsOnStraightTargetSquare(targetSquare))
				{
					return false;
				}
				else
				{
					return true;
				}
			}
			else
			{
				return false;
			}
		}
		//If piece exists on target square
		else if (pieceIsOnStraightTargetSquare(targetSquare))
		{
			pieceBlockingMovement = true;
			return false;
		}
		return true;
	}

	//Check if a piece exists on the straight target square
	//-----------------------------------------------------
	bool pieceIsOnStraightTargetSquare(glm::vec2 & targetSquare)
	{
		for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
		{
			if (targetSquare == Pieces::pieceOnSquare[i].position)
			{
				return true;
			}
		}
		return false;
	}

	//Determines if killing move is possible
	//--------------------------------------
	bool killMovePossible(glm::vec2 &targetSquare, int pieceIndex, const std::vector<glm::vec2> &pawnPositions, Pieces* &enemyPiece, bool isWhite)
	{
		for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
		{
			//If target square has an opponent piece
			if (targetSquare == Pieces::pieceOnSquare[i].position)
			{
				if (Pieces::pieceOnSquare[i].isWhite != isWhite)
				{
					enemyPiece = Pieces::pieceOnSquare[i].piece;
					return true;
				}
			}
		}
		return false;
	}

	//Checks for the possibility of enPassant and adds relevant squares to the target squares vector
	//----------------------------------------------------------------------------------------------
	void enPassantMove(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, const std::vector<glm::vec2> &pawnPositions)
	{
		//Needs the values stored in highlightMovesStack to check for enPassant
		if (!Board::highlightMovesStack.empty())
		{
			auto previousMovedPiece = Board::highlightMovesStack.top();
			glm::vec2 rightSquare = pawnPositions[pieceIndex] + glm::vec2(squareSizeX, 0.0);
			glm::vec2 leftSquare = pawnPositions[pieceIndex] - glm::vec2(squareSizeX, 0.0);
			//Check if the squares next to the pawn has a pawn piece on it and that the pawn had moved double squares in the previous move
			for (size_t i{ 0 }; i < Pieces::pieceOnSquare.size(); i++)
			{
				//right square has a piece on it
				if (rightSquare == Pieces::pieceOnSquare[i].position)
				{
					//that piece is a pawn
					if (Pieces::pieceOnSquare[i].pieceEnum == Piece::PAWN)
					{
						//Check if this pawn was moved in the previous move
						//Negative value is checked as pieces have been switched
						if (Pieces::pieceOnSquare[i].position == -previousMovedPiece.first.second)
						{
							//See if it was a double square move
							glm::vec2 diffBetweenOldAndNewSquare = previousMovedPiece.first.second - previousMovedPiece.first.first;
							if (diffBetweenOldAndNewSquare.y == 2.0 * squareSizeY)
							{
								PieceAttribs square;
								square.position = pawnPositions[pieceIndex] + moves[0];
								square.piece = Pieces::pieceOnSquare[i].piece;
								square.index = Pieces::pieceOnSquare[i].index;
								square.rightEnPassantMove = true;
								targetSquares.push_back(square);
							}
						}
					}
				}
				else if (leftSquare == Pieces::pieceOnSquare[i].position)
				{
					if (Pieces::pieceOnSquare[i].pieceEnum == Piece::PAWN)
					{
						if (Pieces::pieceOnSquare[i].position == -previousMovedPiece.first.second)
						{
							glm::vec2 diffBetweenOldAndNewSquare = previousMovedPiece.first.second - previousMovedPiece.first.first;
							if (diffBetweenOldAndNewSquare.y == 2.0 * squareSizeY)
							{
								PieceAttribs square;
								square.position = pawnPositions[pieceIndex] + moves[1];
								square.piece = Pieces::pieceOnSquare[i].piece;
								square.index = Pieces::pieceOnSquare[i].index;
								square.leftEnPassantMove = true;
								targetSquares.push_back(square);
							}
						}
					}
				}
			}
		}
	}

	//When king is in check this function allows only those moves that prevent the king from being attacked
	//Function is similar to pinnedPieceValidSquare function
	//-----------------------------------------------------------------------------------------------------
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

	//Checks if the current piece is pinned
	//-------------------------------------
	bool piecePinned(const std::vector<glm::vec2> &pawnPositions, int pieceIndex, const std::vector<Pieces*> &pieces, bool isWhite)
	{
		//Clear temp vectors
		tempSquaresAttacked.clear();
		tempPieceOnSquare.clear();

		//Assign original values to temp variables for temporary storage
		tempKingAttackingPieces = Pieces::kingAttackingPieces;
		tempKingAttacked = Pieces::kingAttacked;
		tempPieceOnSquare.insert(tempPieceOnSquare.end(), Pieces::pieceOnSquare.begin(), Pieces::pieceOnSquare.end());
		tempSquaresAttacked.insert(tempSquaresAttacked.end(), Pieces::squaresAttacked.begin(), Pieces::squaresAttacked.end());

		//Iterate through the pieceOnSquare vector to get the index of the current piece
		for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
		{
			if (pawnPositions[pieceIndex] == Pieces::pieceOnSquare[i].position)
			{
				//"Remove" the piece from the board by giving it an arbitrary value outside the board
				Pieces::pieceOnSquare[i].position = glm::vec2(100.0, 100.0);
				break;
			}
		}

		//Clear the squaresAttacked vector and recalculate attacked squares after current piece has been removed
		Pieces::squaresAttacked.clear();
		for (size_t i = 0; i < pieces.size(); i++)
		{
			pieces[i]->calcTargetSquares(!isWhite, false);
		}

		//Restore the original kingAttackingPieces and kingAttacked values as the calcTargetSquares function above may have changed it depending on new squaresAttacked
		Pieces::kingAttackingPieces = tempKingAttackingPieces;
		Pieces::kingAttacked = tempKingAttacked;
		
		//Iterate through the new squaresAttacked vector to determine the target squares threatened by the opponent piece which is directly attacking the king
		for (size_t i = 0; i < Pieces::squaresAttacked.size(); i++)
		{
			if (Pieces::squaresAttacked[i].attackingOpponentKing)
			{
				//If the current pawn is on a square which is being attacked by an opponent piece that is attacking the king
				//Then the current pawn is pinned
				if (pawnPositions[pieceIndex] == Pieces::squaresAttacked[i].position)
				{
					return true;
				}
			}
		}
		//If piece is not pinned
		setBackOriginalValues();
		return false;
	}
	
	//This function evaluates target squares for a pinned piece and determines if it is valid
	//---------------------------------------------------------------------------------------
	bool pinnedPieceValidSquare(glm::vec2 &targetSquare, Pieces* &enemyPiece, int pieceIndex, const std::vector<glm::vec2> &pawnPositions, std::vector<glm::vec2> &initPawnPositions, bool isWhite)
	{
		//If it is a straight move
		if (targetSquare.x == pawnPositions[pieceIndex].x)
		{
			//Check if double square move is possible
			if (targetSquare.y == (pawnPositions[pieceIndex].y + squareSizeY * 2.0))
			{
				if (pawnPositions[pieceIndex] == initPawnPositions[pieceIndex])
				{
					//Iterate through squaresAttacked vector to determine if target square has the same position as that of an attacked square by the pinning piece
					//Remember that squaresAttacked.attackingOpponentKing has the value true only for squares along the same direction as opponent king
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
				}
			}
			else
			{
				//Iterate through squaresAttacked vector to determine if target square has the same position as that of an attacked square by the pinning piece
				//Remember that squaresAttacked.attackingOpponentKing has the value true only for squares along the same direction as opponent king
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
			}
		}
		else
		{
			//Iterate through pieceOnSquare vector to determine if target square has an opponent piece on it that is attacking friend king
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
		}
		return false;
	}

	//Sets back the original vector values stored in temp vectors
	//-----------------------------------------------------------
	void setBackOriginalValues()
	{
		Pieces::squaresAttacked.clear();
		Pieces::squaresAttacked.insert(Pieces::squaresAttacked.end(), tempSquaresAttacked.begin(), tempSquaresAttacked.end());
		Pieces::pieceOnSquare.clear();
		Pieces::pieceOnSquare.insert(Pieces::pieceOnSquare.end(), tempPieceOnSquare.begin(), tempPieceOnSquare.end());
	}
	
	//---------------------------
	//Setter and Getter functions
	//---------------------------

	void setPieceBlockingMovement(bool val)
	{
		pieceBlockingMovement = val;
	}

private:
	//For a pawn at initial position, double square should not be possible if a piece is blocking its movement
	//This var stores that value
	bool pieceBlockingMovement; 
	
	//Temp storage variables
	int tempKingAttackingPieces;  //temporarily stores the original kingAttackingPieces value as it changes within checkPiecePinned function
	bool tempKingAttacked;  //temporarily stores the original kingAttacked value as it changes within checkPiecePinned function
	std::vector<PieceAttribs> tempSquaresAttacked;  //temporarily stores the original squareAttacked vector values as it changes within checkPiecePinned function
	std::vector<PieceAttribs> tempPieceOnSquare;  //temporarily stores the original pieceOnSquare vector values as it changes within checkPiecePinned function
	
};

#endif
