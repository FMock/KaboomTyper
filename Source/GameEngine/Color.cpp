#include "Color.h"
#include "Utilities.h"
#include <stdexcept>

using namespace GameEngine;
using namespace GameEngine::Utility;

// Definition of static members
ColorParameters Color::s_colorParameters;
bool Color::s_initialized;

Color::Color()
{
	if (!s_initialized)
	{
		InitializeColorParameters();
	}
}

Color::~Color()
{
}

bool Color::InitializeColorParameters()
{
	bool initialized = Utilities::ReadXmlFile("../../Config/ColorParameters.xml", s_colorParameters); // TODO: REMOVE HARD-CODED PATH

	if (!initialized)
	{
		throw std::runtime_error("Failed to initialize ColorParameters from XML file.");
	}

	// Load all the textures into the String-to-Color Texture map
	s_colorParameters.m_stringColorTextureColorMap["red"] = s_colorParameters.redTexture;
	s_colorParameters.m_stringColorTextureColorMap["darkRed"] = s_colorParameters.darkRedTexture;
	s_colorParameters.m_stringColorTextureColorMap["blue"] = s_colorParameters.blueTexture;
	s_colorParameters.m_stringColorTextureColorMap["darkBlue"] = s_colorParameters.darkBlueTexture;
	s_colorParameters.m_stringColorTextureColorMap["green"] = s_colorParameters.greenTexture;
	s_colorParameters.m_stringColorTextureColorMap["darkGreen"] = s_colorParameters.darkGreenTexture;
	s_colorParameters.m_stringColorTextureColorMap["yellow"] = s_colorParameters.yellowTexture;
	s_colorParameters.m_stringColorTextureColorMap["darkYellow"] = s_colorParameters.darkYellowTexture;
	s_colorParameters.m_stringColorTextureColorMap["purple"] = s_colorParameters.purpleTexture;
	s_colorParameters.m_stringColorTextureColorMap["darkPurple"] = s_colorParameters.darkPurpleTexture;
	s_colorParameters.m_stringColorTextureColorMap["white"] = s_colorParameters.whiteTexture;
	s_colorParameters.m_stringColorTextureColorMap["orange"] = s_colorParameters.orangeTexture;
	s_colorParameters.m_stringColorTextureColorMap["brown"] = s_colorParameters.brownTexture;
	s_colorParameters.m_stringColorTextureColorMap["darkGray"] = s_colorParameters.darkGrayTexture;
	s_colorParameters.m_stringColorTextureColorMap["black"] = s_colorParameters.blackTexture;

	s_initialized = true;
}
