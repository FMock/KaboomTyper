#include "TextString.h"
#include<string>
#include<cmath>
#include "Utilities.h"
#include <stdexcept>
#include <cstring>

using namespace GameEngine;
using namespace DrawUtilities;
using namespace GameEngine::Utility;

// Definition of static members
TextStringFont TextString::s_font;
bool TextString::s_fontInitialized = false;

TextString::TextString() 
	: m_moveable(std::make_unique<Moveable>()),
	m_s1(0.0f), m_s2(0.0f), m_t1(0.0f), m_t2(0.0f), m_x(0), m_y(0), m_textSize(0)
{
}

GameEngine::TextString::TextString(std::string text, int x, int y) : TextString(text.c_str(), x, y)
{	
}

GameEngine::TextString::TextString(const char* text, int x, int y)
{
	m_string = text;
	m_textSize = std::strlen(text);

	// Initialize the static TextStringFont member if it hasn't been initialized yet
	if (!s_fontInitialized)
	{
		bool initialized = Utilities::ReadXmlFile("../../Config/FontParameters.xml", m_fontParameters);

		if (!initialized)
		{
			throw std::runtime_error("Failed to initialize FontParameters from XML file.");
		}

		s_font.image = m_fontParameters.m_texture;
		s_font.imageWidth = m_fontParameters.m_fontsheetWidth;
		s_font.imageHeight = m_fontParameters.m_fontsheetHeight;
		s_font.frameWidth = m_fontParameters.m_fontWidth;
		s_font.frameHeight = m_fontParameters.m_fontHeight;
		s_font.numberColumns = (s_font.imageWidth / s_font.frameWidth);
		s_font.numberRows = (s_font.imageHeight / s_font.frameHeight);

		s_fontInitialized = true;
	}

	m_x = x;
	m_y = y;
	m_s1 = 0.0f;
	m_s2 = 0.0f;
	m_t1 = 0.0f;
	m_t2 = 0.0f;
}

/// <summary>
/// Initializes the TextString members.
/// </summary>
/// <param name="string">C-String of text. This text gets drawn to the screen.</param>
/// <param name="x">The starting x postion to draw the text on screen</param>
/// <param name="y">The starting y position to draw the text on screen</param>
void TextString::Initialize(const char* str, int x, int y)
{
	m_string = str;
	m_textSize = std::strlen(str);

	// Initialize the static TextStringFont member if it hasn't been initialized yet
	if (!s_fontInitialized)
	{
		bool initialized = Utilities::ReadXmlFile("../../Config/FontParameters.xml", m_fontParameters);

		if (!initialized)
		{
			throw std::runtime_error("Failed to initialize FontParameters from XML file.");
		}

		s_font.image = m_fontParameters.m_texture;
		s_font.imageWidth = m_fontParameters.m_fontsheetWidth;
		s_font.imageHeight = m_fontParameters.m_fontsheetHeight;
		s_font.frameWidth = m_fontParameters.m_fontWidth;
		s_font.frameHeight = m_fontParameters.m_fontHeight;
		s_font.numberColumns = (s_font.imageWidth / s_font.frameWidth);
		s_font.numberRows = (s_font.imageHeight / s_font.frameHeight);

		s_fontInitialized = true;
	}

	m_x = x;
	m_y = y;
	m_s1 = 0.0f;
	m_s2 = 0.0f;
	m_t1 = 0.0f;
	m_t2 = 0.0f;
}

/// <summary>
/// Initializes the TextString members.
/// </summary>
/// <param name="string">std::string of text. This text gets drawn to the screen</param>
/// <param name="x">The starting x postion to draw the text on screen</param>
/// <param name="y">The starting y position to draw the text on screen</param>
void TextString::Initialize(std::string& string, int x, int y)
{
	Initialize(string.c_str(), x, y);
}

size_t GameEngine::TextString::GetTextSize()
{
	return m_textSize;
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
void TextString::DrawText()
{
	if (!s_fontInitialized)
	{
		throw std::runtime_error("Error: A TextString must be initialized before it can be used.");
	}

	float colDivision = 1.0f/s_font.numberColumns;
	float rowDivision = 1.0f/s_font.numberRows;
	int currentCol = 0;
	int currentRow = 0;

	for(int i = 0; i < strlen(m_string.c_str()); i++){

		int asciiValue = m_string[i]; // get ascii value of character

		currentCol = (asciiValue - 32) % 16;
		currentRow = abs(((asciiValue - (32 + currentCol)) / 16) - 5);

		// update s1, s2, t1, t2
		m_s1 = currentCol * colDivision;
		m_s2 = (currentCol * colDivision) + colDivision;
		m_t1 = currentRow * rowDivision;
		m_t2 = (currentRow * rowDivision) + rowDivision;

		GlDrawFrameParams params;

		params.tex = s_font.image;
		params.x = m_x + i * (s_font.frameWidth / 2);
		params.y = m_y;
		params.w = s_font.frameWidth;
		params.h = s_font.frameHeight;
		params.s1 = m_s1;
		params.s2 = m_s2;
		params.t1 = m_t1;
		params.t2 = m_t2;

		glDrawFrame(params);
	}
}

void TextString::Update(float deltaTime)
{

}
