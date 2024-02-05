#pragma once
#include<GL/glew.h>
#include<string>

struct FontParameters
{
	std::string m_fontsheetName;
    GLuint m_texture;
	int m_fontsheetWidth = 496;
	int m_fontsheetHeight = 216;
	int m_fontWidth = 31;
	int m_fontHeight = 36;
};