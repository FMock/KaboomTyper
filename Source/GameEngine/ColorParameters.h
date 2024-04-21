#pragma once
#include<GL/glew.h>
#include<string>
#include <map>

namespace GameEngine
{
	struct ColorParameters
	{
		std::string redFileName;
		GLuint redTexture;
		std::string darkRedFileName;
		GLuint darkRedTexture;
		std::string blueFileName;
		GLuint blueTexture;
		std::string darkBlueFileName;
		GLuint darkBlueTexture;
		std::string greenFileName;
		GLuint greenTexture;
		std::string darkGreenFileName;
		GLuint darkGreenTexture;
		std::string yellowFileName;
		GLuint yellowTexture;
		std::string darkYellowFileName;
		GLuint darkYellowTexture;
		std::string purpleFileName;
		GLuint purpleTexture;
		std::string darkPurpleFileName;
		GLuint darkPurpleTexture;
		std::string whiteFileName;
		GLuint whiteTexture;
		std::string orangeFileName;
		GLuint orangeTexture;
		std::string brownFileName;
		GLuint brownTexture;
		std::string darkGrayFileName;
		GLuint darkGrayTexture;
		std::string blackFileName;
		GLuint blackTexture;

		int textureWidth;
		int textureHeight;

		std::map<std::string, GLuint> m_stringColorTextureColorMap;

	};
}
