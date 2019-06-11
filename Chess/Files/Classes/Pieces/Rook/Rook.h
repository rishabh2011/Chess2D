/* ---------------------------------------------------------------------------------------------------------------
  Universal piece functions have been commented in pawn class. Only piece specific functions commented in this class
  ---------------------------------------------------------------------------------------------------------------- */
#ifndef ROOK_H
#define ROOK_H

#include <Pieces.h>
#include <Board.h>
#include <Pieces/King/King.h>
#include <Moves.h>
#include <UndoMoves.h>

class Rook : public Pieces
{
public:

	Rook()
	{
		whiteRook = Graphics::loadTexture("Files/Textures/Pieces/White/white rook.png", true);
		blackRook = Graphics::loadTexture("Files/Textures/Pieces/Black/black rook.png", true);
		setInitRookPositions();
	}
	
	void rookOutline(int index, bool isWhite) const
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
				for (size_t i{ 0 }; i < whiteRookPositions.size(); i++)
				{
					moves.calcRookTargetSquares(targetSquares, i, rookMoves, whiteRookPositions, isWhite, pieces);
					availableTargetSquares.push_back(targetSquares);
					targetSquares.clear();
				}
			}
			else
			{
				for (size_t i{ 0 }; i < blackRookPositions.size(); i++)
				{
					moves.calcRookTargetSquares(targetSquares, i, rookMoves, blackRookPositions, isWhite, pieces);
					availableTargetSquares.push_back(targetSquares);
					targetSquares.clear();
				}
			}
		}
		else
		{
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whiteRookPositions.size(); i++)
				{
					moves.calcRookAttackedSquares(this, targetSquares, i, rookMoves, whiteRookPositions, isWhite, King::getKingPosition(!isWhite));
				}
			}
			else
			{
				for (size_t i{ 0 }; i < blackRookPositions.size(); i++)
				{
					moves.calcRookAttackedSquares(this, targetSquares, i, rookMoves, blackRookPositions, isWhite, King::getKingPosition(!isWhite));
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
				Pieces::drawPieces(&whiteRook, whiteRookPositions, index, index, true);
			}
			else
			{
				Pieces::drawPieces(&blackRook, blackRookPositions, index, index, true);
			}
		}
		else
		{
			Pieces::drawPieces(&whiteRook, whiteRookPositions, 0, whiteRookPositions.size() - 1);
			Pieces::drawPieces(&blackRook, blackRookPositions, 0, blackRookPositions.size() - 1);
		}
	}

	virtual void drawColor(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index of the position value that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPiecesColor(&whiteRook, whiteRookPositions, whiteRookColors, index, index, true);
			}
			else
			{
				Pieces::drawPiecesColor(&blackRook, blackRookPositions, blackRookColors, index, index, true);
			}
		}
		else
		{
			Pieces::drawPiecesColor(&whiteRook, whiteRookPositions, whiteRookColors, 0, whiteRookPositions.size() - 1);
			Pieces::drawPiecesColor(&blackRook, blackRookPositions, blackRookColors, 0, blackRookPositions.size() - 1);
		}
	}

	//Draws the check mating pieces in pink
	virtual void drawCheckMatingPieces(int index, bool isWhite) const override
	{
		if (isWhite)
		{
			//Get square index of the piece and color that square in pink
			int squareIndex = Board::getSquareIndex(whiteRookPositions[index]);
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
			int squareIndex = Board::getSquareIndex(blackRookPositions[index]);
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
		TrackMoves::addSelectedPieceDataToOnePieceAffectedMovesStack(isWhite, selectedPieceIndex, whiteRookPositions, blackRookPositions);
	}

	//Calls TrackMoves function for the same to add the castled rook's data to the stack
	//----------------------------------------------------------------------------------
	virtual void addCastledRookDataToTwoPiecesAffectedMovesStack(bool isWhite, bool castlingKingSide, bool castlingQueenSide) override
	{
		TrackMoves::addCastledRookDataToTwoPiecesAffectedMovesStack(isWhite, castlingKingSide, castlingQueenSide, whiteRookPositions, blackRookPositions);
	}

	virtual void movePiece(const glm::vec2 &targetSquare, int pieceIndex, float deltaTime, bool isWhite, const std::vector<Pieces*> &pieces, GLFWwindow* window) override
	{
		draw(pieceIndex, isWhite);

		if (isWhite)
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, rookMoves, whiteRookPositions);
		}
		else
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, rookMoves, blackRookPositions);
		}
		//Increment this rook's move once
		if (!rookMoveIncremented)
		{
			incrementRookMove(pieceIndex, isWhite);
			rookMoveIncremented = true;
		}
	}

	//Moves the rook to its designated target square on king castle
	//-------------------------------------------------------------
	virtual void moveCastlingRook(bool isWhite, float deltaTime, const std::vector<Pieces*> &pieces, GLFWwindow* window) override
	{
		float queenSideRookMoveSpeed = 1.05f;  //multiply deltaTime with this value as queen side rook has to travel greater distance
		if (isWhite)
		{
			if (Board::getCastlingKingSide())
			{
				diffBetweenSquares = whiteKingSideRookCastlingSquare - initRookPositions[1];
				movePiece(whiteKingSideRookCastlingSquare, 1, deltaTime, isWhite, pieces, window);
			}
			else if (Board::getCastlingQueenSide())
			{
				diffBetweenSquares = whiteQueenSideRookCastlingSquare - initRookPositions[0];
				//multiplied deltaTime with 1.2 as this rook has to travel 3 squares compared to king's 2 squares within the same time
				movePiece(whiteQueenSideRookCastlingSquare, 0, deltaTime * queenSideRookMoveSpeed, isWhite, pieces, window);  
			}
		}
		else
		{
			if (Board::getCastlingKingSide())
			{
				diffBetweenSquares = blackKingSideRookCastlingSquare - initRookPositions[0];
				movePiece(blackKingSideRookCastlingSquare, 1, deltaTime, isWhite, pieces, window);
			}
			else if (Board::getCastlingQueenSide())
			{
				diffBetweenSquares = blackQueenSideRookCastlingSquare - initRookPositions[1];
				//multiplied deltaTime with 1.2 as this rook has to travel 3 squares compared to king's 2 squares within the same time
				movePiece(blackQueenSideRookCastlingSquare, 0, deltaTime * queenSideRookMoveSpeed, isWhite, pieces, window);
			}
		}
	}

	virtual void switchPiecePositions() override
	{ 
		Pieces::switchPiecePositions(whiteRookPositions);
		Pieces::switchPiecePositions(blackRookPositions);
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
			val = Pieces::pieceColorIndex(whiteRookColors, whiteRookColors.size(), color);
		}
		else
		{
			val = Pieces::pieceColorIndex(blackRookColors, blackRookColors.size(), color);
		}
		if (val != -1)
		{
			rookOutline(val, isWhite);
			return val;
		}
		return -1;
	}
	
	virtual const glm::vec2& getPiecePosition(int index, bool isWhite) const override
	{
		if (isWhite)
		{
			return whiteRookPositions[index];
		}
		else
		{
			return blackRookPositions[index];
		}
	}

	virtual const std::vector<glm::vec2>& getWhitePiecePositions() const override
	{
		return whiteRookPositions;
	}

	virtual const std::vector<glm::vec2>& getBlackPiecePositions() const override
	{
		return blackRookPositions;
	}

	virtual void setWhitePiecePositions(std::vector<glm::vec2> whitePositions) override
	{
		for (size_t i{ 0 }; i < whiteRookPositions.size(); i++)
		{
			whiteRookPositions[i] = whitePositions[i];
		}
	}

	virtual void setBlackPiecePositions(std::vector<glm::vec2> blackPositions) override
	{
		for (size_t i{ 0 }; i < blackRookPositions.size(); i++)
		{
			blackRookPositions[i] = blackPositions[i];
		}
	}

	virtual void deletePiecePosition(int index, bool isWhite, bool isPawnPromote = false) override
	{
		if (isWhite)
		{
			blackRookPositions[index] = glm::vec2(100.0, 100.0);
		}
		else
		{
			whiteRookPositions[index] = glm::vec2(100.0, 100.0);
		}
	}

	//Generates a new rook at the target square where pawn gets promoted
	//------------------------------------------------------------------
	void generateNewRook(const glm::vec2* targetSquare, bool isWhite)
	{
		if (isWhite)
		{
			//Add a new rook to the rook position vector 
			whiteRookPositions.push_back(*targetSquare);
			//Also generate a new color for the new rook
			whiteRookColors.push_back(Graphics::genRandomColor());
			Board::highlightMove.second.index = whiteRookPositions.size() - 1;
			Board::highlightMove.second.piece = this;
		}
		else
		{
			//Add a new rook to the rook position vector 
			blackRookPositions.push_back(*targetSquare);
			//Also generate a new color for the new rook
			blackRookColors.push_back(Graphics::genRandomColor());
			Board::highlightMove.second.index = blackRookPositions.size() - 1;
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
			diffBetweenSquares = targetSquare - whiteRookPositions[index];
		}
		else
		{
			diffBetweenSquares = targetSquare - blackRookPositions[index];
		}
	}

	virtual void restorePreviousMove(PieceAttribs attribs)
	{
		TrackMoves::restorePreviousMove(attribs, whiteRookPositions, blackRookPositions);
	}

	virtual void deletePromotedPiece(bool isWhite) override
	{
		TrackMoves::deleteGeneratedPiece(isWhite, whiteRookPositions, blackRookPositions);
	}

	//Returns the rook texture used in the pawnPromotion UI
	//-----------------------------------------------------
	static unsigned int getRookTexture(bool isWhite)
	{
		if (isWhite)
		{
			return whiteRook;
		}
		else
		{
			return blackRook;
		}
	}
			
	virtual ~Rook() = default;
	
