#pragma once
#ifndef TIMER_H
#define TIMER_H
#include<SDL.h>
#include<string>
#include<sstream>

/* File Name - timer.h
   Author - Frank Mock
   Timer gives objects a way to keep track of time in seconds
*  The main game loop advances the timer and objects with a reference to timer 
*  can query it to get time values to use for game control.
*  The seconds count can be reset to zero.
*  Timer utilizes the SDL function SDL_GetTicks() for a millisecond count
*/

using std::ostringstream;

struct Timer{
public:
	int time; // Amount of seconds that have passed since timer started
	int prevTime;
	int count; // Resetable seconds count
	bool reset; // When true, the seconds count will be set back to zero

	inline Timer(){}

	// Initialize with SDL_GetTicks()
	inline Timer(int t):time(t), prevTime(0), count(0), reset(false){}

	inline void reSet(){ reset = true;}

	// Advance the elapsed time or reset the seconds count
	inline void update(float deltaTime){

		if(reset)
			count = 0;
		else{
			prevTime = time;
			int ms = SDL_GetTicks();
			time += deltaTime;
			time = (ms - time) / 1000;
			if(time - prevTime == 1)
				count++;
		}
		reset = false;
	}

	inline int getElapsedTime() const { return time; }

	inline std::string to_string() const{
	ostringstream oss;
	oss << "*************************\n"
		<< "Elapsed Time (sec.)= " <<  time << "\n"
		<< "count = " << count << "\n"
		<< "reset = " << reset << "\n";
		return oss.str();
	}
};
#endif