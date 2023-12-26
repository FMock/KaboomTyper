#ifndef CURSOR_H
#define CURSOR_H

#include<GL/glew.h>
#include"DrawUtils.h"
#include"AnimationData.h"

class Cursor{
public:
	Cursor(AnimationData &animData);
	AnimationData animData;
	void draw();
	void draw(int x, int y);
	void update(float deltaTime);
	void changeAnim(int num);
	GLuint image;
	// The particular animation that is active
	int animNum;
	// The previous animation
	int prevAnim;
	//The frame number of the active animation that's being displayed
	int frameNum;
	// The amount of time the current frame has been displayed
	float frameTime;
	// The FPS the animation is running at (24 FPS by default)
	float animFPS;
	
	// position of cursor
	int xPos;
	int yPos;

	// cursor width and height
	int cursorWidth;
	int cursorHeight;
};

#endif