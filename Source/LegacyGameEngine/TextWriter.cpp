#include"TextWriter.h"

TextWriter::TextWriter(std::string s, std::map<std::string, GLuint>& tMap):strToImageMap(tMap){
	text = s;
	fontWidth = 23;
	fontHeight = 25;
	cursorWidth = 20;
	cursorHeight = 2;
	xPosition = 25;
	yPosition = 540;
	yPositionCursor = yPosition + 30;
	offset = 0;

	//******* Cursor Animation **********************************************************
	cursorframeInfo.push_back(AnimFrameData(0, 6)); // Animation #0 
	cursorImages.push_back(glTexImageTGAFile("../../Resources/images/cursor01.tga", &cursorWidth, &cursorHeight)); // #0
	cursorImages.push_back(glTexImageTGAFile("../../Resources/images/cursor02.tga", &cursorWidth, &cursorHeight));
	cursorImages.push_back(glTexImageTGAFile("../../Resources/images/cursor03.tga", &cursorWidth, &cursorHeight));
	cursorImages.push_back(glTexImageTGAFile("../../Resources/images/cursor04.tga", &cursorWidth, &cursorHeight));
	cursorImages.push_back(glTexImageTGAFile("../../Resources/images/cursor05.tga", &cursorWidth, &cursorHeight));
	cursorImages.push_back(glTexImageTGAFile("../../Resources/images/cursor06.tga", &cursorWidth, &cursorHeight));
	cursorAnimData = AnimationData(cursorImages, cursorframeInfo);
	cursor = new Cursor(cursorAnimData);
}

void TextWriter::draw(){
	cursor->draw(xPosition + offset, yPositionCursor);
	int textSize = text.size();
	// For each character of text, get it's corresponding image from the map and draw it
	offset = 0;
	for(std::string::size_type i = 0; i < textSize; ++i) {
		// convert char to string
		std::stringstream ss;
		ss << text[i];
		std::string s;
		ss >> s; // s is a key in strToImageMap
		glDrawSprite(strToImageMap[s],  xPosition + offset, yPosition, fontWidth, fontHeight);
		offset += fontWidth;
	}
}

void TextWriter::update(std::string s){
	//.001 is avg deltaTime
	cursor->update(.001);
	text = s;
}

std::string TextWriter::to_string() const{
	std::ostringstream oss;

	oss << "text = " << text << "\n";
	return oss.str();
}