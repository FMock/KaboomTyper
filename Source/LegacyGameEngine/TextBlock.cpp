#include "TextBlock.h"

/**
* File Name - TextBlock.cpp
* Author - Frank Mock
* 
* TextBlock class represents a colored block with text on it
*/

using std::ostringstream;
typedef Game_Data GD;

enum Direction{LEFT, RIGHT, UP, DOWN };

TextBlock::TextBlock(int x, int y, int w, int h, std::string s, std::map<std::string, GLuint>& tMap):Sprite(x, y, w, h), strToImageMap(tMap){
	colors.push_back("white"), colors.push_back("blue"), colors.push_back("green");
	colors.push_back("yellow"), colors.push_back("purple"), colors.push_back("red");
	colors.push_back("orange");
	srand(time(0));
	color = colors.at(rand() % 7);
	fontWidth = 23;
	fontHeight = 25;
	text = s;
	collided = false;
	remove = false;
	isHit = false;
	moving = true;
	direction = DOWN;
	box.setW(characterSize[0]);
	box.setH(characterSize[1]);
	//loadImages();
	setWidth(100);
	setHeight(25);
	setXSpeed(100);
	setYSpeed(100);
}


// Draw the textBlock to the screen
void TextBlock::draw(){
	int textSize = text.size();
	setWidth(textSize * fontWidth);
	glDrawSprite(strToImageMap[color],  characterPos[0], characterPos[1], characterSize[0], characterSize[1]);

	// For each character of text, get it's corresponding image from the map and draw it
	int offset = 0;
	for(std::string::size_type i = 0; i < textSize; ++i) {
		// convert char to string
		std::stringstream ss;
		ss << text[i];
		std::string s;
		ss >> s; // s is a key in strToImageMap
		glDrawSprite(strToImageMap[s],  characterPos[0] + offset, characterPos[1], fontWidth, fontHeight);
		offset += fontWidth;
	}
}

void TextBlock::update(float deltaTime){
	if(moving){
		moveDown();

		if(characterPos[0] > GD::WORLD_WIDTH - characterSize[0] - GD::BLOCK_AREA_TO_RH_EDGE){
			change_x = 0; //stop block from going out of bounds (right boundry)
			characterPos[0] -= 1;
		}

		if(characterPos[0] < 0){
			change_x = 0; //stop block from going out of bounds (left boundry)
			characterPos[0] += 1;
		}

		if(characterPos[1] < 0){
			change_y = 0; //stop block from going out of bounds up (upper boundry)
			characterPos[1] += 1;
		}

		if(characterPos[1] > GD::WORLD_HEIGHT - characterSize[1] - GD::BL_FLOOR_TO_BOTTOM){
			change_y = 0; //stop block from going out of bounds up (lower boundry)
			characterPos[1] -= 1;
			moving = false;
		}

		characterPos[0] += change_x * deltaTime;
		box.setX(abs(characterPos[0]));
		characterPos[1] += change_y * deltaTime;
		box.setY(abs(characterPos[1]));
	}else{
		stop();
	}
}

void TextBlock::moveLeft(){
	moving = true;
	prev_change_y = change_y;
	change_y = 0;
	prev_change_x = change_x;
	change_x = -speedX;
	direction = LEFT;
}

void TextBlock::moveRight(){
	moving = true;
	prev_change_y = change_y;
	change_y = 0;
	prev_change_x = change_x;
	change_x = speedX;
	direction = RIGHT;
}

void TextBlock::moveUp(){
	moving = true;
	prev_change_x = change_x;
	change_x = 0;
	prev_change_y = change_y;
	change_y = -speedY;
	direction = UP;
}

void TextBlock::moveDown(){
	moving = true;
	prev_change_x = change_x;
	change_x = 0;
	prev_change_y = change_y;
	change_y = speedY;
	direction = DOWN;
}

void TextBlock::stop(){
	moving = false;
	prev_change_x = change_x;
	change_x = 0;
	prev_change_y = change_y;
	change_y = 0;
}

void TextBlock::collision(Sprite &sprite){
	collided = true;
}


std::string TextBlock::to_string() const{
	ostringstream oss;
	oss << "TextBlock ************\n"
		<< "isDead = " << remove << "\n"
		<< "moving = " << moving << "\n"
		<< "Direction = " << direction << "\n"
		<< "isHit = "  << isHit << "\n"
		<< "AABB x = " << box.x << "\n"
		<< "AABB y = " << box.y << "\n"
		<< "AABB w = " << box.w << "\n"
		<< "AABB h = " << box.h << "\n"
		<< "xPos = " << getXPos() << "\n"
		<< "YPos = " << getYPos() << "\n"
		<< "END TextBlock *********\n";
	return oss.str();
}
