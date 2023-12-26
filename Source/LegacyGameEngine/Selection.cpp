#include "selection.h"

Selection::Selection(std::string name, int xMin, int xMax, int yMin, int yMax, bool s):
					    categoryName(name), 
						xMin(xMin), xMax(xMax), 
						yMin(yMin), yMax(yMax), 
						selected(s),
						width(20),
						height(15){
						
	selectedImage = glTexImageTGAFile("../../Resources/images/selected.tga", &width, &height);
}

/* Only Draw selection if selection flag is true
 * GUI will set/unset selection flag
 */
void Selection::draw(){
	int xPad = 1;
	int yPad = 2;
	if(selected)
		glDrawSprite(selectedImage,  xMin + xPad, yMin + yPad, width, height);
}