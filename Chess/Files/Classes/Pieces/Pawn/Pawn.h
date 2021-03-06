/* ------------------------------------------------------------------------------------------------------------------
   Specific piece classes derive from Base class Pieces. Universal piece function comments apply to all piece classes
   ------------------------------------------------------------------------------------------------------------------ */
#pragma once
#ifndef PAWN_H
#define PAWN_H

#include <Pieces.h>
#include <Board.h>
#include <Pieces/Queen/Queen.h>
#include <Pieces/Rook/Rook.h>
#include <Pieces/Knight/Knight.h>
#include <Pieces/Bishop/Bishop.h>
#include <Pieces/King/King.h>
#include <vector>
#include "PawnPromotion.h"
#include <UndoMoves.h>

class Pawn : public Pieces
{
public:

	//Constructor
	//-----------
	Pawn()
	{
		whitePawn = Graphics::loadTexture("Files/Textures/Pieces/White/white pawn.png", true);
		blackPawn = Graphics::loadTexture("Files/Textures/Pieces/Black/black pawn.png", true);
		setInitPawnPositions();
	}

	//Setup stenciling to draw outline of the piece under mouse cursor
	//----------------------------------------------------------------
	void pawnOutline(int index, bool isWhite) const
	{
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		draw(index, isWhite);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		drawColor(index, isWhite);
	}

