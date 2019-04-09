/* ------------------------------------------------------------------------------------------------------
   Class renders UI for pawn promotion piece selection as well has handles user input for piece selection
   ------------------------------------------------------------------------------------------------------ */
#ifndef PAWNPROMOTION_H
#define PAWNPROMOTION_H

#include "Graphics.h"
#include <Pieces/Rook/Rook.h>
#include <Pieces/Knight/Knight.h>
#include <Pieces/Bishop/Bishop.h>
#include <Pieces/Queen/Queen.h>
#include <Mouse.h>

enum class PawnPromotionPiece { NO_PIECE = -1, ROOK, KNIGHT, BISHOP, QUEEN };

class PawnPromotion
{
private:
	//Variables used to render rectangle box overlay
	unsigned int VAO;
	Shader* shader;
	float scaleX{ 0.7 };  //half-width of rectangular box
	float scaleY{ 0.4 };  //half-height of rectangular box
	//This value stores the scale of the entire rectangular box 
	//which starts at 0 and increases over time for the fade in effect
	float scale;  

	//Transition variables (used for the fade in effect of rectangular box)
	float transitionSpeed = 5.0;
	bool transitionOver{ false };
		
	//Pawn promotion variables
	std::vector<glm::vec3> pieceColors;  //stores a random color for each piece
	std::vector<glm::vec2> pawnPromotionSquares;  //stores all pawn promotion squares
	bool pawnPromoted = false;
	PawnPromotionPiece promotionPiece;
	
