/* --------------------------------------------------------
   Class performs possible moves calculation for all pieces
   -------------------------------------------------------- */
#ifndef MOVES_H
#define MOVES_H

#include "Files/Classes/Pieces.h"
#include "PawnRules.h"
#include "RookRules.h"
#include "KnightRules.h"
#include "BishopRules.h"
#include "QueenRules.h"
#include "KingRules.h"

class Moves
{
public:

	//Moves the selected piece over time to the selected target square
	//----------------------------------------------------------------
	void movePiece(const glm::vec2& targetSquare, const glm::vec2 &diffBetweenSquares, int pieceIndex, float deltaTime, const std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions)
	{
		positions[pieceIndex] += diffBetweenSquares * deltaTime * moveSpeed;
		//If piece position exceeds targetSquare set piece position to targetSquare
		if (diffBetweenSquares.x >= 0)
		{
			if (positions[pieceIndex].x >= targetSquare.x)
			{
				positions[pieceIndex].x = targetSquare.x;
			}
		}
		if (diffBetweenSquares.x < 0)
		{
			if (positions[pieceIndex].x <= targetSquare.x)
			{
				positions[pieceIndex].x = targetSquare.x;
			}
		}
		if (diffBetweenSquares.y >= 0)
		{
			if (positions[pieceIndex].y >= targetSquare.y)
			{
				positions[pieceIndex].y = targetSquare.y;
			}
		}
		if (diffBetweenSquares.y < 0)
		{
			if (positions[pieceIndex].y <= targetSquare.y)
			{
				positions[pieceIndex].y = targetSquare.y;
			}
		}
	}

	//------------------------------------------------------------------------------------
	//Calculate current player available moves
	//These functions calculate the squares available to move for every piece on the board 
	//------------------------------------------------------------------------------------
	
