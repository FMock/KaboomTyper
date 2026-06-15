#pragma once

#include<GL/glew.h>
#include<string>
#include<vector>
#include "RGBColor.h"
#include "Color.h"
#include "Common.h"
#include <cmath>


struct GlDrawFrameParams
{
	GLuint tex;
	short x;
	short y;
	short w;
	short h;
	float s1;
	float s2;
	float t1;
	float t2;
	float scale;
	// Modulation color for the frame. Defaults to white (no tint) so existing
	// callers are unaffected; the font atlas is white-on-transparent, so setting
	// a non-white color tints the drawn glyph/frame.
	RGBColor color{ 255, 255, 255 };
};

namespace DrawUtilities
{
	GLuint glTexImageTGAFile(const char* filename);
	GLuint glTexImageTGAFile(const char* filename, int* outWidth, int* outHeight);

	/* Uses default values for s and t texture coordinates since it assumes
		* the full texture will be drawn*/
	void glDrawSprite(GLuint tex, int x, int y, int w, int h);

	/// <summary>
	/// Draw scaled sprite
	/// </summary>
	/// <param name="tex"></param>
	/// <param name="x"></param>
	/// <param name="y"></param>
	/// <param name="w"></param>
	/// <param name="h"></param>
	/// <param name="scaleX"></param>
	/// <param name="scaleY"></param>
	void glDrawSpriteScaled(GLuint tex, int x, int y, int w, int h, float scaleX, float scaleY);
	void glDrawSpriteWithGlow(GLuint tex, int x, int y, int w, int h, float scaleX, float scaleY);
	void glDrawSpriteWithSoftGlow(GLuint tex, int x, int y, int w, int h, float scaleX, float scaleY);
	void glDrawSpriteScaledRotated(GLuint tex, int x, int y, int w, int h, float scaleX, float scaleY, float angle);
	void glDrawSpriteScaled(GLuint tex, int x, int y, int w, int h, float scaleX, float scaleY, GLuint xScaled, GLuint yScaled);
	void glDrawTexture(GLuint tex, int x, int y, float width, float height); // assumes the texture w = 1 and h = 1
	void glDrawRectangleOutline(GLuint tex, int x, int y, int w, int h, float scaleX, float scaleY);
	void glDrawRectangleOutline(int x, int y, int w, int h, const RGBColor& outlineColor);
	// Draws a rectangle border of the given pixel thickness as four filled bars (inset within
	// the rect). Reliable across drivers, unlike thick GL_LINES.
	void glDrawThickRectangleOutline(int x, int y, int w, int h, const RGBColor& color, int thickness);
	void glDrawRectangleOutline(GLuint tex, int x, int y, float width, float height); // assumes a texture w and h of 1
	void glDrawRectangleOutlineFilled(GLuint tex, int x, int y, int w, int h, float scaleX, float scaleY, const RGBColor& fillColor);
	void glDrawFilledRectangle(int x, int y, int w, int h, const RGBColor& fillColor);
	void glDrawRectangleOutline(int x, int y, int w, int h, float scaleX, float scaleY, const RGBColor& color);
	void glDrawFilledRectangle(int x, int y, int w, int h, float scaleX, float scaleY, const RGBColor& color);
	void glDrawFilledRectangle(int x, int y, int w, int h, float scaleX, float scaleY, const RGBColor& color, unsigned char opacity);
	void glDrawFilledTriangle(int x, int y, int size, float scaleX, float scaleY, const RGBColor& color, float rotation = 0.0f);
	void glDrawAudio(GLuint tex, int x, int y, int w, int h, std::vector<float> audio);

	/// <summary>
	/// Draws a single frame of a GL texture
	/// </summary>
	/// <param name="params">GlDrawFrameParams object containing specifics of the frame to draw:
	/// <paramref name="tex">GLuint tex is an openGL texture mapped to a quad</paramref>
	/// <paramref name="x">int x position to draw sprite in the game world</paramref>
	/// <paramref name="y">int y position to draw sprite in the game world</paramref>
	/// <paramref name="w">int w is frame width - width of part of texture to be drawn</paramref>
	/// <paramref name="h">is frame height - height of part of texture to be drawn</paramref>
	/// <paramref name="s1">float s1 is left texture coordinate</paramref>
	/// <paramref name="s2">float s2 is right texture coordinate</paramref>
	/// <paramref name="t1">float t1 is lower texture coordinate</paramref>
	/// <paramref name="t2">float t2 is upper texture coordinate</paramref>
	///
	/// (s1,t2)******(s2,t2)
	///    *            *
	///    *            *
	///    *            *
	///    *            *
	/// (s1,t1)******(s2,t1)
	/// 
	/// </param>
	void glDrawFrame(GlDrawFrameParams params);
	void glDrawFrameScaled(GlDrawFrameParams params);
	void glDrawFrameScaled(GlDrawFrameParams params, float angle);

	/*Draws each character image of string*/
	void drawRasterText(GLuint tex, int x, int y, int w, int h, char string[]);

	void glDrawSpriteRotate(GLuint tex, int x, int y, int w, int h, GLfloat angle);

	void DrawRectangle(int x, int y, int w, int h, const RGBColor& fillColor);
}