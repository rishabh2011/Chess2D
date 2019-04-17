/* ------------------------------------------------------------------
   Base class Pieces. Sets up positions for all pieces and draws them
   ------------------------------------------------------------------ */
#ifndef PIECES_H
#define PIECES_H

#include <Graphics.h>
#include <vector>
#include <stack>

//Need the enum as class interdependency errors occur when #including queen, bishop, rook in kingRules class
enum class Piece {PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING};

class Pieces;

//Struct holds all necessary attributes of a piece
struct PieceAttribs
{
	glm::vec2 position;
	int index = -1;  //piece index within its vector
	bool isWhite; //piece white or black
	Pieces *piece = nullptr;
	Piece pieceEnum;  //piece type
	bool attackingOpponentKing = false; 

	//Castling booleans
	bool castlingKingSideSquare = false;
	bool castlingQueenSideSquare = false;

	//EnPassant booleans
	bool rightEnPassantMove = false;
	bool leftEnPassantMove = false;
};

//Data related to newly generated piece at pawn promotion
struct PromotedPieceAttribs
{
	Pieces* promotedPiece;
	bool isWhite;
};

class Pieces
{
public:

	static std::vector<PieceAttribs> pieceOnSquare;  //stores the position of all the pieces on the board
	static std::vector<PieceAttribs> squaresAttacked; //stores the squares attacked by opponent pieces

	//Board square variables
	static std::vector<glm::vec2> squarePositions;
	static std::vector<glm::vec3> squareColors;

	//King attacked variables
	static bool kingAttacked;
	static int kingAttackingPieces;  //Store total attacking pieces. If attacking pieces are more than 1, only king is allowed to move

	//Variables used to check possibility of castling
	static int whiteKingMoved;
	static int blackKingMoved;
	static std::vector<int> whiteRookMoved;
	static std::vector<int> blackRookMoved;
	
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

	virtual void addSelectedPieceDataToOnePieceAffectedMovesStack(bool isWhite, int selectedPieceIndex) = 0;

	virtual void movePiece(const glm::vec2 &targetSquare, int pieceIndex, float deltaTime, bool isWhite, const std::vector<Pieces*> &pieces, GLFWwindow* window) = 0;

	virtual void switchPiecePositions() = 0;
	
	virtual void deletePiecePosition(int pieceIndex, bool isWhite, bool isPawnPromote = false) = 0;

	virtual void calcDifferenceBetweenSquares(const glm::vec2 &targetSquare, int index, bool isWhite) = 0;

	virtual void clearAvailableTargetSquares() = 0;

	virtual void restorePreviousMove(PieceAttribs attrib) = 0;

	virtual void deletePromotedPiece(bool isWhite)
	{

	}
	
	//Draws king square as red if in check
	//Only overriden in king class
	//------------------------------------
	virtual void drawKingInCheck(bool isWhite)
	{

	}

	//Adds the apropriate castled rook's data to the twoPiecesAffectedMoves stack
	//---------------------------------------------------------------------------
	virtual void addCastledRookDataToTwoPiecesAffectedMovesStack(bool isWhite, bool castlingKingSide, bool castlingQueenSide)
	{

	}
	
	//Determines which rook to move for castling and moves that rook
	//Only override in rook class
	//--------------------------------------------------------------
	virtual void moveCastlingRook(bool isWhite, float deltaTime, const std::vector<Pieces*> &pieces, GLFWwindow* window)
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

	//Increment Rook Moves
	//--------------------
	static void incrementRookMove(int pieceIndex, bool isWhite)
	{
		//Only increment for 0 and 1 pieceIndex as 
		//1 : pieceIndex will cause out of range exception 
		//2 : not required to track move number for generated piece. That piece cannot castle
		if (pieceIndex <= 1)
		{
			if (isWhite)
			{
				whiteRookMoved[pieceIndex]++;
			}
			else
			{
				blackRookMoved[pieceIndex]++;
			}
		}
	}

	//Decrement Rook Moves
	//--------------------
	static void decrementRookMove(int pieceIndex, bool isWhite)
	{
		if (pieceIndex <= 1)
		{
			if (isWhite)
			{
				whiteRookMoved[pieceIndex]--;
			}
			else
			{
				blackRookMoved[pieceIndex]--;
			}
		}
	}
	
	//Increment King Moves
	//--------------------
	static void incrementKingMove(bool isWhite)
	{
		if (isWhite)
		{
			whiteKingMoved++;
		}
		else
		{
			blackKingMoved++;
		}
	}

	//Decrement King Moves
	//--------------------
	static void decrementKingMove(bool isWhite)
	{
		if (isWhite)
		{
			whiteKingMoved--;
		}
		else
		{
			blackKingMoved--;
		}
	}
	
	virtual ~Pieces() = default;
};

std::vector<PieceAttribs> Pieces::pieceOnSquare{};
std::vector<glm::vec2> Pieces::squarePositions{};
std::vector<glm::vec3> Pieces::squareColors{};
std::vector<PieceAttribs> Pieces::squaresAttacked{};
bool Pieces::kingAttacked = false;
int Pieces::kingAttackingPieces{ 0 };
int Pieces::whiteKingMoved{ 0 };
int Pieces::blackKingMoved{ 0 };
std::vector<int> Pieces::whiteRookMoved{ 0, 0 };
std::vector<int> Pieces::blackRookMoved{ 0, 0 };

#endif

