#pragma once
#include<GL/glew.h>
#include<string>

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
	int blockWidth = 30;
	int blockHeight = 30;
};