	//Calculate all the possible target squares for the currently selected pawn using the pawnMoves vector
	//----------------------------------------------------------------------------------------------------
	virtual void calcTargetSquares(bool isWhite, bool currentPlayer, const std::vector<Pieces*> &pieces = std::vector<Pieces*>{}) override
	{
		std::vector<PieceAttribs> targetSquares;
		//If current player, calculate the available target squares for the current player
		if (currentPlayer)
		{
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whitePawnPositions.size(); i++)
				{
					moves.calcPawnTargetSquares(targetSquares, i, pawnMoves, whitePawnPositions, initPawnPositions, isWhite, pieces);
					availableTargetSquares.push_back(targetSquares);
					targetSquares.clear();
				}
			}
			else
			{
				if (Board::AIEnabled)
				{
					reversePawnMoves();
				}
				for (size_t i{ 0 }; i < blackPawnPositions.size(); i++)
				{
					moves.calcPawnTargetSquares(targetSquares, i, pawnMoves, blackPawnPositions, initPawnPositions, isWhite, pieces);
					availableTargetSquares.push_back(targetSquares);
					targetSquares.clear();
				}
				if (Board::AIEnabled)
				{
					reversePawnMoves();
				}
			}
		}
		//Calculate squares attacked by all opponent pawns
		//Need this to evaluate king safe squares as well as king attacked or not
		else
		{
			//As pawn positions have been switched, reverse pawn moves values before calculating target squares as pawns can move in only 1 direction
			reversePawnMoves();
			if (isWhite)
			{
				if (Board::AIEnabled)
				{
					reversePawnMoves();
				}
				for (size_t i{ 0 }; i < whitePawnPositions.size(); i++)
				{
					moves.calcPawnAttackedSquares(this, targetSquares, i, pawnMoves, whitePawnPositions, isWhite, King::getKingPosition(!isWhite));
				}
				if (Board::AIEnabled)
				{
					reversePawnMoves();
				}
			}
			else
			{
				for (size_t i{ 0 }; i < blackPawnPositions.size(); i++)
				{
					moves.calcPawnAttackedSquares(this, targetSquares, i, pawnMoves, blackPawnPositions, isWhite, King::getKingPosition(!isWhite));
				}
				
			}
			squaresAttacked.insert(squaresAttacked.end(), targetSquares.begin(), targetSquares.end());
			reversePawnMoves();
		}
	}

	//Reverses pawn moves vector values
	//---------------------------------
	void reversePawnMoves()
	{
		for (size_t i{ 0 }; i < pawnMoves.size(); i++)
		{
			pawnMoves[i] = -pawnMoves[i];
		}
	}

	//Draw pawn visible to user. Calls base class drawPieces function
	//---------------------------------------------------------------
	virtual void draw(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index value of the position that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPieces(&whitePawn, whitePawnPositions, index, index, true);
			}
			else
			{
				Pieces::drawPieces(&blackPawn, blackPawnPositions, index, index, true);
			}
		}
		else
		{
			Pieces::drawPieces(&whitePawn, whitePawnPositions, 0, whitePawnPositions.size() - 1);
			Pieces::drawPieces(&blackPawn, blackPawnPositions, 0, blackPawnPositions.size() - 1);
		}
	}

	//Draw pawn in color for setting up mouse hit detection. Calls base class drawPiecesColor function
	//------------------------------------------------------------------------------------------------
	virtual void drawColor(int index = -1, bool isWhite = false) const override
	{
		//Used to draw a specific piece based on an index of the position value that the mouse is pointing to
		if (index != -1)
		{
			if (isWhite)
			{
				Pieces::drawPiecesColor(&whitePawn, whitePawnPositions, whitePawnColors, index, index, true);
			}
			else
			{
				Pieces::drawPiecesColor(&blackPawn, blackPawnPositions, blackPawnColors, index, index, true);
			}
		}
		else
		{
			Pieces::drawPiecesColor(&whitePawn, whitePawnPositions, whitePawnColors, 0, whitePawnPositions.size() - 1);
			Pieces::drawPiecesColor(&blackPawn, blackPawnPositions, blackPawnColors, 0, blackPawnPositions.size() - 1);
		}
	}

	//Draws the check mating pieces in pink
	virtual void drawCheckMatingPieces(int index, bool isWhite) const override
	{
		if (isWhite)
		{
			//Get square index of the piece and color that square in pink
			int squareIndex = Board::getSquareIndex(whitePawnPositions[index]);
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
			int squareIndex = Board::getSquareIndex(blackPawnPositions[index]);
			if (squareIndex != -1)
			{
				Graphics::drawBoard(Graphics::getPiecesColorShader(), &Board::getTexture(), squarePositions, squareIndex, squareIndex, 1.0, true, glm::vec3(1.0, 0.5, 0.6));
			}
			//Finally draw the king back on top
			draw(index, isWhite);
		}
	}

	//Draw all the possible target squares for pawn  
	//---------------------------------------------
	virtual void drawMoves(GLFWwindow* window, int selectedPieceIndex, bool isWhite, const std::vector<Pieces*> &pieces) override
	{
		//Loop through the targetSquares vector and draw the target squares
		for (size_t i = 0; i < availableTargetSquares[selectedPieceIndex].size(); i++)
		{
			if (availableTargetSquares[selectedPieceIndex][i].rightEnPassantMove || availableTargetSquares[selectedPieceIndex][i].leftEnPassantMove)
			{
				Board::drawTargetSquare(enemySquareColor, availableTargetSquares[selectedPieceIndex][i].position, window, availableTargetSquares[selectedPieceIndex][i].piece,
					availableTargetSquares[selectedPieceIndex][i].index, isWhite, pieces, false, false, availableTargetSquares[selectedPieceIndex][i].rightEnPassantMove, availableTargetSquares[selectedPieceIndex][i].leftEnPassantMove);
			}
			//If an opponent piece exists on target square, draw the square in red
			else if (availableTargetSquares[selectedPieceIndex][i].piece)
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

	//Calculate the difference between the selected target square and the selected piece position
	//-------------------------------------------------------------------------------------------
	virtual void calcDifferenceBetweenSquares(const glm::vec2 &targetSquare, int index, bool isWhite) override
	{
		if (isWhite)
		{
			diffBetweenSquares = targetSquare - whitePawnPositions[index];
		}
		else
		{
			diffBetweenSquares = targetSquare - blackPawnPositions[index];
		}
	}
	
	//Calls TrackMoves function for the same to add this pawn's data to the stack
	//---------------------------------------------------------------------------
	virtual void addSelectedPieceDataToOnePieceAffectedMovesStack(bool isWhite, int selectedPieceIndex) override
	{
		TrackMoves::addSelectedPieceDataToOnePieceAffectedMovesStack(isWhite, selectedPieceIndex, whitePawnPositions, blackPawnPositions);
	}

	//Move pawn to the target square
	//If target square is a pawn promotion square, promote the pawn to a user selected piece
	//--------------------------------------------------------------------------------------
	virtual void movePiece(const glm::vec2 &targetSquare, int pieceIndex, float deltaTime, bool isWhite, const std::vector<Pieces*> &pieces, GLFWwindow* window) override
	{
		//Draw the selected piece again 
		//In situations where a kill occurs, this makes sure that the selected piece is always being rendered
		//On top of the enemy piece and not under it in some situations
		draw(pieceIndex, isWhite);

		if (promotion.checkPawnPromotion(&targetSquare))
		{
			//Do not move pawn until a piece for promotion is selected
			if (!promotion.getPawnPromoted())
			{
				promotion.draw(window, deltaTime, isWhite);
				return;
			}
		}

		//Move pawn
		if (isWhite)
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, pawnMoves, whitePawnPositions);
		}
		else
		{
			moves.movePiece(targetSquare, diffBetweenSquares, pieceIndex, deltaTime, pawnMoves, blackPawnPositions);
		}

		//Once pawn reaches target square, promote it
		if(getPiecePosition(pieceIndex, isWhite).x == targetSquare.x && getPiecePosition(pieceIndex, isWhite).y == targetSquare.y)
		{
			promotePawn(pieces, targetSquare, isWhite, pieceIndex);
		}
	}
	
	//Invert pawn position values
	//---------------------------
	virtual void switchPiecePositions() override
	{
		Pieces::switchPiecePositions(whitePawnPositions);
		Pieces::switchPiecePositions(blackPawnPositions);
	}

	//Clears the available target squares vector for next iteration
	//-------------------------------------------------------------
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
		
	//Get the index of the piece based on the color of the pixel under mouse color and draw that piece's outline
	//----------------------------------------------------------------------------------------------------------
	virtual int drawPieceOutline(const glm::vec3* color, bool isWhite) const override
	{
		int val;
		if (isWhite)
		{
			val = Pieces::pieceColorIndex(whitePawnColors, whitePawnColors.size(), color);
		}
		else
		{
			val = Pieces::pieceColorIndex(blackPawnColors, blackPawnColors.size(), color);
		}
		if (val != -1)
		{
			pawnOutline(val, isWhite);
			return val;
		}
		return -1;
	}

	//Sets the piece position to a value outside opengl coordinates
	//-------------------------------------------------------------
	virtual void deletePiecePosition(int index, bool isWhite, bool isPawnPromote = false) override
	{
		if (isPawnPromote)
		{
			if (isWhite)
			{
				whitePawnPositions[index] = glm::vec2(100.0, 100.0);
			}
			else
			{
				blackPawnPositions[index] = glm::vec2(100.0, 100.0);
			}
		}
		else
		{
			if (isWhite)
			{
				blackPawnPositions[index] = glm::vec2(100.0, 100.0);
			}
			else
			{
				whitePawnPositions[index] = glm::vec2(100.0, 100.0);
			}
		}
	}

	//Returns the piece position at the given index
	//---------------------------------------------
	virtual const glm::vec2& getPiecePosition(int index, bool isWhite) const override
	{
		if (isWhite)
		{
			return whitePawnPositions[index];
		}
		else
		{
			return blackPawnPositions[index];
		}
	}

	virtual const std::vector<glm::vec2>& getWhitePiecePositions() const override
	{
		return whitePawnPositions;
	}

	virtual const std::vector<glm::vec2>& getBlackPiecePositions() const override
	{
		return blackPawnPositions;
	}

	virtual void setWhitePiecePositions(std::vector<glm::vec2> whitePositions) override
	{
		for (size_t i{ 0 }; i < whitePawnPositions.size(); i++)
		{
			whitePawnPositions[i] = whitePositions[i];
		}
	}

	virtual void setBlackPiecePositions(std::vector<glm::vec2> blackPositions) override
	{
		for (size_t i{ 0 }; i < blackPawnPositions.size(); i++)
		{
			blackPawnPositions[i] = blackPositions[i];
		}
	}

	//Promote pawn to the desired piece
	//---------------------------------
	void promotePawn(const std::vector<Pieces*> &pieces, const glm::vec2 &targetSquare, bool isWhite, int pieceIndex)
	{
		if (promotion.getPawnPromoted())
		{
			Board::highlightMove.first.first = Board::highlightMovesStack.top().first.first;
			Board::highlightMove.first.second = Board::highlightMovesStack.top().first.second;
			Board::highlightMovesStack.pop();
			for (size_t i = 0; i < pieceOnSquare.size(); i++)
			{
				int targetPieceIndex;
				//If an enemy piece was on a target square, delete that piece from the board
				if (targetSquare == pieceOnSquare[i].position)
				{
					pieceOnSquare[i].piece->deletePiecePosition(pieceOnSquare[i].index, isWhite);
				}
			}
			deletePiecePosition(pieceIndex, isWhite, true);
			promotion.setPawnPromoted(false);
			switch (promotion.getPawnPromotionPiece())
			{
			case PawnPromotionPiece::QUEEN:
				for (size_t i = 0; i < pieces.size(); i++)
				{
					Queen *queen = dynamic_cast<Queen*>(pieces[i]);
					if (queen != nullptr)
					{
						queen->generateNewQueen(&targetSquare, isWhite);
					}
				}
				break;
			case PawnPromotionPiece::BISHOP:
				for (size_t i = 0; i < pieces.size(); i++)
				{
					Bishop *bishop = dynamic_cast<Bishop*>(pieces[i]);
					if (bishop != nullptr)
					{
						bishop->generateNewBishop(&targetSquare, isWhite);
					}
				}
				break;
			case PawnPromotionPiece::ROOK:
				for (size_t i = 0; i < pieces.size(); i++)
				{
					Rook *rook = dynamic_cast<Rook*>(pieces[i]);
					if (rook != nullptr)
					{
						rook->generateNewRook(&targetSquare, isWhite);
					}
				}
				break;
			case PawnPromotionPiece::KNIGHT:
				for (size_t i = 0; i < pieces.size(); i++)
				{
					Knight *knight = dynamic_cast<Knight*>(pieces[i]);
					if (knight != nullptr)
					{
						knight->generateNewKnight(&targetSquare, isWhite);
					}
				}
				break;
			}
			//Switch players after pawn promotion is complete
			Board::switchPlayer(pieces);
		}
	}
	
	//Calls TrackMoves function for the same to restore the previous values for this pawn
	//-----------------------------------------------------------------------------------
	virtual void restorePreviousMove(PieceAttribs attribs)
	{
		TrackMoves::restorePreviousMove(attribs, whitePawnPositions, blackPawnPositions);
	}

	//Destructor
	//----------
	virtual ~Pawn()
	{}
	
