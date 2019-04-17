/* ---------------------------------------------------------------------------------------------------------------
  Universal piece functions have been commented in pawn class. Only piece specific functions commented in this class
  ---------------------------------------------------------------------------------------------------------------- */
#ifndef BISHOP_H
#define BISHOP_H

#include <Pieces.h>
#include <Board.h>
#include <Pieces/King/King.h>
#include <Moves.h>
#include <UndoMoves.h>

class Bishop : public Pieces
{
public:

	Bishop()
	{
		whiteBishop = Graphics::loadTexture("Files/Textures/Pieces/White/white bishop.png", true);
		blackBishop = Graphics::loadTexture("Files/Textures/Pieces/Black/black bishop.png", true);
		setInitBishopPositions();
	}
	
	void bishopOutline(int index, bool isWhite) const 
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		draw(index, isWhite);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		drawColor(index, isWhite);
	}

	virtual void calcTargetSquares(bool isWhite, bool currentPlayer, const std::vector<Pieces*> &pieces = std::vector<Pieces*>{}) override
	{
		std::vector<PieceAttribs> targetSquares;
		if (currentPlayer)
		{
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whiteBishopPositions.size(); i++)
				{
					moves.calcBishopTargetSquares(targetSquares, i, bishopMoves, whiteBishopPositions, isWhite, pieces);
					availableTargetSquares.push_back(targetSquares);
					targetSquares.clear();
				}
			}
			else
			{
				for (size_t i{ 0 }; i < blackBishopPositions.size(); i++)
				{
					moves.calcBishopTargetSquares(targetSquares, i, bishopMoves, blackBishopPositions, isWhite, pieces);
					availableTargetSquares.push_back(targetSquares);
					targetSquares.clear();
				}
			}
		}
		else
		{
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whiteBishopPositions.size(); i++)
				{
					moves.calcBishopAttackedSquares(targetSquares, i, bishopMoves, whiteBishopPositions, isWhite, King::getKingPosition(!isWhite));
				}
			}
			else
			{
				for (size_t i{ 0 }; i < blackBishopPositions.size(); i++)
				{
					moves.calcBishopAttackedSquares(targetSquares, i, bishopMoves, blackBishopPositions, isWhite, King::getKingPosition(!isWhite));
				}
			}
			squaresAttacked.insert(squaresAttacked.end(), targetSquares.begin(), targetSquares.end());
		}
	}

	virtual void draw(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index value of the position that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPieces(&whiteBishop, whiteBishopPositions, index, index, true);
			}
			else
			{
				Pieces::drawPieces(&blackBishop, blackBishopPositions, index, index, true);
			}
		}
		else
		{
			Pieces::drawPieces(&whiteBishop, whiteBishopPositions, 0, whiteBishopPositions.size() - 1);
			Pieces::drawPieces(&blackBishop, blackBishopPositions, 0, blackBishopPositions.size() - 1);
		}
	} 

	virtual void drawColor(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index of the position value that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPiecesColor(&whiteBishop, whiteBishopPositions, whiteBishopColors, index, index, true);
			}
			else
			{
				Pieces::drawPiecesColor(&blackBishop, blackBishopPositions, blackBishopColors, index, index, true);
			}
		}
		else
		{
			Pieces::drawPiecesColor(&whiteBishop, whiteBishopPositions, whiteBishopColors, 0, whiteBishopPositions.size() - 1);
			Pieces::drawPiecesColor(&blackBishop, blackBishopPositions, blackBishopColors, 0, blackBishopPositions.size() - 1);
		}
	}

	virtual void drawMoves(GLFWwindow* window, int selectedPieceIndex, bool isWhite, const std::vector<Pieces*> &pieces) override
	{
		//Loop through the targetSquares vector and draw the target squares
		for (size_t i = 0; i < availableTargetSquares[selectedPieceIndex].size(); i++)
		{
			//If an opponent piece exists on target square, draw the square in red
			if (availableTargetSquares[selectedPieceIndex][i].piece)
			{
				Board::drawTargetSquare(enemySquareColor, availableTargetSquares[selectedPieceIndex][i].position, window, availableTargetSquares[selectedPieceIndex][i].piece, availableTargetSquares[selectedPieceIndex][i].index, isWhite);
			}
			else
			{
				Board::drawTargetSquare(targetSquareColor, availableTargetSquares[selectedPieceIndex][i].position, window, availableTargetSquares[selectedPieceIndex][i].piece, availableTargetSquares[selectedPieceIndex][i].index, isWhite);
			}
			//If user has selected a square for moving then break 
			if (Board::status == PieceStatus::MOVING)
			{
				return;
			}
		}
		//If mouse is clicked on an invalid square
		if (Mouse::getMouseClicked())
		{
			Board::status = PieceStatus::NOT_SELECTED;
		}
	}
		
	virtual void addSelectedPieceDataToOnePieceAffectedMovesStack(bool isWhite, int selectedPieceIndex) override
	{
		TrackMoves::addSelectedPieceDataToOnePieceAffectedMovesStack(isWhite, selectedPieceIndex, whiteBishopPositions, blackBishopPositions);
	}

	virtual void movePiece(const glm::vec2 &targetSquare, int pieceIndex, float deltaTime, bool isWhite, const std::vector<Pieces*> &pieces, GLFWwindow* window) override
	{
		draw(pieceIndex, isWhite);

		if (isWhite)
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, bishopMoves, whiteBishopPositions);
		}
		else
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, bishopMoves, blackBishopPositions);
		}
	}

	virtual void switchPiecePositions() override
	{
		Pieces::switchPiecePositions(whiteBishopPositions);
		Pieces::switchPiecePositions(blackBishopPositions);
	}

	virtual void clearAvailableTargetSquares() override
	{
		availableTargetSquares.clear();
	}

	virtual int drawPieceOutline(const glm::vec3* color, bool isWhite) const override
	{
		int val;
		if (isWhite)
		{
			val = Pieces::pieceColorIndex(whiteBishopColors, whiteBishopColors.size(), color);
		}
		else
		{
			val = Pieces::pieceColorIndex(blackBishopColors, blackBishopColors.size(), color);
		}
		if (val != -1)
		{
			bishopOutline(val, isWhite);
			return val;
		}
		return -1;
	}
		
	virtual const glm::vec2& getPiecePosition(int index, bool isWhite) const override
	{
		if (isWhite)
		{
			return whiteBishopPositions[index];
		}
		else
		{
			return blackBishopPositions[index];
		}
	}

	virtual void deletePiecePosition(int index, bool isWhite, bool isPawnPromote = false) override
	{
		if (isWhite)
		{
			blackBishopPositions[index] = glm::vec2(2.0, 2.0);
		}
		else
		{
			whiteBishopPositions[index] = glm::vec2(2.0, 2.0);
		}
	}

	//Generates a new bishop at the target square where pawn gets promoted
	//--------------------------------------------------------------------
	void generateNewBishop(const glm::vec2* targetSquare, bool isWhite)
	{
		if (isWhite)
		{
			//Add a new Bishop to the Bishop position vector 
			whiteBishopPositions.push_back(*targetSquare);
			//Also generate a new color for the new Bishop
			whiteBishopColors.push_back(Graphics::genRandomColor());
			Board::highlightMove.second.index = whiteBishopPositions.size() - 1;
			Board::highlightMove.second.piece = this;
		}
		else
		{
			//Add a new Bishop to the Bishop position vector 
			blackBishopPositions.push_back(*targetSquare);
			//Also generate a new color for the new Bishop
			blackBishopColors.push_back(Graphics::genRandomColor());
			Board::highlightMove.second.index = whiteBishopPositions.size() - 1;
			Board::highlightMove.second.piece = this;
		}
		TrackMoves::addPromotedPieceDataToThreePiecesAffectedMovesStack(isWhite, this);
		Board::highlightMove.second.isWhite = isWhite;
		Board::highlightMovesStack.push(Board::highlightMove);
	}

	virtual void calcDifferenceBetweenSquares(const glm::vec2 &targetSquare, int index, bool isWhite) override
	{
		if (isWhite)
		{
			diffBetweenSquares = targetSquare - whiteBishopPositions[index];
		}
		else
		{
			diffBetweenSquares = targetSquare - blackBishopPositions[index];
		}
	}

	virtual void restorePreviousMove(PieceAttribs attribs)
	{
		TrackMoves::restorePreviousMove(attribs, whiteBishopPositions, blackBishopPositions);
	}

	virtual void deletePromotedPiece(bool isWhite) override
	{
		TrackMoves::deleteGeneratedPiece(isWhite, whiteBishopPositions, blackBishopPositions);
	}

	//Returns the bishop texture used in the pawnPromotion UI
	//-------------------------------------------------------
	static unsigned int getBishopTexture(bool isWhite)
	{
		if (isWhite)
		{
			return whiteBishop;
		}
		else
		{
			return blackBishop;
		}
	}

	virtual ~Bishop() = default;

