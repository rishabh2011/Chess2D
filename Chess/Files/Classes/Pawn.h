/* -----------------------------------------------------------------------------------
   Specific piece classes derive from Base class Pieces. Comments apply to all classes
   ----------------------------------------------------------------------------------- */
#pragma once
#ifndef PAWN_H
#define PAWN_H

#include "Pieces.h"
#include "Board.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "King.h"
#include <PawnRules.h>
#include <vector>

class Pawn : public Pieces
{
public:

	//Constructor
	Pawn()
	{
		whitePawn = Graphics::loadTexture("Files/Textures/Pieces/White/white pawn.png", true);
		blackPawn = Graphics::loadTexture("Files/Textures/Pieces/Black/black pawn.png", true);
		setInitPawnPositions();
	}

	//-----------------
	//VIRTUAL FUNCTIONS
	//-----------------

	//Calculate all the possible target squares for the currently selected pawn using the pawnMoves vector
	//----------------------------------------------------------------------------------------------------
	virtual void calcTargetSquares(bool isWhite, int index = -1) override
	{
		if (index != -1)
		{
			if (isWhite)
			{
				moves.calcPawnTargetSquares(targetSquares, index, pawnMoves, whitePawnPositions, initPawnPositions, isWhite);
			}
			else
			{
				moves.calcPawnTargetSquares(targetSquares, index, pawnMoves, blackPawnPositions, initPawnPositions, isWhite);
			}
		}
		else
		{
			reversePawnMoves();
			if (isWhite)
			{
				for (size_t i{ 0 }; i < whitePawnPositions.size(); i++)
				{
					moves.calcPawnAttackedSquares(targetSquares, i, pawnMoves, whitePawnPositions, isWhite, King::blackKingPositions[0]);
				}
				squaresAttacked.insert(squaresAttacked.end(), targetSquares.begin(), targetSquares.end());
			}
			else
			{
				for (size_t i{ 0 }; i < blackPawnPositions.size(); i++)
				{
					moves.calcPawnAttackedSquares(targetSquares, i, pawnMoves, blackPawnPositions, isWhite, King::whiteKingPositions[0]);
				}
				squaresAttacked.insert(squaresAttacked.end(), targetSquares.begin(), targetSquares.end());
			}
			/*for (size_t i{ 0 }; i < Pieces::pieceOnSquare.size(); i++)
			{
				if (Pieces::pieceOnSquare[i].position == whitePawnPositions[3])
				{
					std::cout << std::boolalpha << Pieces::pieceOnSquare[i].attackingOpponentKing << std::endl;
				}
			}*/
			targetSquares.clear();
			reversePawnMoves();
		}
	}

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

	//Draw all the possible target squares for pawn  
	//---------------------------------------------
	virtual void drawMoves(GLFWwindow* window, int selectedPieceIndex, bool isWhite) override
	{
		if (targetSquares.size() == 0)
		{
			calcTargetSquares(isWhite, selectedPieceIndex);
		}

		//checkIfPieceIsPinned(targetSquares, selectedPieceIndex, pieces, isWhite);
		
		//Loop through the targetSquares vector
		for (size_t i = 0; i < targetSquares.size(); i++)
		{
			if (targetSquares[i].piece)
			{
				Board::drawTargetSquare(enemySquareColor, targetSquares[i].position, window, targetSquares[i].piece, targetSquares[i].index, isWhite);
			}
			else
			{
				Board::drawTargetSquare(targetSquareColor, targetSquares[i].position, window, targetSquares[i].piece, targetSquares[i].index, isWhite);
			}
			if (Board::status == PieceStatus::MOVING)
			{
				targetSquares.clear();
				return;
			}
		}
		//If mouse is clicked on an invalid square
		if (Mouse::getMouseClicked())
		{
			Board::status = PieceStatus::NOT_SELECTED;
			targetSquares.clear();
		}
	}

	/*void checkIfPieceIsPinned(std::vector<glm::vec2> &targetSquares, int selectedPieceIndex, const std::vector<Pieces *> &pieces, bool isWhite)
	{
		if (King::kingAttacked)
		{
			return;
		}

		Pieces::squaresAttacked.clear();
		int squareIndex;
		if (isWhite)
		{
			squareIndex = Pieces::getSquareIndex(whitePawnPositions[selectedPieceIndex]);
		}
		else
		{
			squareIndex = Pieces::getSquareIndex(blackPawnPositions[selectedPieceIndex]);
		}
		Pieces::pieceOnSquare[squareIndex] = false;

		for (size_t i = 0; i < pieces.size(); i++)
		{
			pieces[i]->checkIfAttackingKing(isWhite);
		}
		
		if (King::kingAttacked)
		{
			moveAllowed[selectedPieceIndex] = false;
			isPinned[selectedPieceIndex] = true;
			for (size_t i = 0; i < targetSquares.size(); i++)
			{
				bool squareAttacked = false;
				for (size_t j = 0; j < Pieces::squaresAttacked.size(); j++)
				{
					if (targetSquares[i] == squaresAttacked[j])
					{
						moveAllowed[selectedPieceIndex] = true;
						squareAttacked = true;
					}
				}
				if (!squareAttacked)
				{
					targetSquares[i] = glm::vec2(2.0, 2.0);
				}
			}
			King::kingAttacked = false;
			Pieces::pieceOnSquare[squareIndex] = true;
			if (Mouse::getMouseClicked())
			{
				Board::status = PieceStatus::NOT_SELECTED;
			}
			return;
		}

		isPinned[selectedPieceIndex] = false;
		Pieces::pieceOnSquare[squareIndex] = true;
	}*/

