#ifndef AI_H
#define AI_H

#include <random>
#include <ctime>
#include <Pieces.h>

std::uniform_int_distribution<unsigned int> randomNumber{ 0, 5 };

class AI
{
public:

	static PieceAttribs move;
	static int pieceIndex;
	static int selectedPiece;
	static bool movePlayed;
	
	static void playMoveAtRandom(std::vector<Pieces*> &pieces, bool isWhite)
	{
		while (!movePlayed)
		{
			bool moveAvailable{ false };
			selectedPiece = randomNumber(engine);
			for (size_t i{ 0 }; i < pieces[selectedPiece]->getAvailableTargetSquares().size(); i++)
			{
				if (pieces[selectedPiece]->getAvailableTargetSquares()[i].size() != 0)
				{
					moveAvailable = true;
					break;
				}
			}
			if (moveAvailable)
			{
				std::uniform_int_distribution<unsigned int> randomPieceIndex{ 0, pieces[selectedPiece]->getAvailableTargetSquares().size() - 1 };
				while (true)
				{
					pieceIndex = randomPieceIndex(engine);
					unsigned int numTargetSquares = pieces[selectedPiece]->getAvailableTargetSquares()[pieceIndex].size();
					if (numTargetSquares != 0)
					{
						std::uniform_int_distribution<unsigned int> randomTargetSquareIndex{ 0, numTargetSquares - 1 };
						int targetSquareIndex = randomTargetSquareIndex(engine);
						move.castlingKingSideSquare = pieces[selectedPiece]->getAvailableTargetSquares()[pieceIndex][targetSquareIndex].castlingKingSideSquare;
						move.castlingQueenSideSquare = pieces[selectedPiece]->getAvailableTargetSquares()[pieceIndex][targetSquareIndex].castlingQueenSideSquare;
						move.rightEnPassantMove = pieces[selectedPiece]->getAvailableTargetSquares()[pieceIndex][targetSquareIndex].rightEnPassantMove;
						move.leftEnPassantMove = pieces[selectedPiece]->getAvailableTargetSquares()[pieceIndex][targetSquareIndex].leftEnPassantMove;
						move.position = pieces[selectedPiece]->getAvailableTargetSquares()[pieceIndex][targetSquareIndex].position;
						move.piece = pieces[selectedPiece]->getAvailableTargetSquares()[pieceIndex][targetSquareIndex].piece;
						movePlayed = true;
						break;
					}
				}
			}
		}

	}
};

PieceAttribs AI::move{};
int AI::pieceIndex{ -1 };
int AI::selectedPiece{ -1 };
bool AI::movePlayed{ false };


#endif