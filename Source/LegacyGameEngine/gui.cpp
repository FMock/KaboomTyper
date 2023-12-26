#include"gui.h"

typedef Game_Data GD;

GUI::GUI(KeyStates& k, Game_Data& g):keyStates(&k),
									 gameData(&g),
									 width(20),
									 height(20),
									 options(false),
									 menuItemSelected("default"),
									 selectedIndex(0),
									 playMusicOptionCount(0){
	loadImages();
	gameData->menuItemSelected = menuItemSelected;
	buildSelections();
	musicOptionSelection = Selection("playMusicOption", gameData->PLAY_MUSIC_OPTION_MIN_X, 
														gameData->PLAY_MUSIC_OPTION_MAX_X, 
														gameData->PLAY_MUSIC_OPTION_MIN_Y, 
														gameData->PLAY_MUSIC_OPTION_MAX_Y, true);
}

// Fills vector with Selection objects for category menu itmes
void GUI::buildSelections(){
	selectionItems.push_back(Selection("default", 745, 767, 286, 303, true));
	selectionItems.push_back(Selection("animals", 745, 767, 312, 329, false));
	selectionItems.push_back(Selection("places", 745, 767, 339, 356, false));
	selectionItems.push_back(Selection("things", 745, 767, 364, 381, false));
	selectionItems.push_back(Selection("transportation", 745, 767, 390, 407, false));
	selectionItems.push_back(Selection("music", 745, 767, 416, 433, false));
	selectionItems.push_back(Selection("anime", 745, 767, 440, 457, false));
}

void GUI::loadImages(){
	logo = glTexImageTGAFile("../../Resources/images/logo.tga", &width, &height);
	vertBorder = glTexImageTGAFile("../../Resources/images/vertBorder.tga", &width, &height);
	horzBorder = glTexImageTGAFile("../../Resources/images/horzBorder.tga", &width, &height);
	cloudBackground = glTexImageTGAFile("../../Resources/images/cloud_background.tga", &width, &height);
	textTypeArea = glTexImageTGAFile("../../Resources/images/textTypeArea.tga", &width, &height);
	optionsClosed = glTexImageTGAFile("../../Resources/images/options.tga", &width, &height);
	optionsOpen = glTexImageTGAFile("../../Resources/images/options_open.tga", &width, &height);
	helpClosed = glTexImageTGAFile("../../Resources/images/help.tga", &width, &height);
	helpOpen = glTexImageTGAFile("../../Resources/images/help_open.tga", &width, &height);
	helpMenu = glTexImageTGAFile("../../Resources/images/help_instructions.tga", &width, &height);
	aboutClosed = glTexImageTGAFile("../../Resources/images/about.tga", &width, &height);
	aboutOpen = glTexImageTGAFile("../../Resources/images/about_open.tga", &width, &height);
	marker_left = glTexImageTGAFile("../../Resources/images/marker_left.tga", &width, &height);
	marker_right = glTexImageTGAFile("../../Resources/images/marker_right.tga", &width, &height);
}

void GUI::update(float deltaTime){
	options = keyStates->getOptionButtonPressed();

	for(int i = 0; i < selectionItems.size(); i++){
		if(selectionItems.at(i).selected)
			menuItemSelected = selectionItems.at(i).categoryName;
	}

	// If options are being shown, check/change menu selection
	if(keyStates->optionsDisplayed){
		if(keyStates->mouseClicked){
			// Check if Music Play option has been selected
			if(keyStates->mouseX > GD::PLAY_MUSIC_OPTION_MIN_X && 
			   keyStates->mouseX < GD::PLAY_MUSIC_OPTION_MAX_X && 
			   keyStates->mouseY > GD::PLAY_MUSIC_OPTION_MIN_Y && 
			   keyStates->mouseY < GD::PLAY_MUSIC_OPTION_MAX_Y){
				   gameData->playMenuSelectionSound = true; // main will respond by playing sound
				   playMusicOptionCount += 1;
				   gameData->playMusicOptionCount += 1;

				   // PlayMusic? odd count is false, even count is true
				   playMusicOptionCount % 2 == 0 ? gameData->playMusic = true : gameData->playMusic = false;
			}

			// Check each word category selection options
			for(int i = 0; i < selectionItems.size(); i++){
				if(keyStates->mouseX >= selectionItems.at(i).xMin &&
					keyStates->mouseX <= selectionItems.at(i).xMax &&
					keyStates->mouseY >= selectionItems.at(i).yMin &&
					keyStates->mouseY <= selectionItems.at(i).yMax){
					
					    // A menu item was clicked
						if(i != selectedIndex){
							gameData->playMenuSelectionSound = true; // main will respond by playing sound
							selectionItems.at(i).selected = true;
							selectionItems.at(selectedIndex).selected = false;
							selectedIndex = i;
							menuItemSelected = selectionItems.at(i).categoryName;
							gameData->menuItemSelected = menuItemSelected;
						}
				}
			}
		}
	} // End check/change menu selection
}
	