	void calcPawnTargetSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, const std::vector<glm::vec2> &positions, std::vector<glm::vec2> initPawnPositions, bool isWhite, const std::vector<Pieces*> &pieces)
	{
		//If more than one opponent pieces attacking king, this piece cannot move
		if (Pieces::kingAttackingPieces > 1)
		{
			return;
		}
		//Pawns are switched when black
		//There is index mismatching as only 1 initPawnPositions vector is stored
		//Hence reverse x values so that indexes match for black pieces
		if(!isWhite)
		{
			for (size_t i = 0; i < initPawnPositions.size(); i++)
			{
				initPawnPositions[i].x = -initPawnPositions[i].x;
			}
		}
		PieceAttribs squares;
		//If pawn is pinned
		if (pawnRules.piecePinned(positions, pieceIndex, pieces, isWhite))
		{
			//If pawn is pinned and king is attacked then current pawn cannot move even if it has an available square to move to
			if (Pieces::kingAttacked)
			{
				pawnRules.setBackOriginalValues();
				return;
			}
			for (size_t i = 0; i < moves.size(); i++)
			{
				enemyPiece = nullptr;
				glm::vec2 targetSquare = positions[pieceIndex] + moves[i]; //Calculate the next target square based on moves vector
				//Check if target square is a square that the pinned piece can move to without jeopardizing the king's safety
				//In pawn's case it can only be a killing move on the pinning piece
				if (pawnRules.pinnedPieceValidSquare(targetSquare, enemyPiece, pieceIndex, positions, initPawnPositions, isWhite))
				{
					for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
					{
						if (targetSquare == Pieces::pieceOnSquare[i].position)
						{
							squares.index = Pieces::pieceOnSquare[i].index;
						}
					}
					squares.position = targetSquare;
					squares.piece = enemyPiece;  //will remain a nullptr for squares that do not have any opponent pieces on them
					targetSquares.push_back(squares);
				}
			}
			pawnRules.setBackOriginalValues();
		}
		//If pawn is not pinned
		else
		{
			for (size_t i = 0; i < moves.size(); i++)
			{
				enemyPiece = nullptr;
				glm::vec2 targetSquare = positions[pieceIndex] + moves[i];  //Calculate the next target square based on moves vector
				//Check if target square is available for the pawn to move to
				if (pawnRules.isValidSquare(targetSquare, enemyPiece, pieceIndex, positions, initPawnPositions, isWhite))
				{
					for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
					{
						if (targetSquare == Pieces::pieceOnSquare[i].position)
						{
							squares.index = Pieces::pieceOnSquare[i].index;
						}
					}
					squares.position = targetSquare;
					squares.piece = enemyPiece;  //will remain a nullptr for squares that do not have any opponent pieces on them
					targetSquares.push_back(squares);
				}
			}
		}
		pawnRules.setPieceBlockingMovement(false);
	}

	void calcRookTargetSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite, const std::vector<Pieces*> &pieces)
	{
		if (Pieces::kingAttackingPieces > 1)
		{
			return;
		}
		PieceAttribs squares;
		if (rookRules.piecePinned(positions, pieceIndex, pieces, isWhite))
		{
			if (Pieces::kingAttacked)
			{
				rookRules.setBackOriginalValues();
				return;
			}
			for (size_t i = 0; i < moves.size(); i++)
			{
				enemyPiece = nullptr;
				glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
				if (rookRules.pinnedPieceValidSquare(targetSquare, isWhite, enemyPiece))
				{
					for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
					{
						if (targetSquare == Pieces::pieceOnSquare[i].position)
						{
							squares.index = Pieces::pieceOnSquare[i].index;
						}
					}
					squares.position = targetSquare;
					squares.piece = enemyPiece;
					targetSquares.push_back(squares);
				}
			}
			rookRules.setBackOriginalValues();
		}
		else
		{
			for (size_t i = 0; i < moves.size(); i++)
			{
				enemyPiece = nullptr;
				glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
				if (rookRules.isValidSquare(targetSquare, enemyPiece, pieceIndex, positions, isWhite))
				{
					for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
					{
						if (targetSquare == Pieces::pieceOnSquare[i].position)
						{
							squares.index = Pieces::pieceOnSquare[i].index;
						}
					}
					squares.position = targetSquare;
					squares.piece = enemyPiece;
					targetSquares.push_back(squares);
				}
			}
		}
		rookRules.setDefaultPieceBlockValues(false);
	}

	void calcKnightTargetSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite, const std::vector<Pieces*> &pieces)
	{
		if (Pieces::kingAttackingPieces > 1)
		{
			return;
		}
		PieceAttribs squares;
		if (knightRules.piecePinned(positions, pieceIndex, pieces, isWhite))
		{
			if (Pieces::kingAttacked)
			{
				knightRules.setBackOriginalValues();
				return;
			}
			for (size_t i = 0; i < moves.size(); i++)
			{
				enemyPiece = nullptr;
				glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
				if (knightRules.pinnedPieceValidSquare(targetSquare, isWhite, enemyPiece))
				{
					for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
					{
						if (targetSquare == Pieces::pieceOnSquare[i].position)
						{
							squares.index = Pieces::pieceOnSquare[i].index;
						}
					}
					squares.position = targetSquare;
					squares.piece = enemyPiece;
					targetSquares.push_back(squares);
				}
			}
			knightRules.setBackOriginalValues();
		}
		else
		{
			for (size_t i = 0; i < moves.size(); i++)
			{
				enemyPiece = nullptr;
				glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
				if (knightRules.isValidSquare(targetSquare, enemyPiece, isWhite))
				{
					for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
					{
						if (targetSquare == Pieces::pieceOnSquare[i].position)
						{
							squares.index = Pieces::pieceOnSquare[i].index;
						}
					}
					squares.position = targetSquare;
					squares.piece = enemyPiece;
					targetSquares.push_back(squares);
				}
			}
		}
	}

	void calcBishopTargetSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite, const std::vector<Pieces*> &pieces)
	{
		if (Pieces::kingAttackingPieces > 1)
		{
			return;
		}
		PieceAttribs squares;
		if (bishopRules.piecePinned(positions, pieceIndex, pieces, isWhite))
		{
			if (Pieces::kingAttacked)
			{
				bishopRules.setBackOriginalValues();
				return;
			}
			for (size_t i = 0; i < moves.size(); i++)
			{
				enemyPiece = nullptr;
				glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
				if (bishopRules.pinnedPieceValidSquare(targetSquare, isWhite, enemyPiece))
				{
					for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
					{
						if (targetSquare == Pieces::pieceOnSquare[i].position)
						{
							squares.index = Pieces::pieceOnSquare[i].index;
						}
					}
					squares.position = targetSquare;
					squares.piece = enemyPiece;
					targetSquares.push_back(squares);
				}
			}
			bishopRules.setBackOriginalValues();
		}
		else
		{
			for (size_t i = 0; i < moves.size(); i++)
			{
				enemyPiece = nullptr;
				glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
				if (bishopRules.isValidSquare(targetSquare, enemyPiece, pieceIndex, positions, isWhite))
				{
					for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
					{
						if (targetSquare == Pieces::pieceOnSquare[i].position)
						{
							squares.index = Pieces::pieceOnSquare[i].index;
						}
					}
					squares.position = targetSquare;
					squares.piece = enemyPiece;
					targetSquares.push_back(squares);
				}
			}
		}
		bishopRules.setDefaultBlockValues(false);
	}

	void calcQueenTargetSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite, const std::vector<Pieces*> &pieces)
	{
		if (Pieces::kingAttackingPieces > 1)
		{
			return;
		}
		PieceAttribs squares;
		if (queenRules.piecePinned(positions, pieceIndex, pieces, isWhite))
		{
			if (Pieces::kingAttacked)
			{
				queenRules.setBackOriginalValues();
				return;
			}
			for (size_t i = 0; i < moves.size(); i++)
			{
				enemyPiece = nullptr;
				glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
				if (queenRules.pinnedPieceValidSquare(targetSquare, isWhite, enemyPiece))
				{
					for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
					{
						if (targetSquare == Pieces::pieceOnSquare[i].position)
						{
							squares.index = Pieces::pieceOnSquare[i].index;
						}
					}
					squares.position = targetSquare;
					squares.piece = enemyPiece;
					targetSquares.push_back(squares);
				}
			}
			queenRules.setBackOriginalValues();
		}
		else
		{
			for (size_t i = 0; i < moves.size(); i++)
			{
				enemyPiece = nullptr;
				glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
				if (queenRules.isValidSquare(targetSquare, enemyPiece, pieceIndex, positions, isWhite))
				{
					for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
					{
						if (targetSquare == Pieces::pieceOnSquare[i].position)
						{
							squares.index = Pieces::pieceOnSquare[i].index;
						}
					}
					squares.position = targetSquare;
					squares.piece = enemyPiece;
					targetSquares.push_back(squares);
				}
			}
		}
		queenRules.setDefaultBlockValues(false);
	}

	void calcKingTargetSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite)
	{
		//King cannot be pinned and it is always allowed to move unless no moves are available
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (kingRules.isValidSquare(targetSquare, enemyPiece, isWhite))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
	}

	//---------------------------------------------------------------------
	//Calculate opponent pieces attacking squares
	//These functions calculate the squares attacked by the opponent pieces
	//---------------------------------------------------------------------

	void calcPawnAttackedSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, const std::vector<glm::vec2> &positions, bool isWhite, const glm::vec2 &kingPosition)
	{
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (pawnRules.attackingSquare(targetSquare, pieceIndex, positions, isWhite, kingPosition))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
		pawnRules.setPieceBlockingMovement(false);
	}
	
	void calcRookAttackedSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite, const glm::vec2 &kingPosition)
	{
		bool kingAttacked = false;
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (rookRules.attackingSquare(targetSquare, pieceIndex, positions, isWhite, kingPosition, kingAttacked))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
		if (kingAttacked)
		{
			rookRules.setSquaresAttacked(targetSquares, positions, pieceIndex, kingPosition);
		}
		rookRules.setDefaultPieceBlockValues(false);
	}
	
	void calcKnightAttackedSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite, const glm::vec2 &kingPosition)
	{
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (knightRules.attackingSquare(targetSquare, pieceIndex, positions, isWhite, kingPosition))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
	}
	
	void calcBishopAttackedSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite, const glm::vec2 &kingPosition)
	{
		bool kingAttacked = false;
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (bishopRules.attackingSquare(targetSquare, pieceIndex, positions, isWhite, kingPosition, kingAttacked))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
		if (kingAttacked)
		{
			bishopRules.setSquaresAttacked(targetSquares, positions, pieceIndex, kingPosition);
		}
		bishopRules.setDefaultBlockValues(false);
	}
	
	void calcQueenAttackedSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite, const glm::vec2 &kingPosition)
	{
		bool kingAttacked = false;
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (queenRules.attackingSquare(targetSquare, pieceIndex, positions, isWhite, kingPosition, kingAttacked))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
		if (kingAttacked)
		{
			queenRules.setSquaresAttacked(targetSquares, positions, pieceIndex, kingPosition);
		}
		queenRules.setDefaultBlockValues(false);
	}
	
	void calcKingAttackedSquares(std::vector<PieceAttribs> &targetSquares, int pieceIndex, std::vector<glm::vec2> &moves, std::vector<glm::vec2> &positions, bool isWhite)
	{
		PieceAttribs squares;
		for (size_t i = 0; i < moves.size(); i++)
		{
			enemyPiece = nullptr;
			glm::vec2 targetSquare = positions[pieceIndex] + moves[i];
			if (kingRules.attackingSquare(targetSquare, isWhite))
			{
				for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
				{
					if (targetSquare == Pieces::pieceOnSquare[i].position)
					{
						squares.index = Pieces::pieceOnSquare[i].index;
					}
				}
				squares.position = targetSquare;
				squares.piece = enemyPiece;
				targetSquares.push_back(squares);
			}
		}
	}
	
private:

	Pieces* enemyPiece{ nullptr };
	PawnRules pawnRules;
	RookRules rookRules;
	KnightRules knightRules;
	BishopRules bishopRules;
	QueenRules queenRules;
	KingRules kingRules;
	float moveSpeed = 2.0f;
};

#endif