	//Move pawn to the target square
	//If target square is a pawn promotion square, promote the pawn to a user selected piece
	//--------------------------------------------------------------------------------------
	virtual void movePiece(const glm::vec2 &targetSquare, int pieceIndex, float deltaTime, bool isWhite, const std::vector<Pieces*> &pieces, GLFWwindow* window) override
	{
		if (pawnRules.checkPawnPromotion(&targetSquare))
		{
			std::cout << "1. Queen" << std::endl;
			std::cout << "2. Rook" << std::endl;
			std::cout << "3. Bishop" << std::endl;
			std::cout << "4. Knight" << std::endl;
			pawnRules.processInput(window);
			//Do not move pawn until a piece for promotion is selected
			if (!pawnRules.getPawnPromoted())
			{
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
	virtual void switchPiecePositions() override
	{
		Pieces::switchPiecePositions(whitePawnPositions);
		Pieces::switchPiecePositions(blackPawnPositions);
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
				whitePawnPositions[index] = glm::vec2(2.0, 2.0);
			}
			else
			{
				blackPawnPositions[index] = glm::vec2(2.0, 2.0);
			}
		}
		else
		{
			if (isWhite)
			{
				blackPawnPositions[index] = glm::vec2(2.0, 2.0);
			}
			else
			{
				whitePawnPositions[index] = glm::vec2(2.0, 2.0);
			}
		}
	}

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

	//Getter functions
	//----------------

	//Returns the piece position at the given index
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

	//Promote pawn to the desired piece
	//---------------------------------
	void promotePawn(const std::vector<Pieces*> &pieces, const glm::vec2 &targetSquare, bool isWhite, int pieceIndex)
	{
		if (pawnRules.getPawnPromoted())
		{
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
			pawnRules.setPawnPromoted(false);
			switch (pawnRules.getPawnPromotionPiece())
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
		
	//Check if the target square is a square which can be used to promote the pawn
	//----------------------------------------------------------------------------
	

	virtual void setMoveAllowed(bool val) override
	{
		for (size_t i = 0; i < moveAllowed.size(); i++)
		{
			moveAllowed[i] = val;
		}
	}

	virtual bool getMoveAllowed(int pieceIndex) override
	{
		return moveAllowed[pieceIndex];
	}

	virtual bool noPieceCanMove() override
	{
		for (size_t i = 0; i < moveAllowed.size(); i++)
		{
			if (moveAllowed[i])
			{
				return false;
			}
		}
		return true;
	}

	virtual ~Pawn()
	{}
	
private:
	//---------------
	//Class variables
	//---------------

	//Texture variables
	unsigned int blackPawn;
	unsigned int whitePawn;
		
	//-------------------------
	//Position and color arrays
	std::vector<glm::vec2> whitePawnPositions;
	std::vector<glm::vec3> whitePawnColors;
	std::vector<glm::vec2> blackPawnPositions;
	std::vector<glm::vec3> blackPawnColors;

	glm::vec2 diffBetweenSquares;
	std::vector<PieceAttribs> targetSquares;
		
	//Stores initial pawn positions
	std::vector<glm::vec2> initPawnPositions;
	
	PawnRules pawnRules;

	//Movement variables
	Moves moves;
	std::vector<glm::vec2> pawnMoves;
	std::vector<bool> moveAllowed;

	//Variables used for drawing target squares
	std::vector<bool> isPinned;
	int targetPieceIndex;
	glm::vec3 targetSquareColor{ 0.0, 0.8, 1.0 };
	glm::vec3 enemySquareColor{ 1.0, 0.0, 0.0 };
	bool pieceExists;
		
	//Sets up initial pawn positions. Calls base class setupPositions function
	void setInitPawnPositions()
	{
		setPawnMoves();
		Pieces::setupPositions(whitePawnPositions, whitePawnColors, 8, 8);
		Pieces::setupPositions(blackPawnPositions, blackPawnColors, 8, 48);

		for (size_t i = 0; i < whitePawnPositions.size(); i++)
		{
			initPawnPositions.push_back(whitePawnPositions[i]);
		}

		for (int i = 56; i < 64; i++)
		{
			pawnRules.setPawnPromotionSquares(Pieces::squarePositions[i]);
		}

		for (size_t i = 0; i < 8; i++)
		{
			moveAllowed.push_back(true);
			isPinned.push_back(false);
		}
	}

	virtual void setupPieceOnSquareValues() override
	{
		PieceAttribs square;
		for (size_t i = 0; i < whitePawnPositions.size(); i++)
		{
			square.position = whitePawnPositions[i];
			square.index = i;
			square.isWhite = true;
			square.piece = this;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}

		for (size_t i = 0; i < blackPawnPositions.size(); i++)
		{
			square.position = blackPawnPositions[i];
			square.index = i;
			square.isWhite = false;
			square.piece = this;
			square.attackingOpponentKing = false;
			pieceOnSquare.push_back(square);
		}
	}

	//------------------------
	//Setup all the pawn moves
	void setPawnMoves()
	{
		//Distance between each square center is 0.25
		pawnMoves.push_back(glm::vec2(0.25, 0.25));
		pawnMoves.push_back(glm::vec2(-0.25, 0.25));
		pawnMoves.push_back(glm::vec2(0.0, 0.25));
		pawnMoves.push_back(glm::vec2(0.0, 0.5));
	}
};

#endif