void GUI::draw(){

	glDrawSprite(logo, 520, 20, 260, 55); // logo is 260 Wide X 55 High

	// Options Menu
	if(keyStates->optionButtonPressedCount % 2 == 0){ // options closed
		if(keyStates->aboutButtonPressedCount % 2 != 0){
			glDrawSprite(aboutOpen, 520, 300, 260, 250);
			glDrawSprite(helpClosed, 520, 260, 260, 40);
		}else{
			// About menu
			glDrawSprite(aboutClosed, 520, 300, 260, 40); // options and about are closed
			// If help Displayed show help screen
			if(keyStates->helpDisplayed){
				glDrawSprite(helpOpen, 520, 260, 260, 40);
				glDrawSprite(helpMenu, 20, 20, 480, 480);
			}else{
				// Help menu
				glDrawSprite(helpClosed, 520, 260, 260, 40);
			}
		}

		glDrawSprite(optionsClosed, 520, 220, 260, 40);

	}else{
		glDrawSprite(optionsOpen, 520, 220, 260, 300);
	}


	if(!keyStates->helpDisplayed) //show background if help instructions aren't displayed
		glDrawSprite(cloudBackground,  0, 0, 500, 500);
	glDrawSprite(marker_left, 20, 80, 10, 17); // marker is 10 wide x 17 high
	glDrawSprite(marker_right, 490, 80, 10, 17); // marker is 10 wide x 17 high
	glDrawSprite(vertBorder,  0, 0, GD::BORDER_WIDTH, GD::WINDOW_WIDTH); // Far left
	glDrawSprite(vertBorder,  500, 0, GD::BORDER_WIDTH, GD::WINDOW_WIDTH); // Left edge menu
	glDrawSprite(vertBorder,  780, 0, GD::BORDER_WIDTH, GD::WINDOW_WIDTH); // Far right
	glDrawSprite(horzBorder,  0, 0, GD::WINDOW_WIDTH, GD::BORDER_WIDTH); // Upper
	glDrawSprite(horzBorder,  0, 500, GD::BLOCK_AREA_WIDTH, GD::BORDER_WIDTH); // Block Floor
	glDrawSprite(horzBorder,  0, 580, GD::WINDOW_WIDTH, GD::BORDER_WIDTH); // Lower
	glDrawSprite(textTypeArea,  20, 520, GD::WINDOW_WIDTH - 40, 60); // text typing area background

	// Only draw the menu selections if the options are being displayed
	if(keyStates->optionsDisplayed){

		if(playMusicOptionCount % 2 == 0)
			musicOptionSelection.draw(); // music play/no-play menu option

		// Word category selection options
		for(int i = 0; i < selectionItems.size(); i++){
			selectionItems.at(i).draw();
		}
	}
}

void GUI::setOptions(bool b){
	options = b;
}

bool GUI::getOptions() const{
	return options;
}

std::string GUI::to_string() const{
	ostringstream oss;
	oss << "GUI ************\n"
		<< "menuItemSelected = " << menuItemSelected << "\n"
		<< "options = " << options << "\n";
	return oss.str();
}