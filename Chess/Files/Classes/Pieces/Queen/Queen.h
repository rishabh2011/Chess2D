/* ---------------------------------------------------------------------------------------------------------------
  Universal piece functions have been commented in pawn class. Only piece specific functions commented in this class
  ---------------------------------------------------------------------------------------------------------------- */
#ifndef QUEEN_H
#define QUEEN_H

#include <Pieces.h>
#include <Board.h>
#include <Pieces/King/King.h>
#include <Moves.h>
#include <UndoMoves.h>

class Queen : public Pieces
{
public:

	Queen()
	{
		whiteQueen = Graphics::loadTexture("Files/Textures/Pieces/White/white queen.png", true);
		blackQueen = Graphics::loadTexture("Files/Textures/Pieces/Black/black queen.png", true);
		setInitQueenPositions();
	}

	void queenOutline(int index, bool isWhite) const
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
				for (size_t i{ 0 }; i < whiteQueenPositions.size(); i++)
				{
					moves.calcQueenTargetSquares(targetSquares, i, queenMoves, whiteQueenPositions, isWhite, pieces);
					availableTargetSquares.push_back(targetSquares);
					targetSquares.clear();
				}
			}
			else
			{
				for (size_t i{ 0 }; i < blackQueenPositions.size(); i++)
				{
					moves.calcQueenTargetSquares(targetSquares, i, queenMoves, blackQueenPositions, isWhite, pieces);
					availableTargetSquares.push_back(targetSquares);
					targetSquares.clear();
				}
			}
		}
		else
		{
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whiteQueenPositions.size(); i++)
				{
					moves.calcQueenAttackedSquares(this, targetSquares, i, queenMoves, whiteQueenPositions, isWhite, King::getKingPosition(!isWhite));
				}
			}
			else
			{
				for (size_t i{ 0 }; i < blackQueenPositions.size(); i++)
				{
					moves.calcQueenAttackedSquares(this, targetSquares, i, queenMoves, blackQueenPositions, isWhite, King::getKingPosition(!isWhite));
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
				Pieces::drawPieces(&whiteQueen, whiteQueenPositions, index, index, true);
			}
			else
			{
				Pieces::drawPieces(&blackQueen, blackQueenPositions, index, index, true);
			}
		}
		else
		{
			Pieces::drawPieces(&whiteQueen, whiteQueenPositions, 0, whiteQueenPositions.size() - 1);
			Pieces::drawPieces(&blackQueen, blackQueenPositions, 0, blackQueenPositions.size() - 1);
		}
	}

	virtual void drawColor(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index of the position value that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPiecesColor(&whiteQueen, whiteQueenPositions, whiteQueenColors, index, index, true);
			}
			else
			{
				Pieces::drawPiecesColor(&blackQueen, blackQueenPositions, blackQueenColors, index, index, true);
			}
		}
		else
		{
			Pieces::drawPiecesColor(&whiteQueen, whiteQueenPositions, whiteQueenColors, 0, whiteQueenPositions.size() - 1);
			Pieces::drawPiecesColor(&blackQueen, blackQueenPositions, blackQueenColors, 0, blackQueenPositions.size() - 1);
		}
	}

	//Draws the check mating pieces in pink
	virtual void drawCheckMatingPieces(int index, bool isWhite) const override
	{
		if (isWhite)
		{
			//Get square index of the piece and color that square in pink
			int squareIndex = Board::getSquareIndex(whiteQueenPositions[index]);
			if (squareIndex != -1)
			{
				Graphics::drawBoard(Graphics::getPiecesColorShader(), &Board::getTexture(), squarePositions, squareIndex, squareIndex, 1.0, true, glm::vec3(1.0, 0.5, 0.6));
			}
			//Finally draw the piece back on top
			draw(index, isWhite);
		}
		else
		{
			//Get square index of the king and color that square in red
			int squareIndex = Board::getSquareIndex(blackQueenPositions[index]);
			if (squareIndex != -1)
			{
				Graphics::drawBoard(Graphics::getPiecesColorShader(), &Board::getTexture(), squarePositions, squareIndex, squareIndex, 1.0, true, glm::vec3(1.0, 0.5, 0.6));
			}
			//Finally draw the king back on top
			draw(index, isWhite);
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
		TrackMoves::addSelectedPieceDataToOnePieceAffectedMovesStack(isWhite, selectedPieceIndex, whiteQueenPositions, blackQueenPositions);
	}

	virtual void movePiece(const glm::vec2 &targetSquare, int pieceIndex, float deltaTime, bool isWhite, const std::vector<Pieces*> &pieces, GLFWwindow* window) override
	{
		draw(pieceIndex, isWhite);

		if (isWhite)
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, queenMoves, whiteQueenPositions);
		}
		else
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, queenMoves, blackQueenPositions);
		}
	}

	virtual void switchPiecePositions()
	{
		Pieces::switchPiecePositions(whiteQueenPositions);
		Pieces::switchPiecePositions(blackQueenPositions);
	}

	virtual void clearAvailableTargetSquares() override
	{
		availableTargetSquares.clear();
	}

	virtual std::vector<std::vector<PieceAttribs>> &getAvailableTargetSquares() override
	{
		return availableTargetSquares;
	}

	//Returns the size of availableTargetSquares
	//------------------------------------------
	virtual unsigned int getPiecesTargetSquaresSize() override
	{
		int targetSquaresSize = 0;
		for (size_t i{ 0 }; i < availableTargetSquares.size(); i++)
		{
			targetSquaresSize += availableTargetSquares[i].size();
		}
		return targetSquaresSize;
	}

	virtual int drawPieceOutline(const glm::vec3* color, bool isWhite) const override
	{
		int val;
		if (isWhite)
		{
			val = Pieces::pieceColorIndex(whiteQueenColors, whiteQueenColors.size(), color);
		}
		else
		{
			val = Pieces::pieceColorIndex(blackQueenColors, blackQueenColors.size(), color);
		}
		if (val != -1)
		{
			queenOutline(val, isWhite);
			return val;
		}
		return -1;
	}
		
	virtual const glm::vec2& getPiecePosition(int index, bool isWhite) const override
	{
		if (isWhite)
		{
			return whiteQueenPositions[index];
		}
		else
		{
			return blackQueenPositions[index];
		}
	}

	virtual const std::vector<glm::vec2>& getWhitePiecePositions() const override
	{
		return whiteQueenPositions;
	}

	virtual const std::vector<glm::vec2>& getBlackPiecePositions() const override
	{
		return blackQueenPositions;
	}

	virtual void setWhitePiecePositions(std::vector<glm::vec2> whitePositions) override
	{
		for (size_t i{ 0 }; i < whiteQueenPositions.size(); i++)
		{
			whiteQueenPositions[i] = whitePositions[i];
		}
	}

	virtual void setBlackPiecePositions(std::vector<glm::vec2> blackPositions) override
	{
		for (size_t i{ 0 }; i < blackQueenPositions.size(); i++)
		{
			blackQueenPositions[i] = blackPositions[i];
		}
	}

	virtual void deletePiecePosition(int index, bool isWhite, bool isPawnPromote = false) override
	{
		if (isWhite)
		{
			blackQueenPositions[index] = glm::vec2(100.0, 100.0);
		}
		else
		{
			whiteQueenPositions[index] = glm::vec2(100.0, 100.0);
		}
	}

	//Generates a new queen at the target square where pawn gets promoted
	//--------------------------------------------------------------------
	void generateNewQueen(const glm::vec2* targetSquare, bool isWhite)
	{
		if (isWhite)
		{
			//Add a new Queen to the Queen position vector 
			whiteQueenPositions.push_back(*targetSquare);
			//Also generate a new color for the new Queen
			whiteQueenColors.push_back(Graphics::genRandomColor());
			Board::highlightMove.second.index = whiteQueenPositions.size() - 1;
			Board::highlightMove.second.piece = this;
		}
		else
		{
			//Add a new Queen to the Queen position vector 
			blackQueenPositions.push_back(*targetSquare);
			//Also generate a new color for the new Queen
			blackQueenColors.push_back(Graphics::genRandomColor());
			Board::highlightMove.second.index = whiteQueenPositions.size() - 1;
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
			diffBetweenSquares = targetSquare - whiteQueenPositions[index];
		}
		else
		{
			diffBetweenSquares = targetSquare - blackQueenPositions[index];
		}
	}

	virtual void restorePreviousMove(PieceAttribs attribs)
	{
		TrackMoves::restorePreviousMove(attribs, whiteQueenPositions, blackQueenPositions);
	}

	virtual void deletePromotedPiece(bool isWhite) override
	{
		TrackMoves::deleteGeneratedPiece(isWhite, whiteQueenPositions, blackQueenPositions);
	}

	//Returns the queen texture used in the pawnPromotion UI
	//------------------------------------------------------
	static unsigned int getQueenTexture(bool isWhite)
	{
		if (isWhite)
		{
			return whiteQueen;
		}
		else
		{
			return blackQueen;
		}
	}

	virtual ~Queen() = default;

