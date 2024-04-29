#include "TextString.h"
#include<string>
#include<cmath>
#include "Utilities.h"
#include <stdexcept>
#include <cstring>
#include <unordered_map>

using namespace GameEngine;
using namespace DrawUtilities;
using namespace GameEngine::Utility;

// Definition of static members
TextStringFont TextString::s_font;
bool TextString::s_fontInitialized = false;

// Implement the static method to initialize the font
void TextString::InitializeFont(FontParameters& fontParameters)
{
	if (!s_fontInitialized)
	{
		bool initialized = Utilities::ReadXmlFile("../../Config/FontParameters.xml", fontParameters);

		if (!initialized)
		{
			throw std::runtime_error("Failed to initialize FontParameters from XML file.");
		}

		s_font.image = fontParameters.m_texture;
		s_font.imageWidth = fontParameters.m_fontsheetWidth;
		s_font.imageHeight = fontParameters.m_fontsheetHeight;
		s_font.frameWidth = fontParameters.m_fontWidth;
		s_font.frameHeight = fontParameters.m_fontHeight;
		s_font.numberColumns = (s_font.imageWidth / s_font.frameWidth);
		s_font.numberRows = (s_font.imageHeight / s_font.frameHeight);
		s_font.padding = fontParameters.m_padding;
		s_font.colDivision = 1.0f / (s_font.imageWidth / s_font.frameWidth);
		s_font.rowDivision = 1.0f / (s_font.imageHeight / s_font.frameHeight);
		s_fontInitialized = true;
	}
}

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
		InitializeFont(m_fontParameters);
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
		InitializeFont(m_fontParameters);
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

size_t GameEngine::TextString::GetFontWidth()
{
	return m_fontParameters.m_fontWidth;
}

size_t GameEngine::TextString::GetFontHeight()
{
	return m_fontParameters.m_fontHeight;
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

	short currentCol = 0;
	short currentRow = 0;
	short previousAscii = -1;
	short strLen = strlen(m_string.c_str());

	for (short i = 0; i < strLen; i++)
	{
		GlDrawFrameParams params; // each character to draw needs a GlDrawFrameParams object

		short asciiValue = m_string[i]; // get ascii value of character

		if (previousAscii != asciiValue) // new ascii character so recalculate values.
		{
			currentCol = (asciiValue - 32) % 16;
			currentRow = abs(((asciiValue - (32 + currentCol)) / 16) - 5);

			// update s1, s2, t1, t2
			m_s1 = currentCol * s_font.colDivision;
			m_s2 = (currentCol * s_font.colDivision) + s_font.colDivision;
			m_t1 = currentRow * s_font.rowDivision;
			m_t2 = (currentRow * s_font.rowDivision) + s_font.rowDivision;
		}

		params.tex = s_font.image;

		if (s_font.padding < 1)
		{
			params.x = m_x + i * (s_font.frameWidth);
		}
		else
		{
			params.x = m_x + i * (s_font.padding * 2);
		}

		params.y = m_y;
		params.w = s_font.frameWidth;
		params.h = s_font.frameHeight;
		params.s1 = m_s1;
		params.s2 = m_s2;
		params.t1 = m_t1;
		params.t2 = m_t2;

		glDrawFrame(params);

		previousAscii = asciiValue; // save copy
	}
}

void TextString::Update(float deltaTime)
{

}

std::string GameEngine::TextString::GetText() const
{
	return m_string;
}
