/***********************************************************************
Utilities for loading and drawing sprites.
*/
#include"DrawUtils.h"
#include<GL/glew.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

using namespace DrawUtilities;

/* Load a file into an OpenGL texture, and return that texture. */
GLuint DrawUtilities::glTexImageTGAFile(const char* filename)
{
	const int BPP = 4;

	/* open the file */
	FILE* file = fopen(filename, "rb");
	if (file == NULL)
	{
		fprintf(stderr, "File: %s -- Could not open for reading.\n", filename);
		return 0;
	}

	/* skip first two bytes of data we don't need */
	fseek(file, 2, SEEK_CUR);

	/* read in the image type.  For our purposes the image type should
	* be either a 2 or a 3. */
	unsigned char imageTypeCode;
	fread(&imageTypeCode, 1, 1, file);
	if (imageTypeCode != 2 && imageTypeCode != 3)
	{
		fclose(file);
		fprintf(stderr, "File: %s -- Unsupported TGA type: %d\n", filename, imageTypeCode);
		return 0;
	}

	/* skip 9 bytes of data we don't need */
	fseek(file, 9, SEEK_CUR);

	/* read image dimensions */
	int imageWidth = 0;
	int imageHeight = 0;
	int bitCount = 0;
	fread(&imageWidth, sizeof(short), 1, file);
	fread(&imageHeight, sizeof(short), 1, file);
	fread(&bitCount, sizeof(unsigned char), 1, file);
	fseek(file, 1, SEEK_CUR);

	/* allocate memory for image data and read it in */
	unsigned char* bytes = (unsigned char*)calloc(imageWidth * imageHeight * BPP, 1);

	/* read in data */
	if (bitCount == 32)
	{
		int it;
		for (it = 0; it != imageWidth * imageHeight; ++it)
		{
			bytes[it * BPP + 0] = fgetc(file);
			bytes[it * BPP + 1] = fgetc(file);
			bytes[it * BPP + 2] = fgetc(file);
			bytes[it * BPP + 3] = fgetc(file);
		}
	}
	else
	{
		int it;
		for (it = 0; it != imageWidth * imageHeight; ++it)
		{
			bytes[it * BPP + 0] = fgetc(file);
			bytes[it * BPP + 1] = fgetc(file);
			bytes[it * BPP + 2] = fgetc(file);
			bytes[it * BPP + 3] = 255;
		}
	}

	fclose(file);

	/* load into OpenGL */
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, bytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	free(bytes);

	return tex;
}

/* Load a file into an OpenGL texture, and return that texture. */
GLuint DrawUtilities::glTexImageTGAFile(const char* filename, int* outWidth, int* outHeight)
{
	const int BPP = 4;

	/* open the file */
	FILE* file = fopen(filename, "rb");
	if (file == NULL)
	{
		fprintf(stderr, "File: %s -- Could not open for reading.\n", filename);
		return 0;
	}

	/* skip first two bytes of data we don't need */
	fseek(file, 2, SEEK_CUR);

	/* read in the image type.  For our purposes the image type should
	* be either a 2 or a 3. */
	unsigned char imageTypeCode;
	fread(&imageTypeCode, 1, 1, file);
	if (imageTypeCode != 2 && imageTypeCode != 3)
	{
		fclose(file);
		fprintf(stderr, "File: %s -- Unsupported TGA type: %d\n", filename, imageTypeCode);
		return 0;
	}

	/* skip 9 bytes of data we don't need */
	fseek(file, 9, SEEK_CUR);

	/* read image dimensions */
	int imageWidth = 0;
	int imageHeight = 0;
	int bitCount = 0;
	fread(&imageWidth, sizeof(short), 1, file);
	fread(&imageHeight, sizeof(short), 1, file);
	fread(&bitCount, sizeof(unsigned char), 1, file);
	fseek(file, 1, SEEK_CUR);

	/* allocate memory for image data and read it in */
	unsigned char* bytes = (unsigned char*)calloc(imageWidth * imageHeight * BPP, 1);

	/* read in data */
	if (bitCount == 32)
	{
		int it;
		for (it = 0; it != imageWidth * imageHeight; ++it)
		{
			bytes[it * BPP + 0] = fgetc(file);
			bytes[it * BPP + 1] = fgetc(file);
			bytes[it * BPP + 2] = fgetc(file);
			bytes[it * BPP + 3] = fgetc(file);
		}
	}
	else
	{
		int it;
		for (it = 0; it != imageWidth * imageHeight; ++it)
		{
			bytes[it * BPP + 0] = fgetc(file);
			bytes[it * BPP + 1] = fgetc(file);
			bytes[it * BPP + 2] = fgetc(file);
			bytes[it * BPP + 3] = 255;
		}
	}

	fclose(file);

	/* load into OpenGL */
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
		GL_BGRA, GL_UNSIGNED_BYTE, bytes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	free(bytes);

	if (outWidth)
	{
		*outWidth = imageWidth;
	}
	if (outHeight)
	{
		*outHeight = imageHeight;
	}
	return tex;
}

