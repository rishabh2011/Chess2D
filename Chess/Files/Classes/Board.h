/*-----------------------------------------------------------------------------------------
  Class sets up board squares position, handles mouse hits on pieces and draws entire board
  ----------------------------------------------------------------------------------------- */
#ifndef BOARD_H
#define BOARD_H

#include <Pieces.h>
#include <Mouse.h>
#include <UndoMoves.h>
#include <vector>

 //------------
 //Enum classes
 //------------

//Piece status containing all possible states of a piece
enum class PieceStatus { NOT_SELECTED, SELECTED, MOVING };
//Player enum containing white and black
enum class Player { WHITE, BLACK };

class Board
{
public:

	//Enum variables
	static Player player;
	static PieceStatus status;
		
	//Square textures
	static unsigned int lightSquareTexture;
	static unsigned int darkSquareTexture;

	static float boardLimitX;
	static float boardLimitY;

	//Stores old and new square values for the previously played piece as well as the piece data
	//for highlight purposes
	static std::pair<std::pair<glm::vec2, glm::vec2>, PieceAttribs> highlightMove;
	static std::stack<std::pair<std::pair<glm::vec2, glm::vec2>, PieceAttribs>> highlightMovesStack;
	
	//Constructor
	//-----------
	Board()
	{
		setupSquarePositions();
		lightSquareTexture = Graphics::loadTexture("Files/Textures/Board/square gray light.png", false);
		darkSquareTexture = Graphics::loadTexture("Files/Textures/Board/square gray dark.png", false);
	}

	//Perform various functions based on mouse click and mouse cursor position
	//------------------------------------------------------------------------
	void playGame(std::vector<Pieces*> &pieces, float deltaTime, GLFWwindow* window)
	{
		//Draw the chess board
		draw(pieces);
		//Get the color under mouse cursor
		glm::vec3 color = Mouse::getPixelColorUnderMouse(window, Graphics::getPiecesColorBuffer());
		
		if (player == Player::WHITE)
		{
			isWhite = true;
		}
		else if (player == Player::BLACK)
		{
			isWhite = false;
		}
		
		//If squares not setup
		if (!squaresSetup)
		{
			//Store the piece positions on the board
			for (size_t i{ 0 }; i < pieces.size(); i++)
			{
				pieces[i]->setupPieceOnSquareValues();
			}
			//Calculate all the squares attacked by opponent pieces
			for (size_t i{ 0 }; i < pieces.size(); i++)
			{
				pieces[i]->calcTargetSquares(!isWhite, false);
			}
			//Calculate all the squares available for current player
			for (size_t i{ 0 }; i < pieces.size(); i++)
			{
				pieces[i]->calcTargetSquares(isWhite, true, pieces);
			}
			squaresSetup = true;
		}

		//For king piece, draw in red if checked
		for (size_t i{ 0 }; i < pieces.size(); i++)
		{
			pieces[i]->drawKingInCheck(isWhite);
		}

		//Perform various operations based on piece status
		switch (status)
		{
		case PieceStatus::NOT_SELECTED:
			highlightPreviousMove();
			drawPiecesOutline(pieces, &color, isWhite);
			break;
		case PieceStatus::SELECTED:
			highlightPreviousMove();
			pieceSelected(pieces, window, isWhite);
			break;
		case PieceStatus::MOVING:
			movePiece(pieces, window, selectedPieceIndex, deltaTime, isWhite);
			break;
		}
	}


