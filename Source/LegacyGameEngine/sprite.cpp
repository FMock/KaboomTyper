#include "Sprite.h"
#include "game_data.h"

using std::string;
using std::ostringstream;

typedef Game_Data GD;

Sprite::Sprite(){
	characterSize = new int[2]();
	characterPos = new float[2]();
	characterSize[0] = 32;
	characterSize[1] = 64;
	setXPos(0);
	setYPos(0);
	change_x = 0;
	change_y = 0;
	// speed is Pixels/Sec
	speedX = 100.0;
	speedY = 100.0;
	type = new char[20];
	type = "sprite";
}

Sprite::Sprite(int x, int y, int w, int h){
	image = NULL;
	characterSize = new int[2]();
	characterPos = new float[2]();
	characterSize[0] = w;
	characterSize[1] = h;
	box = AABB();
	box.setX(x);
	box.setY(y);
	box.setW(w);
	box.setH(h);
	setXPos(x);
	setYPos(y);
	change_x = 0;
	change_y = 0;
	// speed is Pixels/Sec
	speedX = 100.0;
	speedY = 100.0;
	type = new char[20];
	type = "sprite";
}

//Sprite::Sprite(std::string imagePath, int x, int y, int xPos, int yPos){
Sprite::Sprite(std::string imagePath, int x, int y){
	box = AABB();
	box.setX(x);
	box.setY(y);
	box.setW(32);
	box.setH(64);
	characterSize = new int[2]();
	characterPos = new float[2]();
	image = glTexImageTGAFile(imagePath.c_str(), &characterSize[0], &characterSize[1]);
	setXPos(x);
	setYPos(y);
	change_x = 0;
	change_y = 0;
	// speed is Pixels/Sec
	speedX = 0.0;
	speedY = 0.0;
	type = new char[20];
	type = "sprite";
}

Sprite::~Sprite(){

}

void Sprite::setXPos(int x){
	characterPos[0] = x;
	box.setX(abs(characterPos[0]));
}

int Sprite::getXPos() const{
	int x = abs(characterPos[0]);
	return x;
}

void Sprite::setYPos(int y){
	characterPos[1] = y;
	box.setY(abs(characterPos[1]));
}

int Sprite::getYPos() const{
	return abs(characterPos[1]);
}

void Sprite::setWidth(int w){
	characterSize[0] = w;
	box.setW(w);
}

int Sprite::getWidth() const{
	return characterSize[0];
}

void Sprite::setHeight(int h){
	characterSize[1] = h;
	box.setH(h);
}

int Sprite::getHeight() const{
	return characterSize[1];
}

void Sprite::setXSpeed(int s){
	this->speedX = s;
}

void Sprite::setYSpeed(int s){
	this->speedY = s;
}

void Sprite::draw(){
	glDrawSprite(image, characterPos[0], characterPos[1], characterSize[0], characterSize[1]);
}

void Sprite::update(float deltaTime){
	if(characterPos[0] > GD::WORLD_WIDTH - characterSize[0]){
		change_x = 0; //stop player from going out of bounds right
		characterPos[0] -= 1;
	}

	if(characterPos[0] < 0){
		change_x = 0; //stop player from going out of bounds left
		characterPos[0] += 1;
	}

	if(characterPos[1] < 0){
		change_y = 0; //stop player from going out of bounds up
		characterPos[1] += 1;
	}

	if(characterPos[1] > GD::WORLD_HEIGHT - characterSize[1]){
		change_y = 0; //stop player from going out of bounds up
		characterPos[1] -= 1;
	}

	characterPos[0] += change_x * deltaTime;
	box.setX(abs(characterPos[0]));
	characterPos[1] += change_y * deltaTime;
	box.setY(abs(characterPos[1]));
}

void Sprite::moveLeft(){
	change_y = 0; 
	change_x = -speedX;
}

void Sprite::moveRight(){
	change_y = 0; 
	change_x = speedX; 
}

void Sprite::moveUp(){
	change_x = 0;
	change_y = -speedY;
}

void Sprite::moveDown(){
	change_x = 0;
	change_y = speedY;
}

void Sprite::stop(){
	change_x = 0;
	change_y = 0;
}

AABB& Sprite::getBox(){
	return box;
}

string Sprite::to_string() const{
	ostringstream oss;
	oss << "Sprite\n"
		<< "AABB x = " << box.x << "\n"
		<< "AABB y = " << box.y << "\n"
		<< "AABB w = " << box.w << "\n"
		<< "AABB h = " << box.h << "\n"
		<< "xPos = " << getXPos() << "\n"
		<< "YPos = " << getYPos() << "\n"
		<< "speedX" << speedX << "\n"
		<< "speedY" << speedX << "\n";
	return oss.str();
}