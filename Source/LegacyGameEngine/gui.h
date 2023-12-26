#pragma once
#ifndef GUI_H
#define GUI_H
#include<GL/glew.h>
#include"DrawUtils.h"
#include"game_data.h"
#include <sstream>
#include<ctime>
#include"cursor.h"
#include"keystates.h"
#include"selection.h"


// Author - Frank Mock
// gui.h
// The graphic display of the game 

class GUI{
public:
	GUI(KeyStates&, Game_Data&);

	void loadImages();
	KeyStates *keyStates;
	Game_Data *gameData;
	std::vector<Selection> selectionItems; // word category selections
	Selection musicOptionSelection;
	int playMusicOptionCount;
	GLuint logo;
	GLuint cloudBackground;
	GLuint textTypeArea;
	GLuint vertBorder;
	GLuint horzBorder;
	GLuint optionsClosed;
	GLuint optionsOpen;
	GLuint helpClosed;
	GLuint helpOpen;
	GLuint helpMenu;
	GLuint aboutClosed;
	GLuint aboutOpen;
	GLuint marker_left;
	GLuint marker_right;

	std::string menuItemSelected;
	int selectedIndex;

	int width;
	int height;

	bool options;
	void setOptions(bool);
	bool getOptions() const;
	void buildSelections();

	void update(float deltaTime);

	void draw();

	std::string to_string() const;
};
#endif