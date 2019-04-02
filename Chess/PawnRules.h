#ifndef PAWNRULES_H
#define PAWNRULES_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Files/Classes/Pieces.h"
#include "Files/Classes/Board.h"

enum class PawnPromotionPiece { NO_PIECE, QUEEN, ROOK, BISHOP, KNIGHT };

class PawnRules
{
public:

	bool squareUnderAttack(glm::vec2 &targetSquare, int pieceIndex, const std::vector<glm::vec2> &pawnPositions, bool isWhite, const glm::vec2 &kingPosition)
	{
		if (insideBoard(targetSquare))
		{
			//Killing moves
			if (targetSquare.x != pawnPositions[pieceIndex].x)
			{
				checkIfAttackingOpponentKing(targetSquare, pawnPositions, pieceIndex, isWhite, kingPosition);
				return true;
			}
			return false;
		}
		return false;
	}

	bool isValidSquare(glm::vec2 &targetSquare, Pieces* &enemyPiece, int pieceIndex, const std::vector<glm::vec2> &pawnPositions, std::vector<glm::vec2> &initPawnPositions, bool isWhite)
	{
		if (insideBoard(targetSquare))
		{
			//Straight moves
			if (targetSquare.x == pawnPositions[pieceIndex].x)
			{
				if (straightMovePossible(targetSquare, pieceIndex, pawnPositions, initPawnPositions))
				{
					if (Pieces::kingAttacked)
					{
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
			//Killing moves
			else
			{
				if (killMovePossible(targetSquare, pieceIndex, pawnPositions, enemyPiece, isWhite))
				{
					if (Pieces::kingAttacked)
					{
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

	bool insideBoard(glm::vec2 &targetSquare)
	{
		//X and Y limits of board
		if (targetSquare.x > 0.875 || targetSquare.x < -0.875 || targetSquare.y > 0.875 || targetSquare.y < -0.875)
		{
			return false;
		}
		return true;
	}

	bool straightMovePossible(glm::vec2 &targetSquare, int pieceIndex, const std::vector<glm::vec2> &pawnPositions, std::vector<glm::vec2> &initPawnPositions)
	{
		//If a piece is blocking pawn movement then automatically return false
		if (pieceBlockingMovement)
		{
			return false;
		}
		//Check Double square move
		else if (targetSquare.y == (pawnPositions[pieceIndex].y + Board::squareSizeY * 2.0))
		{
			//If pawn is at initial position
			if (pawnPositions[pieceIndex] == initPawnPositions[pieceIndex])
			{
				if (pieceIsOnTargetSquare(targetSquare))
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
		else if (pieceIsOnTargetSquare(targetSquare))
		{
			pieceBlockingMovement = true;
			return false;
		}
		return true;
	}

	bool killMovePossible(glm::vec2 &targetSquare, int pieceIndex, const std::vector<glm::vec2> &pawnPositions, Pieces* &enemyPiece, bool isWhite)
	{
		for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
		{
			//If targetsquare has an opponent piece
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

	void checkIfAttackingOpponentKing(const glm::vec2 &targetSquare, const std::vector<glm::vec2> pawnPositions, int pieceIndex, bool isWhite, const glm::vec2 &kingPosition)
	{
		if(targetSquare == kingPosition)
		{
			Pieces::kingAttacked = true;
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

	bool pieceIsOnTargetSquare(glm::vec2 & targetSquare)
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

	void setPieceBlockingMovement(bool val)
	{
		pieceBlockingMovement = val;
	}

	bool checkPawnPromotion(const glm::vec2 *targetSquare)
	{
		for (size_t i = 0; i < pawnPromotionSquares.size(); i++)
		{
			if (*targetSquare == pawnPromotionSquares[i])
			{
				return true;
			}
		}
		return false;
	}

	//Promote pawn to a specific piece based on user input
	//----------------------------------------------------
	void processInput(GLFWwindow* window)
	{
		if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		{
			pawnPromoted = true;
			promotionPiece = PawnPromotionPiece::QUEEN;
		}
		else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
		{
			pawnPromoted = true;
			promotionPiece = PawnPromotionPiece::ROOK;

		}
		else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
		{
			pawnPromoted = true;
			promotionPiece = PawnPromotionPiece::BISHOP;

		}
		else if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
		{
			pawnPromoted = true;
			promotionPiece = PawnPromotionPiece::KNIGHT;
		}
	}

	void setPawnPromotionSquares(glm::vec2 val)
	{
		pawnPromotionSquares.push_back(val);
	}

	bool getPawnPromoted()
	{
		return pawnPromoted;
	}

	void setPawnPromoted(bool val)
	{
		pawnPromoted = val;
	}

	PawnPromotionPiece getPawnPromotionPiece()
	{
		return promotionPiece;
	}

private:
	bool pieceBlockingMovement;

	//Pawn promotion variables
	std::vector<glm::vec2> pawnPromotionSquares;
	bool pawnPromoted = false;
	PawnPromotionPiece promotionPiece = PawnPromotionPiece::NO_PIECE;
};

#endif