/* Draw the sprite using default values for s and t texture coordinates
*  Assumes the full texture will be drawn
 * The lower left of a texture is (0.0, 0.0) and upper right is (1.0, 1.0)
 * Horizontal axis is s, and vertical axis is t
 * param tex is an openGL texture mapped to a quad
 * param x is position to draw sprite in the game world
 * param y is position to draw sprite in the game world
 * param w is frame width - width of part of texture to be drawn
 * param h is frame height - height of part of texture to be drawn*/
void DrawUtilities::glDrawSprite(GLuint tex, int x, int y, int w, int h)
{
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	{
		glColor3ub(255, 255, 255);

		glTexCoord2f(0.0f, 1.0f); // top left
		glVertex2i(x, y);

		glTexCoord2f(1.0f, 1.0f); // top right
		glVertex2i(x + w, y);

		glTexCoord2f(1.0f, 0.0f); // bottom right
		glVertex2i(x + w, y + h);

		glTexCoord2f(0.0f, 0.0f); // bottom left
		glVertex2i(x, y + h);
	}
	glEnd(); // sends all submitted data to the GPU for rendering
}

/* Draw the sprite with scaling
 * Assumes the full texture will be drawn
 * The lower left of a texture is (0.0, 0.0) and upper right is (1.0, 1.0)
 * Horizontal axis is s, and vertical axis is t
 * param tex is an openGL texture mapped to a quad
 * param x is position to draw sprite in the game world
 * param y is position to draw sprite in the game world
 * param w is frame width - width of part of texture to be drawn
 * param h is frame height - height of part of texture to be drawn
 * param scaleX is the scaling factor along the x-axis
 * param scaleY is the scaling factor along the y-axis */
void DrawUtilities::glDrawSpriteScaled(GLuint tex, int x, int y, int w, int h, float scaleX, float scaleY)
{
	GLuint wScaled = static_cast<GLint>(w * scaleX);
	GLuint hScaled = static_cast<GLint>(h * scaleY);

	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	{
		glColor3ub(255, 255, 255);

		glTexCoord2f(0.0f, 1.0f); // top left
		glVertex2i(x, y);

		glTexCoord2f(scaleX, 1.0f); // top right
		glVertex2i(x + wScaled, y);

		glTexCoord2f(scaleX, 0.0f); // bottom right
		glVertex2i(x + wScaled, y + hScaled);

		glTexCoord2f(0.0f, 0.0f); // bottom left
		glVertex2i(x, y + hScaled);
	}
	glEnd(); // sends all submitted data to the GPU for rendering
}

