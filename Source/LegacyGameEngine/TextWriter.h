#ifndef TEXTWRITER_H
#define TEXTWRITER_H

#include<GL/glew.h>
#include"DrawUtils.h"
#include<cstdlib>
#include<string>
#include<sstream>
#include<map>
#include"cursor.h"

class TextWriter{
public:
	TextWriter(std::string s, std::map<std::string, GLuint>& tMap);
	std::map<std::string, GLuint> strToImageMap;
	std::string text; // the text to be displayed
	int fontWidth;
	int fontHeight;
	int cursorWidth;
	int cursorHeight;
	int xPosition;
	int yPosition;
	int yPositionCursor;
	int offset;
	void draw();
	void update(std::string);

	// Cursor related
	Cursor *cursor;
	std::vector<GLuint> cursorImages;
	std::vector<AnimFrameData> cursorframeInfo;
	AnimationData cursorAnimData; // cursor animation data

	std::string to_string() const;
};

#endif