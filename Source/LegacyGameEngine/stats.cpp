#include"stats.h"

Stats::Stats(Game_Data& g):score(0),totalCount(0),correctCount(0){
	gameData = &g;
}

void Stats::increaseScore(int x){
	if(x >= 0)
		score += x;
	gameData->score = score;
}

void Stats::decreaseScore(int x){
	if(score >= x){
		score -= x;
		gameData->score = score;
	}
}

int Stats::getScore() const{
	return score;
}

void Stats::increaseTotalCount(int x){
	if(x >= 0)
		totalCount += x;
	gameData->totalCount = totalCount;
}

int Stats::getTotalCount(){
	return totalCount;
}

void Stats::increaseCorrectCount(int x){
	if(correctCount >=0)
		correctCount += x;
	gameData->correctCount = correctCount;
}

int Stats::getCorrectCount(){
	return correctCount;
}

void Stats::reset(){
	score = 0;
	totalCount = 0;
	correctCount = 0;
	gameData->score = 0;
	gameData->correctCount = 0;
	gameData->totalCount = 0;
	gameData->currentWord = "";
}

std::string Stats::to_string() const{
	std::ostringstream oss;
	oss << "Stats ************\n"
		<< "score = " << score << "\n"
		<< "totalCount = " << totalCount << "\n"
		<< "correctCount = " << correctCount << "\n";
	return oss.str();
}