	//Draw the target square that a piece can move to
	//-----------------------------------------------
	static void drawTargetSquare(glm::vec3& targetSquareColor, const glm::vec2 &targetSquare, GLFWwindow* window, Pieces* &enemyPiece, int targetPieceIndex, bool isWhite, 
		std::vector<Pieces*> pieces = std::vector<Pieces*>{}, bool castlingKingSideSquare = false, bool castlingQueenSideSquare = false, bool rightEnPassantMove = false, bool leftEnPassantMove = false)
	{
		//Get the squareIndex of the currently selected targetSquare
		squareIndex = getSquareIndex(targetSquare);
		//If targetSquare is outside the board
		if (squareIndex == -1)
		{
			return;
		}

		//If mouse is clicked
		if (Mouse::getMouseClicked())
		{
			//Get the color of the pixel under the mouse
			glm::vec3 color = Mouse::getPixelColorUnderMouse(window, Graphics::getSquaresColorBuffer());

			//Get the index of the color from the Pieces::squareColors array that matches the pixel color
			int squareColorIndex = getSquareIndex(color);
			//If mouse clicked on invalid square
			if (squareColorIndex == -1)
			{
				return;
			}
			if (Pieces::squarePositions[squareColorIndex] == targetSquare)
			{
				processPieceMove(castlingKingSideSquare, castlingQueenSideSquare, rightEnPassantMove, leftEnPassantMove, pieces, isWhite, targetSquare, enemyPiece);
			}
		}
		//Highlight target squares
		Graphics::drawBoard(Graphics::getPiecesColorShader(), &lightSquareTexture, Pieces::squarePositions, squareIndex, squareIndex, 0.95, true, targetSquareColor);
		Graphics::drawBoard(Graphics::getBoardShader(), getSquareTexture(squareIndex), Pieces::squarePositions, squareIndex, squareIndex, 0.90);
		//If the target square contains an enemy piece, draw that enemy piece over the square again
		if (enemyPiece)
		{
			enemyPiece->draw(targetPieceIndex, !isWhite);
		}
	}

	//Processes the selected piece's move before moving it
	//----------------------------------------------------
	static void processPieceMove(bool castlingKingSideSquare, bool castlingQueenSideSquare, bool rightEnPassantMove, bool leftEnPassantMove, std::vector<Pieces*> &pieces, bool isWhite, const glm::vec2 &targetSquare, Pieces* &enemyPiece)
	{
		//Universal
		//---------
		TrackMoves::moveNumber++;
		status = PieceStatus::MOVING;
		selectedTargetSquare = targetSquare;
		piece->addSelectedPieceDataToOnePieceAffectedMovesStack(isWhite, selectedPieceIndex);
		piece->calcDifferenceBetweenSquares(targetSquare, selectedPieceIndex, isWhite);

		//Set the values required to highlight this move
		highlightMove.first.first = piece->getPiecePosition(selectedPieceIndex, isWhite);  //Old square
		highlightMove.first.second = targetSquare;  //New square
		for (size_t i{ 0 }; i < Pieces::pieceOnSquare.size(); i++)
		{
			if (highlightMove.first.first == Pieces::pieceOnSquare[i].position)
			{
				highlightMove.second = Pieces::pieceOnSquare[i];  //Piece Data
			}
		}
		highlightMovesStack.push(highlightMove);
						
		//Handle Castling
		//---------------
		if (castlingKingSideSquare)
		{
			castlingKingSide = true;
		}
		else if (castlingQueenSideSquare)
		{
			castlingQueenSide = true;
		}
		for (size_t i{ 0 }; i < pieces.size(); i++)
		{
			pieces[i]->addCastledRookDataToTwoPiecesAffectedMovesStack(isWhite, castlingKingSide, castlingQueenSide);
		}

		//Handle killed piece data and enPassant
		//--------------------------------------
		if (enemyPiece != nullptr)
		{
			if (rightEnPassantMove)
			{
				enPassant = true;
				//Set enPassant square to the square right of the selected pawn
				enPassantEnemyPieceSquare = piece->getPiecePosition(selectedPieceIndex, isWhite) + glm::vec2(squareSizeX, 0.0); 
				TrackMoves::addKilledPieceDataToTwoPiecesAffectedMovesStack(enPassantEnemyPieceSquare);
			}
			else if (leftEnPassantMove)
			{
				enPassant = true;
				//Set enPassant square to the square left of the selected pawn
				enPassantEnemyPieceSquare = piece->getPiecePosition(selectedPieceIndex, isWhite) - glm::vec2(squareSizeX, 0.0);
				TrackMoves::addKilledPieceDataToTwoPiecesAffectedMovesStack(enPassantEnemyPieceSquare);
			}
			else
			{
				TrackMoves::addKilledPieceDataToTwoPiecesAffectedMovesStack(targetSquare);
			}
		}
	}
	