void DrawUtilities::glDrawSpriteWithGlow(GLuint tex, int x, int y, int w, int h, float scaleX, float scaleY)
{
	GLuint wScaled = static_cast<GLint>(w * scaleX);
	GLuint hScaled = static_cast<GLint>(h * scaleY);

	// Save current state
	GLboolean blendEnabled = glIsEnabled(GL_BLEND);
	GLint srcBlendFunc, dstBlendFunc;
	glGetIntegerv(GL_BLEND_SRC, &srcBlendFunc);
	glGetIntegerv(GL_BLEND_DST, &dstBlendFunc);

	glPushAttrib(GL_CURRENT_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE); // Additive blending

	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	{
		glColor4ub(255, 255, 255, 128); // Semi-transparent white

		glTexCoord2f(0.0f, 1.0f); // top left
		glVertex2i(x, y);

		glTexCoord2f(scaleX, 1.0f); // top right
		glVertex2i(x + wScaled, y);

		glTexCoord2f(scaleX, 0.0f); // bottom right
		glVertex2i(x + wScaled, y + hScaled);

		glTexCoord2f(0.0f, 0.0f); // bottom left
		glVertex2i(x, y + hScaled);
	}
	glEnd();

	// Restore previous state
	glBlendFunc(srcBlendFunc, dstBlendFunc);
	if (!blendEnabled)
	{
		glDisable(GL_BLEND);
	}

	glPopAttrib();

	// Reset color to white with full opacity
	glColor4ub(255, 255, 255, 255);
}

//  Renders the texture multiple times with varying levels of transparency and slightly different sizes or positions.
void DrawUtilities::glDrawSpriteWithSoftGlow(GLuint tex, int x, int y, int w, int h, float scaleX, float scaleY)
{
	GLuint wScaled = static_cast<GLint>(w * scaleX);
	GLuint hScaled = static_cast<GLint>(h * scaleY);

	// Save current state
	GLboolean blendEnabled = glIsEnabled(GL_BLEND);
	GLint srcBlendFunc, dstBlendFunc;
	glGetIntegerv(GL_BLEND_SRC, &srcBlendFunc);
	glGetIntegerv(GL_BLEND_DST, &dstBlendFunc);

	glPushAttrib(GL_CURRENT_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Standard alpha blending

	for (int i = 0; i < 3; ++i)
	{
		float alpha = 1.0f / (i + 1);
		int offset = i * 2;

		glBindTexture(GL_TEXTURE_2D, tex);
		glBegin(GL_QUADS);
		{
			glColor4f(1.0f, 1.0f, 1.0f, alpha); // Varying transparency

			glTexCoord2f(0.0f, 1.0f); // top left
			glVertex2i(x - offset, y - offset);

			glTexCoord2f(scaleX, 1.0f); // top right
			glVertex2i(x + wScaled + offset, y - offset);

			glTexCoord2f(scaleX, 0.0f); // bottom right
			glVertex2i(x + wScaled + offset, y + hScaled + offset);

			glTexCoord2f(0.0f, 0.0f); // bottom left
			glVertex2i(x - offset, y + hScaled + offset);
		}
		glEnd();
	}

	// Restore previous state
	glBlendFunc(srcBlendFunc, dstBlendFunc);
	if (!blendEnabled)
	{
		glDisable(GL_BLEND);
	}

	glPopAttrib();

	// Reset color to white with full opacity
	glColor4ub(255, 255, 255, 255);
}



// Assumes the tex w = 1, and h = 1
void DrawUtilities::glDrawTexture(GLuint tex, int x, int y, float width, float height)
{
	int w = 1;
	int h = 1;

	GLuint adjustedWidth = static_cast<GLuint>(w * width);
	GLuint adjustedHeight = static_cast<GLuint>(h * height);

	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	{
		glColor3ub(255, 255, 255);

		glTexCoord2f(0.0f, 1.0f); // top left
		glVertex2i(x, y);

		glTexCoord2f(width, 1.0f); // top right
		glVertex2i(x + adjustedWidth, y);

		glTexCoord2f(width, 0.0f); // bottom right
		glVertex2i(x + adjustedWidth, y + adjustedHeight);

		glTexCoord2f(0.0f, 0.0f); // bottom left
		glVertex2i(x, y + adjustedHeight);
	}
	glEnd(); // sends all submitted data to the GPU for rendering
}

// Function to draw a sprite at an angle
void DrawUtilities::glDrawSpriteScaledRotated(GLuint tex, int x, int y, int w, int h, float scaleX, float scaleY, float angle)
{
	// Convert the angle to radians
	float radians = angle * (3.1415926535897932f / 180.0f);

	// Calculate the half-width and half-height for scaling
	float halfWidth = w * scaleX * 0.5f;
	float halfHeight = h * scaleY * 0.5f;

	// Calculate the center of the sprite
	float centerX = x + halfWidth;
	float centerY = y + halfHeight;

	// Calculate the four corners of the sprite
	float corners[4][2] = {
		{-halfWidth, -halfHeight}, // bottom left
		{halfWidth, -halfHeight},  // bottom right
		{halfWidth, halfHeight},   // top right
		{-halfWidth, halfHeight}   // top left
	};

	// Rotate each corner around the center
	for (int i = 0; i < 4; ++i)
	{
		float rotatedX = corners[i][0] * cos(radians) - corners[i][1] * sin(radians);
		float rotatedY = corners[i][0] * sin(radians) + corners[i][1] * cos(radians);
		corners[i][0] = rotatedX + centerX;
		corners[i][1] = rotatedY + centerY;
	}

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	{
		glColor3ub(255, 255, 255);

		// Draw the sprite with texture coordinates
		glTexCoord2f(0.0f, 1.0f); // top left
		glVertex2f(corners[3][0], corners[3][1]);

		glTexCoord2f(scaleX, 1.0f); // top right
		glVertex2f(corners[2][0], corners[2][1]);

		glTexCoord2f(scaleX, 0.0f); // bottom right
		glVertex2f(corners[1][0], corners[1][1]);

		glTexCoord2f(0.0f, 0.0f); // bottom left
		glVertex2f(corners[0][0], corners[0][1]);
	}
	glEnd(); // sends all submitted data to the GPU for rendering
}


// TODO: This overload avoids the math in the glVertex2i calls. Use with TextBlock to optimize efficiency.
//       Figure out correct calculations for xScaled and yScaled for TextBlock to use this overload
void DrawUtilities::glDrawSpriteScaled(GLuint tex, int x, int y, int w, int h, float scaleX, float scaleY, GLuint xScaled, GLuint yScaled)
{
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_QUADS);
	{
		glColor3ub(255, 255, 255);

		glTexCoord2f(0.0f, 1.0f); // top left
		glVertex2i(x, y);

		glTexCoord2f(scaleX, 1.0f); // top right
		glVertex2i(xScaled, y);

		glTexCoord2f(scaleX, 0.0f); // bottom right
		glVertex2i(xScaled, yScaled);

		glTexCoord2f(0.0f, 0.0f); // bottom left
		glVertex2i(x, yScaled);
	}
	glEnd(); // sends all submitted data to the GPU for rendering
}

