/* ---------------------------------------------------------------------------------------------------------------
  Universal piece functions have been commented in pawn class. Only piece specific functions commented in this class
  ---------------------------------------------------------------------------------------------------------------- */
#ifndef KING_H
#define KING_H

#include <Pieces.h>
#include <Board.h>
#include <Moves.h>
#include <UndoMoves.h>

class King : public Pieces
{
public:
		
	King()
	{
		whiteKing = Graphics::loadTexture("Files/Textures/Pieces/White/white king.png", true);
		blackKing = Graphics::loadTexture("Files/Textures/Pieces/Black/black king.png", true);
		setInitKingPositions();
	}

	void kingOutline(int index, bool isWhite) const
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
				for (size_t i{ 0 }; i < whiteKingPositions.size(); i++)
				{
					moves.calcKingTargetSquares(targetSquares, i, kingMoves, whiteKingPositions, isWhite);
					availableTargetSquares.push_back(targetSquares);
					targetSquares.clear();
				}
			}
			else
			{
				for (size_t i{ 0 }; i < blackKingPositions.size(); i++)
				{
					moves.calcKingTargetSquares(targetSquares, i, kingMoves, blackKingPositions, isWhite);
					availableTargetSquares.push_back(targetSquares);
					targetSquares.clear();
				}
			}
		}
		else
		{
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whiteKingPositions.size(); i++)
				{
					moves.calcKingAttackedSquares(this, targetSquares, i, kingMoves, whiteKingPositions, isWhite);
				}
			}
			else
			{
				for (size_t i{ 0 }; i < blackKingPositions.size(); i++)
				{
					moves.calcKingAttackedSquares(this, targetSquares, i, kingMoves, blackKingPositions, isWhite);
				}
			}
			squaresAttacked.insert(squaresAttacked.end(), targetSquares.begin(), targetSquares.end());
		}
	}

	//Draws king square in red to indicate check
	//------------------------------------------
	virtual void drawKingInCheck(bool isWhite) override
	{
		if (kingAttacked)
		{
			if (isWhite)
			{
				//Get square index of the king and color that square in red
				int squareIndex = Board::getSquareIndex(whiteKingPositions[0]);
				if (squareIndex != -1)
				{
					Graphics::drawBoard(Graphics::getPiecesColorShader(), &Board::getTexture(), squarePositions, squareIndex, squareIndex, 1.0, true, glm::vec3(1.0, 0.0, 0.0));
				}
				//Finally draw the king back on top
				draw(0, isWhite);
			}
			else
			{
				//Get square index of the king and color that square in red
				int squareIndex = Board::getSquareIndex(blackKingPositions[0]);
				if (squareIndex != -1)
				{
					Graphics::drawBoard(Graphics::getPiecesColorShader(), &Board::getTexture(), squarePositions, squareIndex, squareIndex, 1.0, true, glm::vec3(1.0, 0.0, 0.0));
				}
				//Finally draw the king back on top
				draw(0, isWhite);
			}
		}
	}

	virtual void draw(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index value of the position that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPieces(&whiteKing, whiteKingPositions, index, index, true);
			}
			else
			{
				Pieces::drawPieces(&blackKing, blackKingPositions, index, index, true);
			}
		}
		else
		{
			Pieces::drawPieces(&whiteKing, whiteKingPositions, 0, whiteKingPositions.size() - 1);
			Pieces::drawPieces(&blackKing, blackKingPositions, 0, blackKingPositions.size() - 1);
		}
	}

	virtual void drawColor(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index of the position value that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPiecesColor(&whiteKing, whiteKingPositions, whiteKingColors, index, index, true);
			}
			else
			{
				Pieces::drawPiecesColor(&blackKing, blackKingPositions, blackKingColors, index, index, true);
			}
		}
		else
		{
			Pieces::drawPiecesColor(&whiteKing, whiteKingPositions, whiteKingColors, 0, whiteKingPositions.size() - 1);
			Pieces::drawPiecesColor(&blackKing, blackKingPositions, blackKingColors, 0, blackKingPositions.size() - 1);
		}
	}

	//Draws the check mating pieces in pink
	virtual void drawCheckMatingPieces(int index, bool isWhite) const override
	{
		if (isWhite)
		{
			//Get square index of the piece and color that square in pink
			int squareIndex = Board::getSquareIndex(whiteKingPositions[index]);
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
			int squareIndex = Board::getSquareIndex(blackKingPositions[index]);
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
			//If target square is a castling square, draw the square in orange
			if (availableTargetSquares[selectedPieceIndex][i].castlingKingSideSquare || availableTargetSquares[selectedPieceIndex][i].castlingQueenSideSquare)
			{
				Board::drawTargetSquare(castlingSquareColor, availableTargetSquares[selectedPieceIndex][i].position, window, availableTargetSquares[selectedPieceIndex][i].piece,
				availableTargetSquares[selectedPieceIndex][i].index, isWhite, pieces, availableTargetSquares[selectedPieceIndex][i].castlingKingSideSquare, availableTargetSquares[selectedPieceIndex][i].castlingQueenSideSquare);
			}
			//If an opponent piece exists on target square, draw the square in red
			else if (availableTargetSquares[selectedPieceIndex][i].piece)
			{
				Board::drawTargetSquare(enemySquareColor, availableTargetSquares[selectedPieceIndex][i].position, window, availableTargetSquares[selectedPieceIndex][i].piece, availableTargetSquares[selectedPieceIndex][i].index, isWhite);
			}
			else
			{
				Board::drawTargetSquare(targetSquareColor, availableTargetSquares[selectedPieceIndex][i].position, window, availableTargetSquares[selectedPieceIndex][i].piece,	availableTargetSquares[selectedPieceIndex][i].index, isWhite);
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
		TrackMoves::addSelectedPieceDataToOnePieceAffectedMovesStack(isWhite, selectedPieceIndex, whiteKingPositions, blackKingPositions);
	}

	virtual void movePiece(const glm::vec2 &targetSquare, int pieceIndex, float deltaTime, bool isWhite, const std::vector<Pieces*> &pieces, GLFWwindow* window) override
	{
		draw(pieceIndex, isWhite);

		if (isWhite)
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, kingMoves, whiteKingPositions);
		}
		else
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, kingMoves, blackKingPositions);
		}
		//Increment this king's move once
		if (!kingMoveIncremented)
		{
			incrementKingMove(isWhite);
			kingMoveIncremented = true;
		}
	}

	virtual void switchPiecePositions() override
	{
		Pieces::switchPiecePositions(whiteKingPositions);
		Pieces::switchPiecePositions(blackKingPositions);
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
			val = Pieces::pieceColorIndex(whiteKingColors, whiteKingColors.size(), color);
		}
		else
		{
			val = Pieces::pieceColorIndex(blackKingColors, blackKingColors.size(), color);
		}
		if (val != -1)
		{
			kingOutline(val, isWhite);
			return val;
		}
		return -1;
	}
	
	virtual const glm::vec2& getPiecePosition(int index, bool isWhite) const override
	{
		if (isWhite)
		{
			return whiteKingPositions[index];
		}
		else
		{
			return blackKingPositions[index];
		}
	}
	virtual const std::vector<glm::vec2>& getWhitePiecePositions() const override
	{
		return whiteKingPositions;
	}

	virtual const std::vector<glm::vec2>& getBlackPiecePositions() const override
	{
		return blackKingPositions;
	}

	virtual void setWhitePiecePositions(std::vector<glm::vec2> whitePositions) override
	{
		for (size_t i{ 0 }; i < whiteKingPositions.size(); i++)
		{
			whiteKingPositions[i] = whitePositions[i];
		}
	}

	virtual void setBlackPiecePositions(std::vector<glm::vec2> blackPositions) override
	{
		for (size_t i{ 0 }; i < blackKingPositions.size(); i++)
		{
			blackKingPositions[i] = blackPositions[i];
		}
	}

	virtual void deletePiecePosition(int index, bool isWhite, bool isPawnPromote = false) override{}

	virtual void calcDifferenceBetweenSquares(const glm::vec2 &targetSquare, int index, bool isWhite) override
	{
		if (isWhite)
		{
			diffBetweenSquares = targetSquare - whiteKingPositions[index];
		}
		else
		{
			diffBetweenSquares = targetSquare - blackKingPositions[index];
		}
	}

	virtual void restorePreviousMove(PieceAttribs attribs)
	{
		TrackMoves::restorePreviousMove(attribs, whiteKingPositions, blackKingPositions);
	}

	//Get position of king
	//--------------------
	static const glm::vec2 &getKingPosition(bool isWhite)
	{
		if (isWhite)
		{
			return whiteKingPositions[0];
		}
		else
		{
			return blackKingPositions[0];
		}
	}

	virtual ~King() = default;

