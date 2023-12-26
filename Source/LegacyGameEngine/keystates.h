#ifndef KEYSTATES_H
#define KEYSTATES_H

#include<SDL.h>
#include<algorithm> //std::fill
#include<string>
#include<sstream>
#include"game_data.h"

/*
	Track Keyboard Presses
*/

class KeyStates{
public:
	KeyStates(Game_Data&);
	Game_Data *gameData;
	static const int NUM_KEYS = 128;
	int states[NUM_KEYS]; // helps track key presses
	void zeroAllKeyStates();
	void setKeyPressed(); // sets a key to 1 if pressed
	bool mouseClicked;
	int mouseClickedCount;
	int mouseX;
	int mouseY;
	bool optionButtonPressed;
	bool aboutButtonPressed;
	bool helpButtonPressed;
	bool optionsDisplayed;
	bool aboutDisplayed;
	bool helpDisplayed;
	bool getOptionButtonPressed();
	int optionButtonPressedCount;
	int aboutButtonPressedCount;
	int helpButtonPressedCount;
	std::string to_string() const;
};

#endif