	//Switch players and reset game vars
	//----------------------------------
	static void switchPlayer(const std::vector<Pieces*> &pieces)
	{
		resetGameVars(pieces);

		//Switch player
		if (player == Player::WHITE)
		{
			player = Player::BLACK;
		}
		else if (player == Player::BLACK)
		{
			player = Player::WHITE;
		}
	}

	//Resets variables holding various game conditions
	//------------------------------------------------
	static void resetGameVars(const std::vector<Pieces*> &pieces)
	{
		//Set status to not selected
		status = PieceStatus::NOT_SELECTED;
		//Invert piece positions since both players should be able to play the game from the same perspective
		for (int i = 0; i < pieces.size(); i++)
		{
			pieces[i]->switchPiecePositions();
			pieces[i]->clearAvailableTargetSquares();
		}
		squaresSetup = false;
		Pieces::kingAttacked = false;
		Pieces::kingAttackingPieces = 0;
		Pieces::pieceOnSquare.clear();
		Pieces::squaresAttacked.clear();

		castlingKingSide = false;
		castlingQueenSide = false;

		enPassant = false;
	}

	//Restore board state to previous move 
	//------------------------------------
	static void restorePreviousMove(const std::vector<Pieces*> &pieces)
	{
		if (!TrackMoves::onePieceAffectedMoves.empty())
		{
			//Switch back to the other player
			switchPlayer(pieces);
			
			//Restore the previous state for the piece that was moved
			PieceAttribs attribs;
			attribs = TrackMoves::onePieceAffectedMoves.top();
			TrackMoves::onePieceAffectedMoves.pop();
			attribs.piece->restorePreviousMove(attribs);
			//If the piece was a king, decrement its move(this is required to allow for castling if the king move number becomes 0)
			if (attribs.pieceEnum == Piece::KING)
			{
				Pieces::decrementKingMove(attribs.isWhite);
			}
			//If the piece was a rook, decrement its move(this is required to allow for castling if the rook move number becomes 0)
			else if (attribs.pieceEnum == Piece::ROOK)
			{
				Pieces::decrementRookMove(attribs.index, attribs.isWhite);
			}

			//Restore the previous state for the killed piece or the rook that was castled
			if (!TrackMoves::twoPiecesAffectedMoves.empty())
			{
				std::pair<PieceAttribs, unsigned int> pieceData;
				pieceData = TrackMoves::twoPiecesAffectedMoves.top();
				//if pieceData move number matches current move number
				if (pieceData.second == TrackMoves::moveNumber)
				{
					TrackMoves::twoPiecesAffectedMoves.pop();
					pieceData.first.piece->restorePreviousMove(pieceData.first);
					if (pieceData.first.pieceEnum == Piece::ROOK)
					{
						Pieces::decrementRookMove(pieceData.first.index, pieceData.first.isWhite);
					}
				}
			}

			//Deletes the generated piece on pawn promotion
			if (!TrackMoves::threePiecesAffectedMoves.empty())
			{
				std::pair<PromotedPieceAttribs, unsigned int> generatedPiece;
				generatedPiece = TrackMoves::threePiecesAffectedMoves.top();
				//if pieceData move number matches current move number
				if (generatedPiece.second == TrackMoves::moveNumber)
				{
					TrackMoves::threePiecesAffectedMoves.pop();
					generatedPiece.first.promotedPiece->deletePromotedPiece(generatedPiece.first.isWhite);
				}
			}

			//Restore the previous highlight squares values
			//Decrement move number
			highlightMovesStack.pop();
			TrackMoves::moveNumber--;
		}
	}

	//Get the index value of the position of the square that matches the piece position from Pieces::squarePositions array
	//------------------------------------------------------------------------------------------------------------
	static int getSquareIndex(const glm::vec2& position)
	{
		int index;
		for (int i = 0; i < 64; i++)
		{
			/*glm::vec2 squarePosition;
			squarePosition.x = (int)(Pieces::squarePositions[i].x * 100);
			squarePosition.y = (int)(Pieces::squarePositions[i].y * 100);

			glm::vec2 piecePosition;
			piecePosition.x = (int)(position.x * 100);
			piecePosition.y = (int)(position.y * 100);*/

			if (Pieces::squarePositions[i] == position)
			{
				index = i;
				return index;
			}
		}
		return -1;
	}
	
