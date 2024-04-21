#pragma once
#include<GL/glew.h>
#include<string>
#include <map>

namespace GameEngine
{
	struct TextBlockParameters
	{
		std::string redBlockFileName;
		GLuint redBlockTexture;
		std::string darkRedBlockFileName;
		GLuint darkRedBlockTexture;
		std::string blueBlockFileName;
		GLuint blueBlockTexture;
		std::string darkBlueBlockFileName;
		GLuint darkBlueBlockTexture;
		std::string greenBlockFileName;
		GLuint greenBlockTexture;
		std::string darkGreenBlockFileName;
		GLuint darkGreenBlockTexture;
		std::string yellowBlockFileName;
		GLuint yellowBlockTexture;
		std::string darkYellowBlockFileName;
		GLuint darkYellowBlockTexture;
		std::string purpleBlockFileName;
		GLuint purpleBlockTexture;
		std::string darkPurpleBlockFileName;
		GLuint darkPurpleBlockTexture;
		std::string whiteBlockFileName;
		GLuint whiteBlockTexture;
		std::string orangeBlockFileName;
		GLuint orangeBlockTexture;
		std::string brownBlockFileName;
		GLuint brownBlockTexture;
		std::string darkGrayBlockFileName;
		GLuint darkGrayBlockTexture;
		std::string blackBlockFileName;
		GLuint blackBlockTexture;
		int blockWidth;
		int blockHeight;

		std::map<std::string, GLuint> m_stringColorTextureColorMap;

		// TODO RETHINK THIS. THESE SHOULD BE ASSIGNED FROM A CONFIG FILE
		static const int defaultBlockWidth = 1; // 1 pixel wide
		static const int defaultBlockHeight = 1; // 1 pixel high
		inline static int GetWidth() { return defaultBlockHeight; }
		inline static int GetHeight() { return defaultBlockHeight; }
	};
}