void DrawUtilities::glDrawRectangleOutline(GLuint tex, int x, int y, int w, int h, float scaleX, float scaleY)
{
	GLuint wScaled = static_cast<GLuint>(w * scaleX);
	GLuint hScaled = static_cast<GLuint>(h * scaleY);

	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_LINES);
	{
		glColor3ub(255, 255, 255);

		// Top line
		glTexCoord2f(0.0f, 1.0f); // top left
		glVertex2i(x, y);
		glTexCoord2f(scaleX, 1.0f); // top right
		glVertex2i(x + wScaled, y);

		// Right line
		glTexCoord2f(scaleX, 1.0f); // top right
		glVertex2i(x + wScaled, y);
		glTexCoord2f(scaleX, 0.0f); // bottom right
		glVertex2i(x + wScaled, y + hScaled);

		// Bottom line
		glTexCoord2f(scaleX, 0.0f); // bottom right
		glVertex2i(x + wScaled, y + hScaled);
		glTexCoord2f(0.0f, 0.0f); // bottom left
		glVertex2i(x, y + hScaled);

		// Left line
		glTexCoord2f(0.0f, 0.0f); // bottom left
		glVertex2i(x, y + hScaled);
		glTexCoord2f(0.0f, 1.0f); // top left
		glVertex2i(x, y);
	}
	glEnd();
}

