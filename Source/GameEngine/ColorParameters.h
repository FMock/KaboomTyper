#pragma once
#include<GL/glew.h>
#include<string>
#include <map>

namespace GameEngine
{
	struct ColorParameters
	{
		std::string redFileName; // #1. Red
		GLuint redTexture;

		std::string darkRedFileName; // #2. Dark Red
		GLuint darkRedTexture;

		std::string blueFileName; // #3. Blue
		GLuint blueTexture;

		std::string darkBlueFileName; // #4. Dark Blue
		GLuint darkBlueTexture;

		std::string greenFileName; // #5. Green
		GLuint greenTexture;

		std::string darkGreenFileName; // #6. Dark Green
		GLuint darkGreenTexture;

		std::string yellowFileName; // #7. Yellow
		GLuint yellowTexture;

		std::string darkYellowFileName; // #8. Dark Yellow
		GLuint darkYellowTexture;

		std::string purpleFileName; // #9. Purple
		GLuint purpleTexture;

		std::string darkPurpleFileName; // #10. Dark Purple
		GLuint darkPurpleTexture;

		std::string whiteFileName; // #11. White
		GLuint whiteTexture;

		std::string orangeFileName; // #12. Orange
		GLuint orangeTexture;

		std::string brownFileName; // #13. Brown
		GLuint brownTexture;

		std::string darkGrayFileName; // #14. Dark Gray
		GLuint darkGrayTexture;

		std::string blackFileName; // #15. Black
		GLuint blackTexture;

		int textureWidth;
		int textureHeight;

		std::map<std::string, GLuint> m_stringColorTextureColorMap;
	};
}
