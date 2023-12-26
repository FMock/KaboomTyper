#pragma once
#ifndef TEXTBLOCK_H
#define TEXTBLOCK_H
#include"sprite.h"
#include<map>
#include<vector>
#include"game_data.h"
#include <sstream>
#include<ctime>

// Author - Frank Mock
// TextBlock.h
// TextBlock is a composite sprite consisting of a rectangular 
// block with text imposed over it

class TextBlock : public Sprite{
public:
	TextBlock():Sprite(){}
	TextBlock(int, int, int, int, std::string, std::map<std::string, GLuint>& tMap);

	std::map<std::string, GLuint> strToImageMap;
	
	std::vector<std::string> colors;

	std::string color;
	
	std::string text; // the text to be displayed on the block

	int fontWidth;
	int fontHeight;

	// The direction the character is facing
	int direction;

	int prev_change_x;

	int prev_change_y;

	// If x and y velocity is not 0
	bool moving;

	bool collided;

	// True when hit by any enemy
	bool isHit;

	// If True remove TextBlock from game
	bool remove;

	void update(float deltaTime);

	void draw();

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stop();
	void collision(Sprite &sprite);
	std::string to_string() const;
};
#endif