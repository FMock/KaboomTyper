#include "TextBlock.h"
#include "AABB.h"
#include "DrawUtils.h"

/**
* File Name - TextBlock.cpp
* Author - Frank Mock
* 
* TextBlock class represents a colored block with text on it
*/
using namespace GameEngine;
using namespace DrawUtilities;
using std::ostringstream;
typedef Game_Data GD;

enum Direction{LEFT, RIGHT, UP, DOWN };

// ****** RETHINK THIS CONSTRUCTOR - DOES w and h NEED TO BE PASSED IN WITH EACH OBJECT
// ******

TextBlock::TextBlock(int x, int y, int w, int h, std::string s, std::map<std::string, GLuint>& tMap):Sprite(x, y, w, h), m_strToImageMap(tMap){
	m_colors.push_back("white"), m_colors.push_back("blue"), m_colors.push_back("green");
	m_colors.push_back("yellow"), m_colors.push_back("purple"), m_colors.push_back("red");
	m_colors.push_back("orange");
	srand(time(0));
	m_color = m_colors.at(rand() % 7);
	fontWidth = 31; // 23 previously - TODO THIS SHOULD NOT BE HARD-CODED
	fontHeight = 36; //25 previously - TODO THIS SHOULD NOT BE HARD-CODED
	m_text = s;
	collided = false;
	remove = false;
	isHit = false;
	moving = true;
	direction = DOWN;
	box.setW(m_characterSize[0]);
	box.setH(m_characterSize[1]);
	//loadImages();
	//setWidth(w);  // 30 a single textblock is 30 pixels wide
	setHeight(h); // 30 a single textblock is 30 pixels high, When constructed we only need the height
	int textSize = m_text.size(); // TODO: THERE SHOULD BE A m_textSize member of TextString that TextBlock uses
	setWidth(textSize * fontWidth);
}


// Draw the textBlock to the screen
void TextBlock::Draw(){
	//int textSize = m_text.size(); // TODO: THERE SHOULD BE A m_textSize member of TextString that TextBlock uses
	//setWidth(textSize * fontWidth);
	glDrawSprite(m_strToImageMap[m_color],  m_characterPos[0], m_characterPos[1], m_characterSize[0], m_characterSize[1]);

	// For each character of text, get it's corresponding image from the map and draw it
	//int offset = 0;
	//for(std::string::size_type i = 0; i < textSize; ++i) {
	//	// convert char to string
	//	std::stringstream ss;
	//	ss << m_text[i];
	//	std::string s;
	//	ss >> s; // s is a key in strToImageMap
	//	glDrawSprite(m_strToImageMap[s],  m_characterPos[0] + offset, m_characterPos[1], fontWidth, fontHeight);
	//	offset += fontWidth;
	//}
}

void TextBlock::update(float deltaTime){
	if(moving){
		moveDown();

		if(m_characterPos[0] > GD::WORLD_WIDTH - m_characterSize[0] - GD::BLOCK_AREA_TO_RH_EDGE){
			change_x = 0; //stop block from going out of bounds (right boundry)
			m_characterPos[0] -= 1;
		}

		if(m_characterPos[0] < 0){
			change_x = 0; //stop block from going out of bounds (left boundry)
			m_characterPos[0] += 1;
		}

		if(m_characterPos[1] < 0){
			change_y = 0; //stop block from going out of bounds up (upper boundry)
			m_characterPos[1] += 1;
		}

		if(m_characterPos[1] > GD::WORLD_HEIGHT - m_characterSize[1] - GD::BL_FLOOR_TO_BOTTOM){
			change_y = 0; //stop block from going out of bounds up (lower boundry)
			m_characterPos[1] -= 1;
			moving = false;
		}

		m_characterPos[0] += change_x * deltaTime;
		box.setX(abs(m_characterPos[0]));
		m_characterPos[1] += change_y * deltaTime;
		box.setY(abs(m_characterPos[1]));
	}else{
		stop();
	}
}

void TextBlock::moveLeft(){
	moving = true;
	prev_change_y = change_y;
	change_y = 0;
	prev_change_x = change_x;
	//change_x = -speedX;
	direction = LEFT;
}

void TextBlock::moveRight(){
	moving = true;
	prev_change_y = change_y;
	change_y = 0;
	prev_change_x = change_x;
	//change_x = speedX;
	direction = RIGHT;
}

void TextBlock::moveUp(){
	moving = true;
	prev_change_x = change_x;
	change_x = 0;
	prev_change_y = change_y;
	//change_y = -speedY;
	direction = UP;
}

void TextBlock::moveDown(){
	moving = true;
	prev_change_x = change_x;
	change_x = 0;
	prev_change_y = change_y;
	//change_y = speedY;
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
