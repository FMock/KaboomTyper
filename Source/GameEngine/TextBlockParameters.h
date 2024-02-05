#pragma once
#include<GL/glew.h>
#include<string>

struct TextBlockParameters
{
	std::string m_redBlockFileName;
	GLuint m_redBlockTexture;
	std::string m_blueBlockFileName;
	GLuint m_blueBlockTexture;
	std::string m_greenBlockFileName;
	GLuint m_greenBlockTexture;
	std::string m_yellowBlockFileName;
	GLuint m_yellowBlockTexture;
	std::string m_purpleBlockFileName;
	GLuint m_purpleBlockTexture;
	std::string m_whiteBlockFileName;
	GLuint m_whiteBlockTexture;
	std::string m_orangeBlockFileName;
	GLuint m_orangeBlockTexture;
	int m_blockWidth = 30;
	int m_blockHeight = 30;
};