#include "TextString.h"
#include<string>
#include<cmath>

using namespace DrawUtilities;

/// <summary>
/// Initializes the TextString members.
/// </summary>
/// <param name="string">C-String of text</param>
/// <param name="x">The starting x postion to draw the text on screen</param>
/// <param name="y">The starting y position to draw the text on screen</param>
/// <param name="params">A reference to a TextStringFont</param>
void TextString::Initialize(const char* string, int x, int y, TextStringFont& params)
{
	m_string = string;
	m_image = params.image;
	m_width = params.imageWidth;
	m_height = params.imageHeight;
	m_frameWidth = params.frameWidth;
	m_frameHeight = params.frameHeight;
	m_numColumns = (params.imageWidth / params.frameWidth);
	m_numRows = (params.imageHeight / params.frameHeight);
	m_x = x;
	m_y = y;
	m_changeX = 0;
	m_changeY = 0;
	m_speedX = 140; // TODO GET RID OF THIS MAGIC NUMBER (LOAD FROM CONFIG FILE?)
	m_speedY = 140; // TODO GET RID OF THIS MAGIC NUMBER 
}

/*Draws each character of this objects string
 *Assumes the following raster font image which is 16 characters wide X 6 characters high
 character in the top left corner is a space, ascii character = 32

   !"#$%&'()*+,-./
  0123456789:;<=>?
  @ABCDEFGHIJKLMNO
  PQRSTUVWXYZ[\]^_
  `abcdefghijklmno
  qrstuvwxyz{|}~
 * */
void TextString::DrawText(){

	float colDivision = 1.0f/m_numColumns;
	float rowDivision = 1.0f/m_numRows;
	int currentCol = 0;
	int currentRow = 0;

	for(int i = 0; i < strlen(m_string); i++){

		int asciiValue = m_string[i]; // get ascii value of character

		currentCol = (asciiValue - 32) % 16;
		currentRow = abs(((asciiValue - (32 + currentCol)) / 16) - 5);

		// update s1, s2, t1, t2
		m_s1 = currentCol * colDivision;
		m_s2 = (currentCol * colDivision) + colDivision;
		m_t1 = currentRow * rowDivision;
		m_t2 = (currentRow * rowDivision) + rowDivision;

		GlDrawFrameParams params;

		params.tex = m_image;
		params.x = m_x + i * (m_frameWidth / 2);
		params.y = m_y;
		params.w = m_frameWidth;
		params.h = m_frameHeight;
		params.s1 = m_s1;
		params.s2 = m_s2;
		params.t1 = m_t1;
		params.t2 = m_t2;

		glDrawFrame(params);
	}
}

void TextString::Update(float deltaTime)
{
	m_x += m_changeX * deltaTime;
	m_y += m_changeY * deltaTime;
}

void TextString::MoveRight()
{
	m_changeX += m_speedX;
	m_changeY = 0;
}

void TextString::MoveLeft()
{
	m_changeX += -m_speedX;
	m_changeY = 0;
}

void TextString::MoveUp()
{
	m_changeX = 0;
	m_changeY-= m_speedY;
}

void TextString::MoveDown()
{
	m_changeX = 0;
	m_changeY += m_speedY;
}

void TextString::Stop()
{
	m_changeX = 0;
	m_changeY = 0;
}