	//Returns a ref to square texture required by king class
	//------------------------------------------------------
	static const unsigned int &getTexture()
	{
		return lightSquareTexture;
	}

	//Functions return castle direction variables required by rook class
	//------------------------------------------------------------------
	static bool getCastlingKingSide()
	{
		return castlingKingSide;
	}
	static bool getCastlingQueenSide()
	{
		return castlingQueenSide;
	}
	
	//Destructor
	//----------
	~Board()
	{}

private:
	//---------------
	//Class Variables
	//---------------
	static Pieces* piece;
	static bool squaresSetup;
		
	//Square positions
	static float boardGapX;
	static float boardGapY;
	static std::vector<glm::vec2> lightSquarePositions;
	static std::vector<glm::vec2> darkSquarePositions;
			
	//Piece specific variables
	static int squareIndex;
	static int selectedPieceSquareIndex;
	static int selectedPieceIndex;
	static glm::vec2 selectedTargetSquare;
	bool pieceExists = false;
	bool movingPiece = false;

	//Castling vars
	static bool castlingKingSide;
	static bool castlingQueenSide;

	//EnPassant vars
	static bool enPassant;
	static glm::vec2 enPassantEnemyPieceSquare;

	bool isWhite;
	
	//Draw board squares and pieces visible to user
	//---------------------------------------------
	void draw(std::vector<Pieces*> &pieces)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, Graphics::getSquaresColorBuffer());
		glClear(GL_COLOR_BUFFER_BIT);
		Graphics::drawBoard(Graphics::getPiecesColorShader(), &lightSquareTexture, Pieces::squarePositions, 0, 63, 1.0, false, glm::vec3(1.0, 1.0, 1.0), Pieces::squareColors);

		glBindFramebuffer(GL_FRAMEBUFFER, Graphics::getPiecesColorBuffer());
		glClear(GL_COLOR_BUFFER_BIT);
		drawColor(pieces);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Graphics::drawBoard(Graphics::getBoardShader(), &lightSquareTexture, lightSquarePositions, 0, 31, 1.0);
		Graphics::drawBoard(Graphics::getBoardShader(), &darkSquareTexture, darkSquarePositions, 0, 31, 1.0);
		for (Pieces* piece : pieces)
		{
			piece->draw();
		}
	}

	//Draw pieces with color for mouse hit detection
	//----------------------------------------------
	void drawColor(std::vector<Pieces*> &pieces)
	{
		for (size_t i = 0; i < pieces.size(); i++)
		{
			pieces[i]->drawColor();
		}
	}

	//Highlights the last move played by the opponent
	//-----------------------------------------------
	void highlightPreviousMove()
	{
		if (!highlightMovesStack.empty())
		{
			auto highlightMove = highlightMovesStack.top();

			//Draws the old square value for the piece in yellow
			squareIndex = getSquareIndex(-highlightMove.first.first);
			Graphics::drawBoard(Graphics::getBoardShader(), getSquareTexture(squareIndex), Pieces::squarePositions, squareIndex, squareIndex, 1.0);
			Graphics::drawBoard(Graphics::getPiecesColorShader(), &lightSquareTexture, Pieces::squarePositions, squareIndex, squareIndex, 0.95, true, glm::vec3(1.0, 1.0, 0.0));
			Graphics::drawBoard(Graphics::getBoardShader(), getSquareTexture(squareIndex), Pieces::squarePositions, squareIndex, squareIndex, 0.90);

			//Draws the new square value for the piece in yellow
			squareIndex = getSquareIndex(-highlightMove.first.second);
			Graphics::drawBoard(Graphics::getBoardShader(), getSquareTexture(squareIndex), Pieces::squarePositions, squareIndex, squareIndex, 1.0);
			Graphics::drawBoard(Graphics::getPiecesColorShader(), &lightSquareTexture, Pieces::squarePositions, squareIndex, squareIndex, 0.95, true, glm::vec3(1.0, 1.0, 0.0));
			Graphics::drawBoard(Graphics::getBoardShader(), getSquareTexture(squareIndex), Pieces::squarePositions, squareIndex, squareIndex, 0.90);

			//Draws the piece itself over the new square
			highlightMove.second.piece->draw(highlightMove.second.index, highlightMove.second.isWhite);
		}
	}

	//--------------------------------
	//Setup square positions functions
	//--------------------------------

	//Assigns alternating square positions to light and dark square position variables
	//and calls Graphics function to setup instancing 
	void setupSquarePositions()
	{
		calcSquarePositions();

		//calcSquarePositions stores square values one after the other from bottom left corner to top right corner
		//However right most square of a row has the same color as the left most square of the next row in a chess board
		//Hence a bool variable to check for row end and accordingly store the next row square position in the same array
		bool lightSquare = false;
		for (int i = 0; i < Pieces::squarePositions.size(); i++)
		{
			//Bottom left square in a chess board is always dark
			if (i % 8 == 0 && i != 0) //No need to execute for 1st row
			{
				lightSquare = !lightSquare;
			}
			if (lightSquare)
			{
				lightSquarePositions.push_back(Pieces::squarePositions[i]);
			}
			else
			{
				darkSquarePositions.push_back(Pieces::squarePositions[i]);
			}
			lightSquare = !lightSquare;
		}
	}
	//---------------------------
	//Calculates square positions
	void calcSquarePositions()
	{
		//Divide window coordinates by 8 as chess board consists of 64 squares
		//Convert from window coordinates to opengl coordinates
		squareSizeX = (((SCR_WIDTH - boardGapX) / 8.0) / SCR_WIDTH * 2.0);
		squareSizeY = (((SCR_HEIGHT - boardGapY) / 8.0) / SCR_HEIGHT * 2.0);

		boardLimitX = 1 - squareSizeX / 2.0;
		boardLimitY = 1 - squareSizeY / 2.0;

		//Start from the bottom corner of the window
		//Generate offset of center of each square from bottom corner :
		// as instance rendering
		//offsets the center position of an object by the given value after generating the object
		float x = -1, y = -1;
		for (int i = 0; i < 64; i++)
		{
			//If 8 square positions have been calculated in the current row move to next row
			if (i % 8 == 0 && i != 0)
			{
				y += squareSizeY;
				x = -1;
			}

			glm::vec2 centerOffset;
			//Divide squareSize by 2 as we are offseting center which is half of squareSize
			centerOffset.x = x + (squareSizeX / 2.0) + boardGapX / SCR_WIDTH;
			centerOffset.y = y + (squareSizeY / 2.0) + boardGapY / SCR_HEIGHT;
			Pieces::squarePositions.push_back(centerOffset);
			Pieces::squareColors.push_back(Graphics::genRandomColor());
			x += squareSizeX;
		}

		Graphics::setShadersAndVAO();
	}

	//------------------------------
	//Piece status related functions
	//------------------------------

	//Highlights the piece currently under mouse cursor
	//-------------------------------------------------
	void drawPiecesOutline(std::vector<Pieces*> &pieces, const glm::vec3* color, bool isWhite)
	{
		for (size_t i = 0; i < pieces.size(); i++)
		{
			selectedPieceIndex = pieces[i]->drawPieceOutline(color, isWhite);
			if (selectedPieceIndex != -1)
			{
				if (Mouse::getMouseClicked())
				{
					piece = pieces[i];
					status = PieceStatus::SELECTED;
				}
				break;
			}
		}
	}

	//Selects the piece and displays all squares it can move to
	//---------------------------------------------------------
	void pieceSelected(std::vector<Pieces*> &pieces, GLFWwindow* window, bool isWhite)
	{
		//Draw the square of the currently selected piece in white indicating current piece selection
		//-------------------------------------------------------------------------------------------
		selectedPieceSquareIndex = getSquareIndex(piece->getPiecePosition(selectedPieceIndex, isWhite));
		Graphics::drawBoard(Graphics::getPiecesColorShader(), &lightSquareTexture, Pieces::squarePositions, selectedPieceSquareIndex, selectedPieceSquareIndex, 1.0, true);
		piece->draw(selectedPieceIndex, isWhite);

		//Draw moves for the currently selected piece
		piece->drawMoves(window, selectedPieceIndex, isWhite, pieces);
	}

	//Moves the piece to the selected target square
	//---------------------------------------------
	void movePiece(std::vector<Pieces*> &pieces, GLFWwindow* window, int pieceIndex, float deltaTime, bool isWhite)
	{
		piece->movePiece(selectedTargetSquare, pieceIndex, deltaTime, isWhite, pieces, window);

		//If castling move, then move the appropriate rook
		if (castlingKingSide || castlingQueenSide)
		{
			for (size_t i{ 0 }; i < pieces.size(); i++)
			{
				pieces[i]->moveCastlingRook(isWhite, deltaTime, pieces, window);
			}
		}
		//If piece position matches target square position, player move has ended; switch player
		if (piece->getPiecePosition(pieceIndex, isWhite).x == selectedTargetSquare.x && piece->getPiecePosition(pieceIndex, isWhite).y == selectedTargetSquare.y)
		{
			for (size_t i = 0; i < Pieces::pieceOnSquare.size(); i++)
			{
				//If the move is an enPassant move
				if (enPassant)
				{
					//"Delete" the piece on the enPassant square
					if (enPassantEnemyPieceSquare == Pieces::pieceOnSquare[i].position)
					{
						Pieces::pieceOnSquare[i].piece->deletePiecePosition(Pieces::pieceOnSquare[i].index, isWhite);
					}
				}
				//If an enemy piece was on a target square, delete that piece from the board
				else if (selectedTargetSquare == Pieces::pieceOnSquare[i].position)
				{
					Pieces::pieceOnSquare[i].piece->deletePiecePosition(Pieces::pieceOnSquare[i].index, isWhite);
				}
			}
			switchPlayer(pieces);
		}
	}
	
	//----------------
	//Getter Functions
	//----------------

	//Get the index value of the position of the square that matches the position of the target square selected
	//---------------------------------------------------------------------------------------------------------
	static int getSquareIndex(const glm::vec3& color)
	{
		int index;
		for (int i = 0; i < 64; i++)
		{
			if (Pieces::squareColors[i] == color)
			{
				index = i;
				return index;
			}
		}
		return -1;
	}

	//Get the square texture to be used to draw over the colored square
	//-----------------------------------------------------------------
	static unsigned int* getSquareTexture(int squareIndex)
	{
		for (int i = 0; i < 32; i++)
		{
			if (Pieces::squarePositions[squareIndex] == lightSquarePositions[i])
			{
				return &lightSquareTexture;
			}
			else if (Pieces::squarePositions[squareIndex] == darkSquarePositions[i])
			{
				return &darkSquareTexture;
			}
		}
	}

};