void DrawUtilities::glDrawRectangleOutline(GLuint tex, int x, int y, float width, float height)
{
	GLuint w = 1;
	GLuint h = 1;
	GLuint wScaled = static_cast<GLuint>(w * width);
	GLuint hScaled = static_cast<GLuint>(h * height);

	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_LINES);
	{
		glColor3ub(255, 255, 255);

		// Top line
		glTexCoord2f(0.0f, 1.0f); // top left
		glVertex2i(x, y);
		glTexCoord2f(width, 1.0f); // top right
		glVertex2i(x + wScaled, y);

		// Right line
		glTexCoord2f(width, 1.0f); // top right
		glVertex2i(x + wScaled, y);
		glTexCoord2f(width, 0.0f); // bottom right
		glVertex2i(x + wScaled, y + hScaled);

		// Bottom line
		glTexCoord2f(width, 0.0f); // bottom right
		glVertex2i(x + wScaled, y + hScaled);
		glTexCoord2f(0.0f, 0.0f); // bottom left
		glVertex2i(x, y + hScaled);

		// Left line
		glTexCoord2f(0.0f, 0.0f); // bottom left
		glVertex2i(x, y + hScaled);
		glTexCoord2f(0.0f, 1.0f); // top left
		glVertex2i(x, y);
	}
	glEnd();
}

void DrawUtilities::glDrawRectangleOutline(int x, int y, int w, int h, const RGBColor& outlineColor)
{
	glBegin(GL_LINES);
	{
		glColor3ub(outlineColor.r, outlineColor.g, outlineColor.b); // Set outline color

		// Top line
		glVertex2i(x, y);
		glVertex2i(x + w, y);

		// Right line
		glVertex2i(x + w, y);
		glVertex2i(x + w, y + h);

		// Bottom line
		glVertex2i(x + w, y + h);
		glVertex2i(x, y + h);

		// Left line
		glVertex2i(x, y + h);
		glVertex2i(x, y);
	}
	glEnd();
}


void DrawUtilities::glDrawRectangleOutlineFilled(GLuint tex, int x, int y, int w, int h, float scaleX, float scaleY, const RGBColor& fillColor)
{
	GLuint wScaled = static_cast<GLuint>(w * scaleX);
	GLuint hScaled = static_cast<GLuint>(h * scaleY);

	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_LINES);
	{
		glColor3ub(255, 255, 255); // Color for the outline

		// Top line
		glTexCoord2f(0.0f, 1.0f); // top left
		glVertex2i(x, y);
		glTexCoord2f(scaleX, 1.0f); // top right
		glVertex2i(x + wScaled, y);

		// Right line
		glTexCoord2f(scaleX, 1.0f); // top right
		glVertex2i(x + wScaled, y);
		glTexCoord2f(scaleX, 0.0f); // bottom right
		glVertex2i(x + wScaled, y + hScaled);

		// Bottom line
		glTexCoord2f(scaleX, 0.0f); // bottom right
		glVertex2i(x + wScaled, y + hScaled);
		glTexCoord2f(0.0f, 0.0f); // bottom left
		glVertex2i(x, y + hScaled);

		// Left line
		glTexCoord2f(0.0f, 0.0f); // bottom left
		glVertex2i(x, y + hScaled);
		glTexCoord2f(0.0f, 1.0f); // top left
		glVertex2i(x, y);
	}
	glEnd();

	// Fill the rectangle
	glColor3ub(fillColor.r, fillColor.g, fillColor.b); // Color for filling
	glBegin(GL_QUADS);
	{
		glVertex2i(x, y);
		glVertex2i(x + wScaled, y);
		glVertex2i(x + wScaled, y + hScaled);
		glVertex2i(x, y + hScaled);
	}
	glEnd();
}

void DrawUtilities::glDrawFilledRectangle(int x, int y, int w, int h, const RGBColor& fillColor)
{
	glBegin(GL_QUADS);
	{
		glColor3ub(fillColor.r, fillColor.g, fillColor.b); // Set fill color

		glVertex2i(x, y);
		glVertex2i(x + w, y);
		glVertex2i(x + w, y + h);
		glVertex2i(x, y + h);
	}
	glEnd();
}

