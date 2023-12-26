#include "hit.h"
#include"game_data.h"

/**
* Hit class represents the hit animation objects
* hit objects are created when wordBlocks are about to be removed
*/

using std::ostringstream;
typedef Game_Data GC;

enum facing{LEFT, RIGHT, UP, DOWN };

Hit::Hit(int x, int y, int w, int h, 
	                       AnimationData &aData, int moveDir):Sprite(x, y, w, h){
	type = "hit";
	collieded = false;
	isDead = false;
	isHit = false;
	box.setW(characterSize[0]);
	box.setH(characterSize[1]);
	animFPS = 8.0f;
	direction = 1;
	stopped = true;
	animData = aData;
	changeAnim(0);
	prevAnim = 0;
	if(moveDir == 0)
		moveDown();
	else if(moveDir == 1)
		moveUp();
	else if(moveDir == 2)
		moveLeft();
	else if(moveDir == 3)
		moveRight();
	else
		stop();
}

// x and y should be the camera's x and y
/*
void Hit::draw(int x, int y){
	glDrawSprite(image,  characterPos[0] - x, characterPos[1] - y, characterSize[0], characterSize[1]);
}
*/

void Hit::updateAnim(float deltaTime){
	// update how long the current frame has been displayed
	frameTime += deltaTime;
	
	// This check determines if it is time ito change to the next frame
	if(frameTime > (1/animFPS)){
		// The number of frames to increment is the integral result of
		// frametime / (1 / animFPS), which is frameTime * animFPS
		frameNum += frameTime * animFPS;

		// Check if we've advanced past the last frame and must wrap
		if(frameNum >= animData.frameInfo.at(animNum).numFrames){
			frameNum = frameNum % animData.frameInfo.at(animNum).numFrames;
			finished = true;
		}

		// Update the active image
		int imageNum = animData.frameInfo.at(animNum).startFrame + frameNum;
		image = animData.images.at(imageNum);

		frameTime = fmod(frameTime, 1 / animFPS);
	}
}

void Hit::changeAnim(int num){
	animNum = num;
	// The active animation is now at from 0 and 0.0f time
	frameNum = 0;
	frameTime = 0.0f;
	// Set active image, which is just the starting frame
	int imageNum = animData.frameInfo.at(animNum).startFrame;
	image = animData.images.at(imageNum);
}


void Hit::moveLeft(){
	stopped = false;
	prev_change_y = change_y;
	change_y = 0;
	prev_change_x = change_x;
	change_x = -speedX;
	direction = LEFT;
	changeAnim(2);
}

void Hit::moveRight(){
	stopped = false;
	prev_change_y = change_y;
	change_y = 0;
	prev_change_x = change_x;
	change_x = speedX;
	direction = RIGHT;
	changeAnim(3);
}

void Hit::moveUp(){
	stopped = false;
	prev_change_x = change_x;
	change_x = 0;
	prev_change_y = change_y;
	change_y = -speedY;
	direction = UP;
	changeAnim(1);
}

void Hit::moveDown(){
	stopped = false;
	prev_change_x = change_x;
	change_x = 0;
	prev_change_y = change_y;
	change_y = speedY;
	direction = DOWN;
	changeAnim(0);
}

void Hit::stop(){
	stopped = true;
	prev_change_x = change_x;
	change_x = 0;
	prev_change_y = change_y;
	change_y = 0;
}

void Hit::collision(Sprite &sprite){
	collieded = true;
}


std::string Hit::to_string() const{
	ostringstream oss;
	oss << "Hit ************\n"
		<< "isDead = " << isDead << "\n"
		<< "Direction = " << direction << "\n"
		<< "isHit = "  << isHit << "\n"
		<< "AABB x = " << box.x << "\n"
		<< "AABB y = " << box.y << "\n"
		<< "AABB w = " << box.w << "\n"
		<< "AABB h = " << box.h << "\n"
		<< "xPos = " << getXPos() << "\n"
		<< "YPos = " << getYPos() << "\n"
		<< "END Hit *********\n";
	return oss.str();
}
