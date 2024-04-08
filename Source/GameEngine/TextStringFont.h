#pragma once
#include<GL/glew.h>

/// <summary>
/// TextStringFont.h
/// Holds all the data pertaining to a raster image font.
/// A TextString will use this information to draw text to the screen.
/// 
/// A font image is an image file that contains a series of characters 
/// aligned in rows and columns similar to the following.
/// 
///    !"#$%&'()*+,-./
///   0123456789:;<=>?
///   @ABCDEFGHIJKLMNO
///   PQRSTUVWXYZ[\]^_
///   `abcdefghijklmno
///   qrstuvwxyz{|}~
/// 
/// </summary>

namespace GameEngine
{
	struct TextStringFont
	{
		GLuint image;    // the loaded font image
		int imageWidth;  // width of font image file
		int imageHeight; // height of the font image file
		int frameWidth;  // width of a single character of the image file
		int frameHeight; // height of a single character of the image file
		int numberColumns; // (imageWidth / frameWidth);
		int numberRows; // (imageHeight / frameHeight);
		int padding; // space between characters
		inline int GetFontWidth() const { return frameWidth; }
		inline int GetFontHeight() const { return frameHeight; }
	};
}