void DrawUtilities::glDrawRectangleOutline(int x, int y, int w, int h, float scaleX, float scaleY, const RGBColor& color)
{
	GLuint wScaled = static_cast<GLuint>(w * scaleX);
	GLuint hScaled = static_cast<GLuint>(h * scaleY);

	// Save current state
	glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

	// Disable texturing and set color
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3ub(color.r, color.g, color.b); // Set color to specified RGB

	glBegin(GL_LINES);
	{
		// Top line
		glVertex2i(x, y); // Top left
		glVertex2i(x + wScaled, y); // Top right (scaled width)

		// Right line
		glVertex2i(x + wScaled, y); // Top right (scaled width)
		glVertex2i(x + wScaled, y + hScaled); // Bottom right (scaled width and height)

		// Bottom line
		glVertex2i(x + wScaled, y + hScaled); // Bottom right (scaled width and height)
		glVertex2i(x, y + hScaled); // Bottom left (scaled height)

		// Left line
		glVertex2i(x, y + hScaled); // Bottom left (scaled height)
		glVertex2i(x, y); // Top left
	}
	glEnd();

	// Restore previous state
	glPopClientAttrib();
	glPopAttrib();
}


void DrawUtilities::glDrawFilledRectangle(int x, int y, int w, int h, float scaleX, float scaleY, const RGBColor& color)
{
	GLuint wScaled = static_cast<GLuint>(w * scaleX);
	GLuint hScaled = static_cast<GLuint>(h * scaleY);

	// Save current state
	glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

	// Disable texturing and set color
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3ub(color.r, color.g, color.b); // Set color to specified RGB

	glBegin(GL_QUADS);
	{
		// Top left vertex
		glVertex2i(x, y);

		// Top right vertex (scaled width)
		glVertex2i(x + wScaled, y);

		// Bottom right vertex (scaled width and height)
		glVertex2i(x + wScaled, y + hScaled);

		// Bottom left vertex (scaled height)
		glVertex2i(x, y + hScaled);
	}
	glEnd();

	// Restore previous state
	glPopClientAttrib();
	glPopAttrib();
}

void DrawUtilities::glDrawFilledRectangle(int x, int y, int w, int h, float scaleX, float scaleY, const RGBColor& color, unsigned char opacity)
{
	GLuint wScaled = static_cast<GLuint>(w * scaleX);
	GLuint hScaled = static_cast<GLuint>(h * scaleY);

	// Save current state
	glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

	// Enable blending for transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Disable texturing and set color with opacity
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor4ub(color.r, color.g, color.b, opacity); // Set color to specified RGB and opacity

	glBegin(GL_QUADS);
	{
		// Top left vertex
		glVertex2i(x, y);

		// Top right vertex (scaled width)
		glVertex2i(x + wScaled, y);

		// Bottom right vertex (scaled width and height)
		glVertex2i(x + wScaled, y + hScaled);

		// Bottom left vertex (scaled height)
		glVertex2i(x, y + hScaled);
	}
	glEnd();

	// Restore previous state
	glDisable(GL_BLEND);
	glPopClientAttrib();
	glPopAttrib();
}



void DrawUtilities::glDrawFilledTriangle(int x, int y, int size, float scaleX, float scaleY, const RGBColor& color, float rotation)
{
	// Save current state
	glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT);
	glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

	// Disable texturing and set color
	glBindTexture(GL_TEXTURE_2D, 0);
	glColor3ub(color.r, color.g, color.b); // Set color to specified RGB

	// Convert rotation angle from degrees to radians
	float angle = rotation * PI / 180.0f;

	// Calculate half size for centering
	float halfSize = size / 2.0f;

	// Calculate triangle vertices
	float vertices[3][2] = {
		{0.0f, halfSize},       // Top vertex
		{-halfSize, -halfSize}, // Bottom left vertex
		{halfSize, -halfSize}   // Bottom right vertex
	};

	// Apply scaling and rotation to vertices
	for (int i = 0; i < 3; ++i)
	{
		float scaledX = vertices[i][0] * scaleX;
		float scaledY = vertices[i][1] * scaleY;
		vertices[i][0] = scaledX * cos(angle) - scaledY * sin(angle);
		vertices[i][1] = scaledX * sin(angle) + scaledY * cos(angle);
	}

	// Translate to the specified position
	glBegin(GL_TRIANGLES);
	{
		glVertex2f(x + vertices[0][0], y + vertices[0][1]);
		glVertex2f(x + vertices[1][0], y + vertices[1][1]);
		glVertex2f(x + vertices[2][0], y + vertices[2][1]);
	}
	glEnd();

	// Restore previous state
	glPopClientAttrib();
	glPopAttrib();
}