private:
	static unsigned int blackRook;
	static unsigned int whiteRook;
		
	std::vector<glm::vec2> whiteRookPositions;
	std::vector<glm::vec3> whiteRookColors;
	std::vector<glm::vec2> blackRookPositions;
	std::vector<glm::vec3> blackRookColors;

	glm::vec2 diffBetweenSquares;
	std::vector<std::vector<PieceAttribs>> availableTargetSquares;

	Moves moves;
	std::vector<glm::vec2> rookMoves;
	bool rookMoveIncremented{ false };
			
	//Castling variables
	glm::vec2 whiteKingSideRookCastlingSquare;
	glm::vec2 whiteQueenSideRookCastlingSquare;
	glm::vec2 blackKingSideRookCastlingSquare;
	glm::vec2 blackQueenSideRookCastlingSquare;
	std::vector<glm::vec2> initRookPositions;
	
	glm::vec3 targetSquareColor{ 0.0, 0.8, 1.0 };
	glm::vec3 enemySquareColor{ 1.0, 0.0, 0.0 };
			
	void setInitRookPositions()
	{
		setRookMoves();
		Pieces::setupPositions(whiteRookPositions, whiteRookColors, 2, 0, 7);
		Pieces::setupPositions(blackRookPositions, blackRookColors, 2, 56, 63);
		for (size_t i{ 0 }; i < whiteRookPositions.size(); i++)
		{
			initRookPositions.push_back(whiteRookPositions[i]);
		}
		whiteKingSideRookCastlingSquare.x = initRookPositions[1].x - squareSizeX * 2.0;
		whiteKingSideRookCastlingSquare.y = -Board::boardLimitY;
		whiteQueenSideRookCastlingSquare.x = initRookPositions[0].x + squareSizeX * 3.0;
		whiteQueenSideRookCastlingSquare.y = -Board::boardLimitY;
		blackKingSideRookCastlingSquare.x = initRookPositions[0].x + squareSizeX * 2.0;
		blackKingSideRookCastlingSquare.y = -Board::boardLimitY;
		blackQueenSideRookCastlingSquare.x = initRookPositions[1].x - squareSizeX * 3.0;
		blackQueenSideRookCastlingSquare.y = -Board::boardLimitY;
	}

	virtual void setupPieceOnSquareValues() override
	{
		rookMoveIncremented = false;
		PieceAttribs square;
		for (size_t i = 0; i < whiteRookPositions.size(); i++)
		{
			square.position = whiteRookPositions[i];
			square.index = i;
			square.isWhite = true;
			square.piece = this;
			square.pieceEnum = Piece::ROOK;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}

		for (size_t i = 0; i < blackRookPositions.size(); i++)
		{
			square.position = blackRookPositions[i];
			square.index = i;
			square.isWhite = false;
			square.piece = this;
			square.pieceEnum = Piece::ROOK;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}
	}

	void setRookMoves()
	{
		//Rook can move straight in 4 directions for a maximum of 7 squares
		for (int i = 0; i < 7; i++)
		{
			rookMoves.push_back(glm::vec2((i + 1) * squareSizeX, 0.0));
			rookMoves.push_back(-glm::vec2((i + 1) * squareSizeX, 0.0));
			rookMoves.push_back(glm::vec2(0.0, (i + 1) * squareSizeY));
			rookMoves.push_back(-glm::vec2(0.0, (i + 1) * squareSizeY));
		}
	}
};

unsigned int Rook::whiteRook = 0;
unsigned int Rook::blackRook = 0;

#endif