	//Sets up VAO for rectangular box
	//-------------------------------
	void setupVAO()
	{
		//z value is 0.5 to facilitate blending
		float vertices[] =
		{
			-scaleX, -scaleY, 
			 scaleX, -scaleY, 
			 scaleX,  scaleY, 
			 scaleX,  scaleY,  
			-scaleX,  scaleY, 
			-scaleX, -scaleY 
		};
		unsigned int VBO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	//Draws pieces over rectangle overlay
	//-----------------------------------
	void drawPieces(GLFWwindow* window, bool isWhite)
	{
		float offsetX = scaleX / 4.0;  
		glBindVertexArray(Graphics::getVAO());

		Shader* shader = Graphics::getPiecesColorShader();
		shader->use();
		shader->setFloat("scale", 0.8);

		glm::vec3 color;

		//Draw pieces in a color from pieceColors array
		//---------------------------------------------
		color = pieceColors[0] / 255.0f;
		shader->setVec3("color", color);
		glBindTexture(GL_TEXTURE_2D, Rook::getRookTexture(isWhite));
		shader->setVec2("offset", glm::vec2(0.0 - offsetX * 3.0, 0.0));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		color = pieceColors[1] / 255.0f;
		shader->setVec3("color", color);
		glBindTexture(GL_TEXTURE_2D, Knight::getKnightTexture(isWhite));
		shader->setVec2("offset", glm::vec2(0.0 - offsetX, 0.0));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		color = pieceColors[2] / 255.0f;
		shader->setVec3("color", color);
		glBindTexture(GL_TEXTURE_2D, Bishop::getBishopTexture(isWhite));
		shader->setVec2("offset", glm::vec2(0.0 + offsetX, 0.0));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		color = pieceColors[3] / 255.0f;
		shader->setVec3("color", color);
		glBindTexture(GL_TEXTURE_2D, Queen::getQueenTexture(isWhite));
		shader->setVec2("offset", glm::vec2(0.0 + offsetX * 3.0, 0.0));
		glDrawArrays(GL_TRIANGLES, 0, 6);

		processInput(window, isWhite);

		//Draw pieces as usual
		//--------------------
		shader = Graphics::getBoardShader();
		shader->use();
		shader->setFloat("scale", 0.8);
		glBindTexture(GL_TEXTURE_2D, Rook::getRookTexture(isWhite));
		shader->setVec2("offset", glm::vec2(0.0 - offsetX * 3.0, 0.0));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, Knight::getKnightTexture(isWhite));
		shader->setVec2("offset", glm::vec2(0.0 - offsetX, 0.0));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, Bishop::getBishopTexture(isWhite));
		shader->setVec2("offset", glm::vec2(0.0 + offsetX, 0.0));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, Queen::getQueenTexture(isWhite));
		shader->setVec2("offset", glm::vec2(0.0 + offsetX * 3.0, 0.0));
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	//Highlights the piece under mouse cursor and sets pawnPromotion value to that piece
	//----------------------------------------------------------------------------------
	void highlightPiece(int index, bool isWhite)
	{
		Shader* boardShader = Graphics::getBoardShader();
		Shader* piecesColorShader = Graphics::getPiecesColorShader();
		
		glm::vec2 offset;  //variable to offset the entire piece by
		float offsetX = scaleX / 4.0;
		//Convert index value to respective enum value
		promotionPiece = static_cast<PawnPromotionPiece>(index);

		//Highlight
		//---------
		switch (promotionPiece)
		{
		case PawnPromotionPiece::ROOK:
			glBindTexture(GL_TEXTURE_2D, Rook::getRookTexture(isWhite));
			offset = glm::vec2(0.0 - offsetX * 3.0, 0.0);
			break;
		case PawnPromotionPiece::KNIGHT:
			glBindTexture(GL_TEXTURE_2D, Knight::getKnightTexture(isWhite));
			offset = glm::vec2(0.0 - offsetX, 0.0);
			break;
		case PawnPromotionPiece::BISHOP:
			glBindTexture(GL_TEXTURE_2D, Bishop::getBishopTexture(isWhite));
			offset = glm::vec2(0.0 + offsetX, 0.0);
			break;
		case PawnPromotionPiece::QUEEN:
			glBindTexture(GL_TEXTURE_2D, Queen::getQueenTexture(isWhite));
			offset = glm::vec2(0.0 + offsetX * 3.0, 0.0);
			break;
		}
		
		piecesColorShader->use();
		piecesColorShader->setVec2("offset", offset);
		piecesColorShader->setFloat("scale", 0.9);
		piecesColorShader->setVec3("color", glm::vec3(0.0, 1.0, 0.0));
		glDrawArrays(GL_TRIANGLES, 0, 6);
		boardShader->use();
		boardShader->setFloat("scale", 0.8);
		boardShader->setVec2("offset", offset);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	
	//Gets the color value under mouse cursor and promotes pawn on mouse click
	//------------------------------------------------------------------------
	void processInput(GLFWwindow* window, bool isWhite)
	{
		promotionPiece = PawnPromotionPiece::NO_PIECE;
		glm::vec3 color = Mouse::getPixelColorUnderMouse(window);
		//Iterate through pieceColors vector to get the color and highlight that piece
		for (size_t i{ 0 }; i < pieceColors.size(); i++)
		{
			if (color == pieceColors[i])
			{
				highlightPiece(i, isWhite);
			}
		}
		if (Mouse::getMouseClicked())
		{
			//If mouse cursor was pointing to a specific piece, only then promote
			if (promotionPiece != PawnPromotionPiece::NO_PIECE)
			{
				pawnPromoted = true;
				scale = 0.0;
				transitionOver = false;
			}
		}
	}
		
public:
	//Constructor
	//-----------
	PawnPromotion() 
	{
		shader = new Shader("Files/Shaders/pawnPromotionVertexShader.vs", "Files/Shaders/pawnPromotionFragShader.fs");
		for (size_t i = 0; i < 4; i++)
		{
			pieceColors.push_back(Graphics::genRandomColor());
		}
		setupVAO();
	}

	//Draw rectangular box overlay
	//----------------------------
	void draw(GLFWwindow* window, float deltaTime, bool isWhite)
	{
		//Increment scale overtime upto a 1.0
		scale += deltaTime * transitionSpeed;
		if (scale >= 1.0)
		{
			transitionOver = true;
			scale = 1.0;
		}

		//Stenciling for rectangule box outlining
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		//Render rectangle box
		shader->use();
		glBindVertexArray(VAO);
		shader->setVec4("color", glm::vec4(1.0, 1.0, 1.0, 0.8));
		shader->setFloat("scale", scale);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Render outline
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		shader->setVec4("color", glm::vec4(0.0, 0.0, 0.0, 1.0));
		shader->setFloat("scale", scale * 1.02);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glStencilFunc(GL_ALWAYS, 1, 0xFF);

		//If transition is over render the pieces
		if (transitionOver)
		{
			drawPieces(window, isWhite);
		}
		glBindVertexArray(0);
	}

	//Checks if the target square the player has selected is a pawn promotion square
	//------------------------------------------------------------------------------
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
	
	//--------------------------------------------------------
	//Getter and setter functions for pawn promotion variables
	//--------------------------------------------------------

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

	//Destructor
	//----------
	~PawnPromotion()
	{
		delete shader;
	}
};

#endif
