#ifndef SELECTION_H
#define SELECTION_H

#include<string>
#include<GL/glew.h>
#include"DrawUtils.h"

/*
* Selection represents a menu selection
*/

class Selection{
public:
	Selection(std::string selection, int, int, int, int, bool);
	inline Selection(){}; //will never be used
	GLuint selectedImage;
	int xMin, xMax; 
	int yMin, yMax;
	int width, height;
	std::string categoryName;
	bool selected;
	void draw();
};
#endif