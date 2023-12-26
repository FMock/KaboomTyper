#pragma once
#ifndef HIT_H
#define HIT_H
#include"Sprite.h"
#include"AnimationData.h"
#include"game_data.h"

// Represents all the hit animations

class Hit : public Sprite{
public:
	Hit():Sprite(){}
	Hit(int, int, int, int, AnimationData &animData, int moveDir);
	// All of the animation data (includes image files and frame data
	AnimationData animData;
	bool finished;
	bool collieded;
	// The direction the character is facing
	int direction;
	// If x and y velocity is 0 player has stopped moving
	bool stopped;
	// True when hit by any enemy
	bool isHit;
	// If True remove skeleton from game
	bool isDead;
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
	void updateAnim(float deltaTime);
	//void draw(int, int);
	void changeAnim(int num);
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stop();
	void collision(Sprite &sprite);
	std::string to_string() const;
	int prev_change_x;
	int prev_change_y;
};
#endif