private:
	static unsigned int blackBishop;
	static unsigned int whiteBishop;

	std::vector<glm::vec2> whiteBishopPositions;
	std::vector<glm::vec3> whiteBishopColors;
	std::vector<glm::vec2> blackBishopPositions;
	std::vector<glm::vec3> blackBishopColors;

	glm::vec2 diffBetweenSquares;
	std::vector<std::vector<PieceAttribs>> availableTargetSquares;
	
	Moves moves;
	std::vector<glm::vec2> bishopMoves;
	
	glm::vec3 targetSquareColor{ 0.0, 0.8, 1.0 };
	glm::vec3 enemySquareColor{ 1.0, 0.0, 0.0 };
	
	void setInitBishopPositions()
	{
		setBishopMoves();
		Pieces::setupPositions(whiteBishopPositions, whiteBishopColors, 2, 2, 5);
		Pieces::setupPositions(blackBishopPositions, blackBishopColors, 2, 58, 61);
	}

	virtual void setupPieceOnSquareValues() override
	{
		PieceAttribs square;
		for (size_t i = 0; i < whiteBishopPositions.size(); i++)
		{
			square.position = whiteBishopPositions[i];
			square.index = i;
			square.isWhite = true;
			square.piece = this;
			square.pieceEnum = Piece::BISHOP;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}

		for (size_t i = 0; i < blackBishopPositions.size(); i++)
		{
			square.position = blackBishopPositions[i];
			square.index = i;
			square.isWhite = false;
			square.piece = this;
			square.pieceEnum = Piece::BISHOP;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}
	}

	void setBishopMoves()
	{
		//Bishop can move diagonally in 4 directions for a maximum of 7 squares
		for (int i = 0; i < 7; i++)
		{
			bishopMoves.push_back(glm::vec2((i + 1) * squareSizeX, (i + 1) * squareSizeY));
			bishopMoves.push_back(-glm::vec2((i + 1) * squareSizeX, (i + 1) * squareSizeY));
			bishopMoves.push_back(glm::vec2((i + 1) * squareSizeX, -((i + 1) * squareSizeY)));
			bishopMoves.push_back(-glm::vec2((i + 1) * squareSizeX, -((i + 1) * squareSizeY)));
		}
	}
};

unsigned int Bishop::whiteBishop = 0;
unsigned int Bishop::blackBishop = 0;

#endif