private:
	unsigned int blackKing;
	unsigned int whiteKing;

	static std::vector<glm::vec2> whiteKingPositions;
	static std::vector<glm::vec2> blackKingPositions;
	std::vector<glm::vec3> whiteKingColors;
	std::vector<glm::vec3> blackKingColors;

	glm::vec2 diffBetweenSquares;
	std::vector<std::vector<PieceAttribs>> availableTargetSquares;
	
	Moves moves;
	std::vector<glm::vec2> kingMoves;
	bool kingMoveIncremented{ false };
		
	glm::vec3 castlingSquareColor{ 1.0, 0.5, 0.0 };
	glm::vec3 targetSquareColor{ 0.0, 0.8, 1.0 };
	glm::vec3 enemySquareColor{ 1.0, 0.0, 0.0 };
	
	void setInitKingPositions()
	{
		setKingMoves();
		Pieces::setupPositions(whiteKingPositions, whiteKingColors, 1, 4);
		Pieces::setupPositions(blackKingPositions, blackKingColors, 1, 60);
	}

	virtual void setupPieceOnSquareValues() override
	{
		kingMoveIncremented = false;
		PieceAttribs square;
		for (size_t i = 0; i < whiteKingPositions.size(); i++)
		{
			square.position = whiteKingPositions[i];
			square.index = i;
			square.isWhite = true;
			square.piece = this;
			square.pieceEnum = Piece::KING;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}

		for (size_t i = 0; i < blackKingPositions.size(); i++)
		{
			square.position = blackKingPositions[i];
			square.index = i;
			square.isWhite = false;
			square.piece = this;
			square.pieceEnum = Piece::KING;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}
	}
	
	void setKingMoves()
	{
		//King can move in all 8 directions but only 1 square
		kingMoves.push_back(glm::vec2(squareSizeX, 0.0));
		kingMoves.push_back(glm::vec2(0.0, squareSizeY));
		kingMoves.push_back(glm::vec2(squareSizeX, squareSizeY));
		kingMoves.push_back(glm::vec2(-squareSizeX, squareSizeY));
		kingMoves.push_back(-kingMoves[0]);
		kingMoves.push_back(-kingMoves[1]);
		kingMoves.push_back(-kingMoves[2]);
		kingMoves.push_back(-kingMoves[3]);
	}
};

std::vector<glm::vec2> King::whiteKingPositions{};
std::vector<glm::vec2> King::blackKingPositions{};

#endif