private:
	//---------------
	//Class variables
	//---------------

	//Texture variables
	unsigned int blackPawn;
	unsigned int whitePawn;
		
	//Position and color arrays
	std::vector<glm::vec2> initPawnPositions;
	std::vector<glm::vec2> whitePawnPositions;
	std::vector<glm::vec3> whitePawnColors;
	std::vector<glm::vec2> blackPawnPositions;
	std::vector<glm::vec3> blackPawnColors;

	//Target square variables
	glm::vec2 diffBetweenSquares;
	std::vector<std::vector<PieceAttribs>> availableTargetSquares;
	
	PawnPromotion promotion;
	float scale = 0;

	//Movement variables
	Moves moves;
	std::vector<glm::vec2> pawnMoves;

	//Variables used for drawing target squares
	glm::vec3 targetSquareColor{ 0.0, 0.8, 1.0 };
	glm::vec3 enemySquareColor{ 1.0, 0.0, 0.0 };
		
	//Sets up initial pawn positions. Calls base class setupPositions function
	//------------------------------------------------------------------------
	void setInitPawnPositions()
	{
		setPawnMoves();
		Pieces::setupPositions(whitePawnPositions, whitePawnColors, 8, 8);
		Pieces::setupPositions(blackPawnPositions, blackPawnColors, 8, 48);

		//Set initial pawn positions and pawn promotion square positions
		for (size_t i = 0; i < whitePawnPositions.size(); i++)
		{
			initPawnPositions.push_back(whitePawnPositions[i]);
		}
		//i = 56 to 64 implies all top rank squares
		//As position switches for black player, squares will remain the same
		for (int i = 56; i < 64; i++)
		{
			promotion.setPawnPromotionSquares(Pieces::squarePositions[i]);
		}
	}

	//Set piece on square vector values for all pawn pieces
	//-----------------------------------------------------
	virtual void setupPieceOnSquareValues() override
	{
		PieceAttribs square;
		for (size_t i = 0; i < whitePawnPositions.size(); i++)
		{
			square.position = whitePawnPositions[i];
			square.index = i;
			square.isWhite = true;
			square.piece = this;
			square.pieceEnum = Piece::PAWN;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}

		for (size_t i = 0; i < blackPawnPositions.size(); i++)
		{
			square.position = blackPawnPositions[i];
			square.index = i;
			square.isWhite = false;
			square.piece = this;
			square.pieceEnum = Piece::PAWN;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}
	}

	//Setup all the pawn moves
	//------------------------
	void setPawnMoves()
	{
		//Distance between each square center is 0.25

		//Pawn moves are:
		//2 diagonal killing moves
		//2 straight moves, double square move only possible at initial position
		pawnMoves.push_back(glm::vec2(squareSizeX, squareSizeY));
		pawnMoves.push_back(glm::vec2(-squareSizeX, squareSizeY));
		pawnMoves.push_back(glm::vec2(0.0, squareSizeY));
		pawnMoves.push_back(glm::vec2(0.0, squareSizeY * 2.0));
	}
};

#endif
