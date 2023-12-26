#include"hud.h"

typedef Game_Data GD;

Hud::Hud(std::map<std::string, GLuint>& tMap, Game_Data& g):strToImageMap(tMap),
	hudWidth(260), 
	hudHeight(25),
	hudWidthBottom(260),
	hudHeightBottom(18),
	fontWidth(23),
	fontHeight(25),
	startXPos(590),
	initialsXPos(700),
	totalStartXPos(660),
	scoreYPos(82),
	highScoreYPos(114),
	totalWordsYPos(146),
	correctStartXPos(679),
	correctYPos(178){
	gameData = &g;
	score = glTexImageTGAFile("../../Resources/images/score.tga", &hudWidth, &hudHeight);
	wordStats = glTexImageTGAFile("../../Resources/images/high_score.tga", &hudWidth, &hudHeight);
	totalWords = glTexImageTGAFile("../../Resources/images/num_words.tga", &hudWidth, &hudHeight);
	correct = glTexImageTGAFile("../../Resources/images/correct.tga", &hudWidth, &hudHeight);
	hudBottom = glTexImageTGAFile("../../Resources/images/hud_bottom.tga", &hudWidthBottom, &hudHeightBottom);
}

void Hud::draw(){
	glDrawSprite(score, GD::HUD_START_X, 75, hudWidth, hudHeight);
	glDrawSprite(wordStats, GD::HUD_START_X, 107, hudWidth, hudHeight);
	glDrawSprite(totalWords, GD::HUD_START_X, 139, hudWidth, hudHeight);
	glDrawSprite(correct, GD::HUD_START_X, 171, hudWidth, hudHeight);
	drawScore();
	drawHighScore();
	drawPlayerInitials();
	drawTotalCount();
	drawCorrectCount();
	glDrawSprite(hudBottom, 520, 203, hudWidthBottom, hudHeightBottom);
}

void Hud::drawScore(){
		//convert the score to a string
	std::string scoreStr = int_to_string(gameData->score);

	// For each character of score string, get it's corresponding image from the map and draw it
	int scoreTextSize = scoreStr.size();
	int offset = 0;
	for(std::string::size_type i = 0; i < scoreTextSize; ++i) {
		// convert char to string
		std::stringstream ss;
		ss << scoreStr[i];
		std::string s;
		ss >> s; // s is a key in strToImageMap
		glDrawSprite(strToImageMap[s], startXPos + offset, scoreYPos, fontWidth, fontHeight);
		offset += fontWidth;
	}
}

// Draw the highscore in the HUD
void Hud::drawHighScore(){
	//convert the high score to a string
	std::string highScoreStr = int_to_string(gameData->highScore);

	// For each character of highscore string, get it's corresponding image from the map and draw it
	int scoreTextSize = highScoreStr.size();
	int offset = 0;
	for(std::string::size_type i = 0; i < scoreTextSize; ++i) {
		// convert char to string
		std::stringstream ss;
		ss << highScoreStr[i];
		std::string s;
		ss >> s; // s is a key in strToImageMap
		glDrawSprite(strToImageMap[s], startXPos + offset, highScoreYPos, fontWidth, fontHeight);
		offset += fontWidth;
	}
}

// Draw player's initials next to the highscore
void Hud::drawPlayerInitials(){
	// For each character of initials string, get it's corresponding image from the map and draw it
	int initialsSize = gameData->highScoreInitials.size();
	int offset = 0;
	for(std::string::size_type i = 0; i < initialsSize; ++i) {
		// convert char to string
		std::stringstream ss;
		ss << gameData->highScoreInitials[i];
		std::string s;
		ss >> s; // s is a key in strToImageMap
		glDrawSprite(strToImageMap[s], initialsXPos + offset, highScoreYPos, fontWidth, fontHeight);
		offset += fontWidth;
	}
}

// Draw the total amount of words in the HUD
void Hud::drawTotalCount(){
	//convert the total word count to a string
	std::string totalCountStr = int_to_string(gameData->totalCount);

	// For each character of highscore string, get it's corresponding image from the map and draw it
	int textSize = totalCountStr.size();
	int offset = 0;
	for(std::string::size_type i = 0; i < textSize; ++i) {
		// convert char to string
		std::stringstream ss;
		ss << totalCountStr[i];
		std::string s;
		ss >> s; // s is a key in strToImageMap
		glDrawSprite(strToImageMap[s], totalStartXPos + offset, totalWordsYPos, fontWidth, fontHeight);
		offset += fontWidth;
	}
}

// Draw the number of correctly typed words in the HUD
void Hud::drawCorrectCount(){
	//convert the correct word count to a string
	std::string correctCountStr = int_to_string(gameData->correctCount);

	// For each character of highscore string, get it's corresponding image from the map and draw it
	int textSize = correctCountStr.size();
	int offset = 0;
	for(std::string::size_type i = 0; i < textSize; ++i) {
		// convert char to string
		std::stringstream ss;
		ss << correctCountStr[i];
		std::string s;
		ss >> s; // s is a key in strToImageMap
		glDrawSprite(strToImageMap[s], correctStartXPos + offset, correctYPos, fontWidth, fontHeight);
		offset += fontWidth;
	}
}

void Hud::update(){

}

std::string Hud::int_to_string(int n){
	std::ostringstream outstr;
	outstr << n;
	return outstr.str();
}