#pragma once
#ifndef GAME_DATA_H
#define GAME_DATA_H
#include<string>
#include<sstream>

/* All objects with a reference to Game_Data can track important game
* information to use with game control or response */

using std::ostringstream;


struct Game_Data{
public:
	inline Game_Data():currentGameState(0),
								 // player_x(0), /****** NOT USED FOR THIS TYPE OF GAME *****
								 // player_y(0), /****** NOT USED FOR THIS TYPE OF GAME *****
								  score(0),
								  newHighScore(false),
								  highScore(0),
								  totalCount(0),
								  correctCount(0),
								  currentWord(""),
								  optionsDisplayed(false),
								  aboutDisplayed(false),
								  helpDisplayed(false),
								  level(0),
								  playMusic(true),
								  playMusicOptionCount(0),
								  playMenuSelectionSound(false),
								 // player_direction(0),  /****** NOT USED FOR THIS TYPE OF GAME *****
								  lives_remaining(0),
								  mouseClicked(false),
								  mouseX(0),
								  mouseY(0){}

	enum state {START, PLAY, PAUSE, END};

	// General Game Info
	static const int WINDOW_WIDTH = 800;   // Camera Width
	static const int WINDOW_HEIGHT = 600;  // Camera Height
	static const int WORLD_WIDTH = 800;
	static const int WORLD_HEIGHT = 600;
	static const int BORDER_WIDTH = 20;
	static const int BLOCK_AREA_WIDTH = 500;
	static const int BLOCK_AREA_TO_RH_EDGE = 300;
	static const int BL_FLOOR_TO_BOTTOM = 100;
	static const int BLOCK_FALL_AREA_MAX_X = 400;
	static const int HUD_START_X = 520;
	static const int GAME_STATUS_Y = 340;

	// Button Range points
	static const int OPTION_BUTTON_MIN_X = 526;
	static const int OPTION_BUTTON_MAX_X = 676;
	static const int OPTION_BUTTON_MIN_Y = 225;
	static const int OPTION_BUTTON_MAX_Y = 252;

	static const int ABOUT_BUTTON_MIN_X = 529;
	static const int ABOUT_BUTTON_MAX_X = 623;
	static const int ABOUT_BUTTON_MIN_Y = 311;
	static const int ABOUT_BUTTON_MAX_Y = 330;

	static const int HELP_BUTTON_MIN_X = 529;
	static const int HELP_BUTTON_MAX_X = 614;
	static const int HELP_BUTTON_MIN_Y = 268;
	static const int HELP_BUTTON_MAX_Y = 289;

	static const int PLAY_MUSIC_OPTION_MIN_X = 745;
	static const int PLAY_MUSIC_OPTION_MAX_X = 767;
	static const int PLAY_MUSIC_OPTION_MIN_Y = 490;
	static const int PLAY_MUSIC_OPTION_MAX_Y = 507;

	static const int MAX_STRING_SIZE = 32;


	float deltaTime;
	int currentGameState;
	int score;
	int highScore;
	bool newHighScore;
	int lives_remaining;
	int level;
	int totalCount;
	int correctCount;
	std::string currentWord;
	std::string highScoreInitials;
	bool playMusic;
	int playMusicOptionCount;
	bool playMenuSelectionSound;

	// Mouse Data
	bool mouseClicked;
	int mouseX;
	int mouseY;

	// GUI related
	bool optionsDisplayed;
	bool aboutDisplayed;
	bool helpDisplayed;
	std::string menuItemSelected;

	/****** NOT USED FOR THIS GAME TYPE
	// Player Info
	int player_x;
	int player_y;
	int player_direction; // LEFT = 0, RIGHT = 1, UP = 2, DOWN = 3
	
	inline void recordPlayersXPos(int x){ player_x = x * 2; }
	inline void recordPlayersYPos(int y){ player_y = y * 2; }
	inline void recordPlayerDirection(int d) {player_direction = d; }

	inline int getPlayerXPos(){ return player_x; }
	inline int getPlayerYPos(){ return player_y; }
	*/

	inline std::string to_string() const{
	ostringstream oss;
	oss << "*************************\n"
		<< "currentWord = " << currentWord << "\n"
		<< "score = " << score << "\n"
		<< "newHighScore = " << newHighScore << "\n"
		<< "highScoreInitials = " << highScoreInitials << "\n"
		<< "highScore = " << highScore << "\n"
		<< "totalCount = " << totalCount << "\n"
		<< "correctCount = " << correctCount << "\n"
		<< "level = " << level << "\n"
		<< "lives remaining = " << lives_remaining << "\n"
		//<< "Player's X pos. = " <<  player_x << "\n"   /****** NOT USED FOR THIS TYPE OF GAME *****
		//<< "Player's Y pos. = " <<  player_y << "\n"   /****** NOT USED FOR THIS TYPE OF GAME *****
		<< "playMusic = " << playMusic << "\n"
		<< "playMusicOptionCount = " << playMusicOptionCount << "\n"
		<< "optionsDisplayed = " << optionsDisplayed << "\n"
		<< "aboutDisplayed = " << aboutDisplayed << "\n"
		<< "helpDisplayed = " << helpDisplayed << "\n"
		<< "menuItemSelected = " << menuItemSelected << "\n"
		<< "mouseClicked = " << mouseClicked << "\n"
		<< "mouseX = " << mouseX << "\n"
		<< "mouseY = " << mouseY << "\n"
		<< "currentGameState = " << currentGameState << "\n";
		//<< "Player's Direction = " <<  player_direction << "\n";  /****** NOT USED FOR THIS TYPE OF GAME *****
		return oss.str();
	}
};
#endif