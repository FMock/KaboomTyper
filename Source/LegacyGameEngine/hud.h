#ifndef HUD_H
#define HUD_H
#include<map>
#include<GL/glew.h>
#include"DrawUtils.h"
#include <sstream>
#include"game_data.h"

/* HUD (Heads Up Display)
 * To display useful info to player such as score, high score, ect.
 */

class Hud{
public:
	Game_Data *gameData;
	int hudWidth;
	int hudHeight;
	int hudWidthBottom;
	int hudHeightBottom;
	int fontWidth;
	int fontHeight;
	int startXPos;
	int initialsXPos;
	int totalStartXPos;
	int correctStartXPos;
	int scoreYPos;
	int highScoreYPos;
	int totalWordsYPos;
	int correctYPos;
	GLuint score;
	GLuint wordStats;
	GLuint totalWords;
	GLuint correct;
	GLuint hudBottom;
	inline Hud(){};
	Hud(std::map<std::string, GLuint>& tMap, Game_Data&);
	std::map<std::string, GLuint> strToImageMap;
	void draw();
	void update();
	std::string int_to_string(int);
private:
	void drawScore();
	void drawHighScore();
	void drawPlayerInitials();
	void drawTotalCount();
	void drawCorrectCount();
};
#endif