private:
	static unsigned int blackQueen;
	static unsigned int whiteQueen;

	std::vector<glm::vec2> whiteQueenPositions;
	std::vector<glm::vec3> whiteQueenColors;
	std::vector<glm::vec2> blackQueenPositions;
	std::vector<glm::vec3> blackQueenColors;

	glm::vec2 diffBetweenSquares;
	std::vector<std::vector<PieceAttribs>> availableTargetSquares;
	
	Moves moves;
	std::vector<glm::vec2> queenMoves;
	
	glm::vec3 targetSquareColor{ 0.0, 0.8, 1.0 };
	glm::vec3 enemySquareColor{ 1.0, 0.0, 0.0 };
	
	void setInitQueenPositions()
	{
		setQueenMoves();
		Pieces::setupPositions(whiteQueenPositions, whiteQueenColors, 1, 3);
		Pieces::setupPositions(blackQueenPositions, blackQueenColors, 1, 59);
	}

	virtual void setupPieceOnSquareValues() override
	{
		PieceAttribs square;
		for (size_t i = 0; i < whiteQueenPositions.size(); i++)
		{
			square.position = whiteQueenPositions[i];
			square.index = i;
			square.isWhite = true;
			square.piece = this;
			square.pieceEnum = Piece::QUEEN;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}

		for (size_t i = 0; i < blackQueenPositions.size(); i++)
		{
			square.position = blackQueenPositions[i];
			square.index = i;
			square.isWhite = false;
			square.piece = this;
			square.pieceEnum = Piece::QUEEN;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}
	}

	void setQueenMoves()
	{
		//Queen can move in all 8 directions
		//a maximum of 7 squares per direction
		for (int i = 0; i < 7; i++) 
		{
			queenMoves.push_back(glm::vec2((i + 1) * squareSizeX, 0.0));
			queenMoves.push_back(-glm::vec2((i + 1) * squareSizeX, 0.0));
			queenMoves.push_back(glm::vec2(0.0, (i + 1) * squareSizeY));
			queenMoves.push_back(-glm::vec2(0.0, (i + 1) * squareSizeY));
			queenMoves.push_back(glm::vec2((i + 1) * squareSizeX, (i + 1) * squareSizeY));
			queenMoves.push_back(-glm::vec2((i + 1) * squareSizeX, (i + 1) * squareSizeY));
			queenMoves.push_back(glm::vec2(-((i + 1) * squareSizeX), (i + 1) * squareSizeY));
			queenMoves.push_back(-glm::vec2(-((i + 1) * squareSizeX), (i + 1) * squareSizeY));
		}
	}
};

unsigned int Queen::whiteQueen = 0;
unsigned int Queen::blackQueen = 0;

#endif