void DrawUtilities::glDrawAudio(GLuint tex, int x, int y, int w, int h, std::vector<float> audioData)
{
	glBindTexture(GL_TEXTURE_2D, tex);
	glBegin(GL_LINE_STRIP);
	{
		glColor3ub(255, 255, 255);

		// Calculate the scaling factors for x and y axes
		float scaleX = static_cast<float>(1);
		float scaleY = static_cast<float>(100); // Adjust for half height

		for (size_t i = 0; i < audioData.size()/128; ++i)
		{
			// Calculate the position on the screen
			float posX = x + i * scaleX;
			float posY = y + h / 2 + audioData[i] * scaleY;

			// Plot the point
			glVertex2f(posX, posY);
		}
	}
	glEnd();
}


void DrawUtilities::glDrawFrameScaled(GlDrawFrameParams params)
{
	GLuint wScaled = static_cast<GLuint>(params.w * params.scale);
	GLuint hScaled = static_cast<GLuint>(params.h * params.scale);

	glBindTexture(GL_TEXTURE_2D, params.tex);
	glBegin(GL_QUADS);
	{
		glColor3ub(255, 255, 255);

		glTexCoord2f(params.s1, params.t2);  // A
		glVertex2i(params.x, params.y);

		glTexCoord2f(params.s2, params.t2);  // B
		glVertex2i(params.x + wScaled, params.y);

		glTexCoord2f(params.s2, params.t1); // C
		glVertex2i(params.x + wScaled, params.y + hScaled);

		glTexCoord2f(params.s1, params.t1); // D
		glVertex2i(params.x, params.y + hScaled);
	}
	glEnd();
}

void DrawUtilities::glDrawFrameScaled(GlDrawFrameParams params, float angle)
{
	// Convert the angle to radians
	float radians = angle * (PI / 180.0f);

	// Calculate the half-width and half-height for scaling
	float halfWidth = params.w * params.scale * 0.5f;
	float halfHeight = params.h * params.scale * 0.5f;

	// Calculate the center of the frame
	float centerX = params.x + halfWidth;
	float centerY = params.y + halfHeight;

	// Calculate the four corners of the frame
	float corners[4][2] = {
		{-halfWidth, -halfHeight}, // bottom left
		{halfWidth, -halfHeight},  // bottom right
		{halfWidth, halfHeight},   // top right
		{-halfWidth, halfHeight}   // top left
	};

	// Rotate each corner around the center
	for (int i = 0; i < 4; ++i)
	{
		float rotatedX = corners[i][0] * cos(radians) - corners[i][1] * sin(radians);
		float rotatedY = corners[i][0] * sin(radians) + corners[i][1] * cos(radians);
		corners[i][0] = rotatedX + centerX;
		corners[i][1] = rotatedY + centerY;
	}

	// Bind the texture
	glBindTexture(GL_TEXTURE_2D, params.tex);
	glBegin(GL_QUADS);
	{
		glColor3ub(255, 255, 255);

		// Draw the frame with texture coordinates
		glTexCoord2f(params.s1, params.t2); // bottom left
		glVertex2f(corners[0][0], corners[0][1]);

		glTexCoord2f(params.s2, params.t2); // bottom right
		glVertex2f(corners[1][0], corners[1][1]);

		glTexCoord2f(params.s2, params.t1); // top right
		glVertex2f(corners[2][0], corners[2][1]);

		glTexCoord2f(params.s1, params.t1); // top left
		glVertex2f(corners[3][0], corners[3][1]);
	}
	glEnd(); // sends all submitted data to the GPU for rendering
}


