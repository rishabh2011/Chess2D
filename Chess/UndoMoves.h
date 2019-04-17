/* ----------------------------------------------------------------------------------------
   Class tracks all the moves being played in the game and restores previously played moves
   ---------------------------------------------------------------------------------------- */
#ifndef UNDOMOVES_H
#define UNDOMOVES_H

#include <Pieces.h>
#include <stack>
#include <vector>

class TrackMoves
{
public:

	//Stack variables
	static std::stack<PieceAttribs> onePieceAffectedMoves;   //Stores all moves that affect a single piece
	static std::stack<std::pair<PieceAttribs, unsigned int>> twoPiecesAffectedMoves;  //Stores 2nd piece data for moves that affect two pieces (killing moves, castling)
	static std::stack<std::pair<PromotedPieceAttribs, unsigned int>> threePiecesAffectedMoves;  //Stores 3rd piece data for moves that affect 3 pieces (pawn promotion)
	//Tracks the current move number; Incremented for every move played, decremented when undoing moves
	static unsigned int moveNumber;  

	//Pushes the selected piece's data to othe one piece affected move stack
	//----------------------------------------------------------------------
	static void addSelectedPieceDataToOnePieceAffectedMovesStack(bool isWhite, int selectedPieceIndex, const std::vector<glm::vec2> &whitePositions, const std::vector<glm::vec2> &blackPositions)
	{
		if (isWhite)
		{
			//Iterate through pieceOnSquare vector to get the piece data for the currently selected piece
			//and push the data to the stack
			for (size_t i{ 0 }; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (whitePositions[selectedPieceIndex] == Pieces::pieceOnSquare[i].position)
				{
					onePieceAffectedMoves.push(Pieces::pieceOnSquare[i]);
					break;
				}
			}
		}
		else
		{
			for (size_t i{ 0 }; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (blackPositions[selectedPieceIndex] == Pieces::pieceOnSquare[i].position)
				{
					onePieceAffectedMoves.push(Pieces::pieceOnSquare[i]);
					break;
				}
			}
		}
	}

	//Pushes the killed piece's data to the the two pieces affected moves stack
	//-------------------------------------------------------------------------
	static void addKilledPieceDataToTwoPiecesAffectedMovesStack(const glm::vec2 &targetSquare) 
	{
		//Iterate through the pieceOnSquare vector to get the data for piece that is on the target square
		//and add that data to the twoPiecesAffectedMoves stack
		//Along with the current moveNumber so that the game knows when to pop this data from the stack when undoing
		for (size_t i{ 0 }; i < Pieces::pieceOnSquare.size(); i++)
		{
			if (targetSquare == Pieces::pieceOnSquare[i].position)
			{
				std::pair<PieceAttribs, unsigned int> killingMove;
				killingMove.first = Pieces::pieceOnSquare[i];
				killingMove.second = moveNumber;
				twoPiecesAffectedMoves.push(killingMove);
				break;
			}
		}
	}

	//Pushes the appropriate rook's data to the the two pieces affected moves stack during castling
	//---------------------------------------------------------------------------------------------
	static void addCastledRookDataToTwoPiecesAffectedMovesStack(bool isWhite, bool castlingKingSide, bool castlingQueenSide, const std::vector<glm::vec2> &whiteRookPositions, const std::vector<glm::vec2> &blackRookPositions)
	{
		std::pair<PieceAttribs, unsigned int> castlingMove;
		if (castlingKingSide)
		{
			for (size_t i{ 0 }; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (isWhite)
				{
					if (whiteRookPositions[1] == Pieces::pieceOnSquare[i].position)
					{
						castlingMove.first = Pieces::pieceOnSquare[i];
						castlingMove.second = moveNumber;
						twoPiecesAffectedMoves.push(castlingMove);
						break;
					}
				}
				else
				{
					if (blackRookPositions[1] == Pieces::pieceOnSquare[i].position)
					{
						castlingMove.first = Pieces::pieceOnSquare[i];
						castlingMove.second = moveNumber;
						twoPiecesAffectedMoves.push(castlingMove);
						break;
					}
				}
			}
		}
		else if (castlingQueenSide)
		{
			for (size_t i{ 0 }; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (isWhite)
				{
					if (whiteRookPositions[0] == Pieces::pieceOnSquare[i].position)
					{
						castlingMove.first = Pieces::pieceOnSquare[i];
						castlingMove.second = moveNumber;
						twoPiecesAffectedMoves.push(castlingMove);
						break;
					}
				}
				else
				{
					if (blackRookPositions[0] == Pieces::pieceOnSquare[i].position)
					{
						castlingMove.first = Pieces::pieceOnSquare[i];
						castlingMove.second = moveNumber;
						twoPiecesAffectedMoves.push(castlingMove);
						break;
					}
				}
			}
		}
	}

	//Pushes the newly generated piece's data during pawn promotion to the the three pieces affected moves stack 
	//----------------------------------------------------------------------------------------------------------
	static void addPromotedPieceDataToThreePiecesAffectedMovesStack(bool isWhite, Pieces *piece)
	{
		std::pair<PromotedPieceAttribs, unsigned int> generatedPiece;
		generatedPiece.first.promotedPiece = piece;
		generatedPiece.first.isWhite = isWhite;
		generatedPiece.second = moveNumber;
		threePiecesAffectedMoves.push(generatedPiece);
	}

	//Resets the piece positions to their previous values
	//---------------------------------------------------
	static void restorePreviousMove(PieceAttribs attribs, std::vector<glm::vec2> &whitePositions, std::vector<glm::vec2> &blackPositions)
	{
		if (attribs.isWhite)
		{
			whitePositions[attribs.index] = attribs.position;
		}
		else
		{
			blackPositions[attribs.index] = attribs.position;
		}
	}

	//Deletes the generated piece during a pawn promotion
	//---------------------------------------------------
	static void deleteGeneratedPiece(bool isWhite, std::vector<glm::vec2> &whitePositions, std::vector<glm::vec2> &blackPositions)
	{
		if (isWhite)
		{
			whitePositions.pop_back();
		}
		else
		{
			blackPositions.pop_back();
		}
	}
};

unsigned int TrackMoves::moveNumber{ 0 };
std::stack<PieceAttribs> TrackMoves::onePieceAffectedMoves{};
std::stack<std::pair<PieceAttribs, unsigned int>> TrackMoves::twoPiecesAffectedMoves{};
std::stack<std::pair<PromotedPieceAttribs, unsigned int>> TrackMoves::threePiecesAffectedMoves{};
#endif