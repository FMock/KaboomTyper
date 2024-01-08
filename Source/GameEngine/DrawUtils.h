#pragma once

#include<GL/glew.h>
#include<string>

struct GlDrawFrameParams
{
	GLuint tex;
	int x;
	int y;
	int w;
	int h;
	float s1;
	float s2;
	float t1;
	float t2;
};

namespace DrawUtilities
{
	GLuint glTexImageTGAFile(const char* filename);

	/* Uses default values for s and t texture coordinates since it assumes
		* the full texture will be drawn*/
	void glDrawSprite(GLuint tex, int x, int y, int w, int h);

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

	/*Draws each character image of string*/
	void drawRasterText(GLuint tex, int x, int y, int w, int h, char string[]);

	void glDrawSpriteRotate(GLuint tex, int x, int y, int w, int h, GLfloat angle);

}