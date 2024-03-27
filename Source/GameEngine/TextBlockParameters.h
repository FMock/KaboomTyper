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
		std::string blueBlockFileName;
		GLuint blueBlockTexture;
		std::string greenBlockFileName;
		GLuint greenBlockTexture;
		std::string yellowBlockFileName;
		GLuint yellowBlockTexture;
		std::string purpleBlockFileName;
		GLuint purpleBlockTexture;
		std::string whiteBlockFileName;
		GLuint whiteBlockTexture;
		std::string orangeBlockFileName;
		GLuint orangeBlockTexture;
		int blockWidth;
		int blockHeight;

		std::map<std::string, GLuint> m_stringColorTextureColorMap;

		// TODO RETHINK THIS. THESE SHOULD BE ASSIGNED FROM A CONFIG FILE
		static const int defaultBlockWidth = 30;
		static const int defaultBlockHeight = 30;
		inline static int GetWidth() { return defaultBlockHeight; }
		inline static int GetHeight() { return defaultBlockHeight; }
	};
}
