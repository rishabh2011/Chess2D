/* ------------------------------------------------------------------
   Base class Pieces. Sets up positions for all pieces and draws them
   ------------------------------------------------------------------ */
#ifndef PIECES_H
#define PIECES_H

#include <Graphics.h>
#include <vector>

class Pieces;

//Struct holds all necessary attributes of a piece
struct PieceAttribs
{
	glm::vec2 position;
	int index;  //piece index within its vector
	bool isWhite; //piece white or black
	bool attackingOpponentKing = false;  
	Pieces* piece;  //piece type
};

class Pieces
{

public:
	//Board square positions array
	static std::vector<PieceAttribs> pieceOnSquare;  //stores the position of all the pieces on the board
	static std::vector<PieceAttribs> squaresAttacked; //stores the squares attacked by opponent pieces
	static std::vector<glm::vec2> squarePositions;
	static std::vector<glm::vec3> squareColors;
	static bool kingAttacked;
	static int kingAttackingPieces;  //Store total attacking pieces. If attacking pieces are more than 1, only king is allowed to move
	
	//-----------------
	//Virtual Functions
	//-----------------

	virtual void draw(int index = -1, bool isWhite = false) const = 0;

	virtual void drawColor(int index = -1, bool isWhite = false) const = 0;

	virtual int drawPieceOutline(const glm::vec3* color, bool isPlayerWhite) const = 0;

	virtual void drawMoves(GLFWwindow* window, int selectedPieceIndex, bool isWhite, const std::vector<Pieces*> &pieces) = 0;

	virtual void calcTargetSquares(bool isWhite, bool currentPlayer, const std::vector<Pieces*> &pieces = std::vector<Pieces*>{}) = 0;
	
	virtual void setupPieceOnSquareValues() = 0;
			
	virtual const glm::vec2& getPiecePosition(int index, bool isWhite) const = 0;

	virtual void movePiece(const glm::vec2 &targetSquare, int pieceIndex, float deltaTime, bool isWhite, const std::vector<Pieces*> &pieces, GLFWwindow* window) = 0;

	virtual void switchPiecePositions() = 0;
	
	virtual void deletePiecePosition(int pieceIndex, bool isWhite, bool isPawnPromote = false) = 0;

	virtual void calcDifferenceBetweenSquares(const glm::vec2 &targetSquare, int index, bool isWhite) = 0;

	virtual void clearAvailableTargetSquares() = 0;
	
	//Draws king square as red if in check
	//Only overriden in king class
	//------------------------------------
	virtual void drawKingInCheck(bool isWhite)
	{

	}

	//----------------
	//Normal functions
	//----------------

	void switchPiecePositions(std::vector<glm::vec2> &positions)
	{
		for (size_t i = 0; i < positions.size(); i++)
		{
			positions[i] = -positions[i];
		}
	}
		
	//Draw pieces visible to user
	//---------------------------
	void drawPieces(const unsigned int* texture, const std::vector<glm::vec2> &positions, int startIndex, int endIndex, bool highlight = false) const
	{
		Graphics::drawBoard(Graphics::getBoardShader(), texture, positions, startIndex, endIndex, 0.8, highlight);
	}

	//Draw pieces for mouse hit detection
	//-----------------------------------
	void drawPiecesColor(const unsigned int *texture, const std::vector<glm::vec2> &positions,
		const std::vector<glm::vec3> &colors, int startIndex, int endIndex, bool highlight = false) const
	{
		if (highlight)
		{
			Graphics::drawBoard(Graphics::getPiecesColorShader(), texture, positions, startIndex, endIndex, 0.86, highlight, glm::vec3(0.0, 1.0, 0.0), colors);
		}
		else
		{
			Graphics::drawBoard(Graphics::getPiecesColorShader(), texture, positions, startIndex, endIndex, 0.8, highlight, glm::vec3(0.0, 1.0, 0.0), colors);
		}
	}

	//Setup initial piece positions
	//-----------------------------
	void setupPositions(std::vector<glm::vec2> &positions, std::vector<glm::vec3> &colors, int numPieces, int squareIndex1, int squareIndex2 = -1)
	{
		//Per side number of pieces
		//Pawn
		if (numPieces == 8)
		{
			for (int i = 0; i < numPieces; i++)
			{
				positions.push_back(squarePositions[squareIndex1++]);
				colors.push_back(Graphics::genRandomColor());
			}
		}
		//Bishop, Knight, Rook
		else if(numPieces == 2)
		{
			positions.push_back(squarePositions[squareIndex1]);
			colors.push_back(Graphics::genRandomColor());
			positions.push_back(squarePositions[squareIndex2]);
			colors.push_back(Graphics::genRandomColor());
		}
		//King, Queen
		else if (numPieces == 1)
		{
			positions.push_back(squarePositions[squareIndex1]);
			colors.push_back(Graphics::genRandomColor());
		}
	}
		
	//Returns the index of the color value under mouse cursor
	//-------------------------------------------------------
	int pieceColorIndex(const std::vector<glm::vec3> &colors, int numPieces, const glm::vec3* color) const
	{
		for (int i = 0; i < numPieces; i++)
		{
			if (*color == colors[i])
			{
				return i;
			}
		}
		return -1;
	}
	
	virtual ~Pieces() = default;
};

std::vector<PieceAttribs> Pieces::pieceOnSquare{};
std::vector<glm::vec2> Pieces::squarePositions{};
std::vector<glm::vec3> Pieces::squareColors{};
std::vector<PieceAttribs> Pieces::squaresAttacked{};
bool Pieces::kingAttacked = false;
int Pieces::kingAttackingPieces{ 0 };

#endif

