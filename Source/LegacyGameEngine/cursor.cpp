#include"cursor.h"

Cursor::Cursor(AnimationData &aData){
	image = NULL;
	animData = aData;
	animFPS = 8.0f;
	prevAnim = 0;
	frameNum = 0;
	frameTime = 0;

	// Position of cursor in the GUI
	xPos = 27;
	yPos = 570;

	// Width and height of cursor
	cursorWidth = 18;
	cursorHeight = 2;

	// Choose the first animation (actually, the only animation since it's simply a cursor)
	changeAnim(0);
}

void Cursor::draw(){
	glDrawSprite(image,  xPos, yPos, cursorWidth, cursorHeight);
}

void Cursor::draw(int x, int y){
	glDrawSprite(image,  x, y, cursorWidth, cursorHeight);
}

void Cursor::update(float deltaTime){

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
		}

		// Update the active image
		int imageNum = animData.frameInfo.at(animNum).startFrame + frameNum;
		image = animData.images.at(imageNum);

		frameTime = fmod(frameTime, 1 / animFPS);
	}
}

void Cursor::changeAnim(int num){
	animNum = num;
	// The active animation is now at from 0 and 0.0f time
	frameNum = 0;
	frameTime = 0.0f;
	// Set active image, which is just the starting frame
	int imageNum = animData.frameInfo.at(animNum).startFrame;
	image = animData.images.at(imageNum);
}