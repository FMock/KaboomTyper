#ifndef STATS_H
#define STATS_H

#include"game_data.h"
#include<sstream>

class Stats{
public:
	inline Stats(){};
	Stats(Game_Data&);
	Game_Data *gameData;
	int score;
	int totalCount;
	int correctCount;
	void increaseScore(int x);
	void decreaseScore(int x);
	int getScore() const;
	void increaseTotalCount(int x);
	int getTotalCount();
	void increaseCorrectCount(int x);
	int getCorrectCount();
	void reset();
	std::string to_string() const;
}; 
#endif