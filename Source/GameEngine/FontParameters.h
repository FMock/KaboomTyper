#pragma once
#include<GL/glew.h>
#include<string>

namespace GameEngine
{
	struct FontParameters
	{
		std::string m_fontsheetName;
		GLuint m_texture;
		int m_fontsheetWidth = 384;
		int m_fontsheetHeight = 192;
		int m_fontWidth = 24;
		int m_fontHeight = 32;
		int m_padding = 0;
	};
}