// Draws a single frame of a GL texture
void DrawUtilities::glDrawFrame(GlDrawFrameParams params)
{
	glBindTexture(GL_TEXTURE_2D, params.tex);
	glBegin(GL_QUADS);
	{
		glColor3ub(255, 255, 255);

		glTexCoord2f(params.s1, params.t2);  // A
		glVertex2i(params.x, params.y);

		glTexCoord2f(params.s2, params.t2);  // B
		glVertex2i(params.x + params.w, params.y);

		glTexCoord2f(params.s2, params.t1); // C
		glVertex2i(params.x + params.w, params.y + params.h);

		glTexCoord2f(params.s1, params.t1); // D
		glVertex2i(params.x, params.y + params.h);
	}
	glEnd();
}


/* drawRasterText - Draws a text string at (x, y) in the game world. tex must be an font image
 * param tex - the font image
 * param x - x position in the game world to draw the string
 * param y - y position in the game world to draw the string
 * param w is frame width - width of part of texture to be drawn
 * param h is frame height - height of part of texture to be drawn
 */
void DrawUtilities::drawRasterText(GLuint tex, int x, int y, int w, int h, char string[])
{
	float s1 = 0.0f; //---------------define just to get it to compile, fix later
	float s2 = 0.0f; //---------------define just to get it to compile, fix later
	float t1 = 0.0f; //---------------define just to get it to compile, fix later
	float t2 = 0.0f; //---------------define just to get it to compile, fix later
	float frameDivision = 0.0f; //---------------define just to get it to compile, fix later
	float rowDivision = 0.0f; //---------------define just to get it to compile, fix later
	int numberOfFrames = 0; //-----------------define just to get it to compile, fix later
	int numberOfRows = 0; //-----------------define just to get it to compile, fix later
	int currentRow = 0; //-----------------define just to get it to compile, fix later

	const size_t N = strlen(string);
	for (int i = 0; i < N; i++)
	{
		int frame = string[i] - 32; //the current frame or letter to draw
		// Keep currentFrame in range
		if (frame >= numberOfFrames)
			frame = frame % numberOfFrames;

		// current row may change. Use modulus
		if (s2 >= 1.0 && numberOfRows > 1)
			currentRow = (currentRow + 1) % numberOfRows;

		// update s1, s2, t1, t2
		s1 = frame * frameDivision;
		s2 = (frame * frameDivision) + frameDivision;
		t1 = currentRow * rowDivision;
		t2 = (currentRow * rowDivision) + rowDivision;

		GlDrawFrameParams params;
		params.tex = tex;
		params.x = x;
		params.y = y;
		params.w = w;
		params.h = h;
		params.s1 = s1;
		params.s2 = s2;
		params.t1 = t1;
		params.t2 = t2;

		glDrawFrame(params);
	}
}

// Draw the sprite rotated by the number of degrees specified by the angle parameter
void DrawUtilities::glDrawSpriteRotate(GLuint tex, int x, int y, int w, int h, GLfloat angle)
{

	glMatrixMode(GL_TEXTURE); // make the texture matrix the target of matrix function calls

	glLoadIdentity();
	glPushMatrix();

	glRotatef(90.0, 0.0f, 0.0f, 1.0f);

	glDrawSprite(tex, x, y, w, h);

	glPopMatrix();
}

void DrawUtilities::DrawRectangle(int x, int y, int w, int h, const RGBColor& fillColor)
{
	GLfloat xf = static_cast<GLfloat>(x);
	GLfloat yf = static_cast<GLfloat>(y);

	glBegin(GL_QUADS);
	{
		glColor3ub(fillColor.r, fillColor.g, fillColor.b); // Set fill color

		glVertex2f(xf, yf);           // Bottom-left corner
		glVertex2f(xf + w, yf);   // Bottom-right corner
		glVertex2f(xf + w, yf + h); // Top-right corner
		glVertex2f(xf, yf + h);  // Top-left corner
	}
	glEnd();
}