#include "TextString.h"
#include <string>
#include <cmath>
#include "Utilities.h"
#include "Common.h"
#include <stdexcept>
#include <cstring>
#include <unordered_map>

using namespace GameEngine;
using namespace DrawUtilities;
using namespace GameEngine::Utility;

// Definition of static members
TextStringFont TextString::s_font;
bool TextString::s_fontInitialized = false;
std::unordered_map<short, TextString::CharMetrics> TextString::s_charMetricsCache;

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
	m_x(0), m_y(0), m_textSize(0), m_velocity(0.0f)
{
}

GameEngine::TextString::TextString(std::string text, int x, int y) : TextString(text.c_str(), x, y)
{
	m_string = text;
	m_textSize = std::strlen(text.c_str());

	// Initialize the static TextStringFont member if it hasn't been initialized yet
	if (!s_fontInitialized)
	{
		InitializeFont(m_fontParameters);
	}

	m_x = x;
	m_y = y;
	m_velocity = 0.0f;
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
	m_velocity = 0.0f;
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
	m_velocity = 0.0f;
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

/// <summary>
/// Draws a scaled version of texture if scaleFactor > 1.0 or < 1.0
/// 
///  Draws each character of this object's string m_string
///  Assumes the following raster font image which is 16 characters wide X 6 characters high
//   character in the top left corner is a space, ascii character = 32
//
//    !"#$%&'()*+,-./
//   0123456789:;<=>?
//   @ABCDEFGHIJKLMNO
//   PQRSTUVWXYZ[\]^_
//   `abcdefghijklmno
//   qrstuvwxyz{|}~
/// 
/// </summary>
/// <param name="scaleFactor">float that determines how much to scale the texture</param>
void GameEngine::TextString::DrawText(float scaleFactor)
{
	if (!s_fontInitialized)
	{
		throw std::runtime_error("Error: A TextString must be initialized before it can be used.");
	}

	const char* str = m_string.c_str();
	short strLen = strlen(str);

	// Precompute font division values
	float colDivision = s_font.colDivision;
	float rowDivision = s_font.rowDivision;
	float frameWidth = s_font.frameWidth;
	float frameHeight = s_font.frameHeight;
	auto fontImage = s_font.image;

	GlDrawFrameParams params; // create once and reuse
	params.scale = scaleFactor;
	params.tex = fontImage;
	params.y = m_y;
	params.w = frameWidth;
	params.h = frameHeight;

	for (short i = 0; i < strLen; i++)
	{
		short asciiValue = str[i]; // get ascii value of character

		// Check if values for this asciiValue are already cached
		auto it = s_charMetricsCache.find(asciiValue);
		if (it == s_charMetricsCache.end())
		{
			// Calculate values and store in cache
			short currentCol = (asciiValue - 32) % 16;
			short currentRow = abs(((asciiValue - (32 + currentCol)) / 16) - 5);

			CharMetrics metrics;
			metrics.s1 = currentCol * colDivision;
			metrics.s2 = metrics.s1 + colDivision;
			metrics.t1 = currentRow * rowDivision;
			metrics.t2 = metrics.t1 + rowDivision;
			metrics.currentCol = currentCol;
			metrics.currentRow = currentRow;

			s_charMetricsCache[asciiValue] = metrics;

			// Use computed values for params
			params.s1 = metrics.s1;
			params.s2 = metrics.s2;
			params.t1 = metrics.t1;
			params.t2 = metrics.t2;
		}
		else
		{
			// Use cached values for params
			params.s1 = it->second.s1;
			params.s2 = it->second.s2;
			params.t1 = it->second.t1;
			params.t2 = it->second.t2;
		}

		params.x = m_x + i * (frameWidth)*scaleFactor;  // Scales spacing between characters 

		glDrawFrameScaled(params); // Has ability to draw a scaled version of texture
	}
}

// Update the position with the current velocity
void TextString::Update(float deltaTime)
{
	//m_velocity += Common::GRAVITY * deltaTime; // Update the velocity with gravity
	m_y += m_velocity * deltaTime; // TextBlock calculated the velocity
}

// Setter for m_velocity
void TextString::SetVelocity(float velocity)
{
	m_velocity = velocity;
}

// Getter for m_velocity
float TextString::GetVelocity() const
{
	return m_velocity;
}

std::string GameEngine::TextString::GetText() const
{
	return m_string;
}