//static variables initialization
//-------------------------------
bool Board::squaresSetup{ false };

Pieces* Board::piece{ nullptr };

float Board::boardGapX{ 0 };
float Board::boardGapY{ 0 };
unsigned int Board::lightSquareTexture{ 0 };
unsigned int Board::darkSquareTexture{ 0 };
std::vector<glm::vec2> Board::lightSquarePositions{};
std::vector<glm::vec2> Board::darkSquarePositions{};
float Board::boardLimitX{ 0 };
float Board::boardLimitY{ 0 };

PieceStatus Board::status{ PieceStatus::NOT_SELECTED };
Player Board::player{ Player::WHITE };

int Board::squareIndex{ -1 };
int Board::selectedPieceSquareIndex{ -1 };
int Board::selectedPieceIndex{ -1 };

glm::vec2 Board::selectedTargetSquare{};

bool Board::castlingKingSide{ false };
bool Board::castlingQueenSide{ false };

std::pair<std::pair<glm::vec2, glm::vec2>, PieceAttribs> Board::highlightMove{};
std::stack<std::pair<std::pair<glm::vec2, glm::vec2>, PieceAttribs>> Board::highlightMovesStack{};

bool Board::enPassant{ false };
glm::vec2 Board::enPassantEnemyPieceSquare{};

#endif
