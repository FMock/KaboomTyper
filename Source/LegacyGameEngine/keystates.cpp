#include"keystates.h"

/*
	Track Keyboard Presses and Mouse Position
*/

typedef Game_Data GD;
using std::ostringstream;

KeyStates::KeyStates(Game_Data& gd):gameData(&gd),
									optionButtonPressed(false),
									optionButtonPressedCount(0),
									optionsDisplayed(false),
									aboutButtonPressed(false),
									aboutButtonPressedCount(0),
									aboutDisplayed(false),
									helpButtonPressed(false),
									helpButtonPressedCount(0),
									helpDisplayed(false),
									mouseClicked(false),
									mouseClickedCount(0),
									mouseX(0),
									mouseY(0)
{
	std::fill_n(states, 128, 0);
	gameData->optionsDisplayed = this->optionsDisplayed;
	gameData->aboutDisplayed = this->aboutDisplayed;
	gameData->helpDisplayed = this->helpDisplayed;
}

// Resets key and mouse states to default initial default state
void KeyStates::zeroAllKeyStates(){
	std::fill_n(states, 128, 0);
	optionButtonPressed = false;
	aboutButtonPressed = false;
	helpButtonPressed = false;
	mouseClicked = false;
	gameData->mouseClicked = false;
}

/*
	Sets the state for any key pressed to 1 in the states array
*/
void KeyStates::setKeyPressed(){

	SDL_Keymod modstates = SDL_GetModState(); // to check for modifier keys like shift or ctrl

	SDL_Event event;
	while (SDL_PollEvent(&event)) {

		switch (event.type) {

		case SDL_MOUSEBUTTONDOWN:
			if(event.button.button == SDL_BUTTON_LEFT){

				// Mouse was clicked
				mouseClicked = true;
				mouseClickedCount += 1;
				gameData->mouseClicked = true;

				// Determine where the player clicked
				mouseX = event.button.x;
				mouseY = event.button.y;
				gameData->mouseX = mouseX;
				gameData->mouseY = mouseY;
				//printf("xPos = %d\n", mouseX);
				//printf("yPos = %d\n", mouseY);

				// Options Button Pressed
				if(mouseX > GD::OPTION_BUTTON_MIN_X && mouseX < GD::OPTION_BUTTON_MAX_X && 
					mouseY > GD::OPTION_BUTTON_MIN_Y && mouseY < GD::OPTION_BUTTON_MAX_Y &&
					!helpDisplayed && !aboutDisplayed){
					optionButtonPressed = true;
					gameData->playMenuSelectionSound = true; // main will respond by playing sound
					optionButtonPressedCount += 1; // odd numbers correspond to open and even to close

					optionButtonPressedCount % 2 == 0 ? optionsDisplayed = false : optionsDisplayed = true;

					gameData->optionsDisplayed = this->optionsDisplayed;
				}

				// About Button Pressed
				if(mouseX > GD::ABOUT_BUTTON_MIN_X && mouseX < GD::ABOUT_BUTTON_MAX_X &&
					mouseY > GD::ABOUT_BUTTON_MIN_Y && mouseY < GD::ABOUT_BUTTON_MAX_Y && 
					!optionsDisplayed && !helpDisplayed){
						aboutButtonPressed = true;
						gameData->playMenuSelectionSound = true; // main will respond by playing sound
						aboutButtonPressedCount += 1; // odd numbers correspond to open and even to close
						aboutButtonPressedCount % 2 == 0 ? aboutDisplayed = false : aboutDisplayed = true;

						gameData->aboutDisplayed = this->aboutDisplayed;
				}

				// Help Button Pressed
				if(mouseX > GD::HELP_BUTTON_MIN_X && mouseX < GD::HELP_BUTTON_MAX_X &&
					mouseY > GD::HELP_BUTTON_MIN_Y && mouseY < GD::HELP_BUTTON_MAX_Y && 
					!optionsDisplayed && !aboutDisplayed){
						helpButtonPressed = true;
						gameData->playMenuSelectionSound = true; // main will respond by playing sound
						helpButtonPressedCount += 1; // odd numbers correspond to open and even to close
						helpButtonPressedCount % 2 == 0 ? helpDisplayed = false : helpDisplayed = true;

						gameData->helpDisplayed = this->helpDisplayed;
				}
			}
			break;
			
		case SDL_KEYUP:
			switch(event.key.keysym.sym){
			case SDLK_BACKSPACE:
				states[SDLK_BACKSPACE] += 1;
				break;
			case SDLK_RETURN:
				states[SDLK_RETURN] += 1;
				break;
			case SDLK_SPACE:
				states[SDLK_SPACE] += 1;
				break;
			case SDLK_COMMA:
				states[SDLK_COMMA] += 1;
				break;
			case SDLK_PERIOD:
				states[SDLK_PERIOD] += 1;
				break;
			case SDLK_EQUALS:
				if(modstates & KMOD_SHIFT)
					states[SDLK_PLUS] += 1; // +
				else
					states[SDLK_EQUALS] += 1; // =
				break;
			case SDLK_MINUS:
				states[SDLK_MINUS] += 1; // -
				break;
			case SDLK_DOLLAR:
				states[SDLK_DOLLAR] += 1;
				break;
			case SDLK_SLASH:
				if(modstates & KMOD_SHIFT)
					states[SDLK_QUESTION] += 1; // ?
				else
					states[SDLK_SLASH] += 1; // /
				break;
			case SDLK_0:
				states[SDLK_0] += 1;
				break;
			case SDLK_1:
				if(modstates & KMOD_SHIFT)
					states[SDLK_EXCLAIM] += 1; // !
				else
					states[SDLK_1] += 1; // 1
				break;
			case SDLK_2:
				states[SDLK_2] += 1;
				break;
			case SDLK_3:
				if(modstates & KMOD_SHIFT)
					states[SDLK_HASH] += 1; // #
				else
					states[SDLK_3] += 1; // 3
				break;
			case SDLK_4:
				if(modstates & KMOD_SHIFT)
					states[SDLK_DOLLAR] += 1; // $
				else
					states[SDLK_4] += 1; // 4
				break;
			case SDLK_5:
				if(modstates & KMOD_SHIFT)
					states[SDLK_PERCENT] += 1; // %
				else
					states[SDLK_5] += 1; // 5
				break;
			case SDLK_6:
				states[SDLK_6] += 1;
				break;
			case SDLK_7:
				states[SDLK_7] += 1;
				break;
			case SDLK_8:
				states[SDLK_8] += 1;
				break;
			case SDLK_9:
				states[SDLK_9] += 1;
				break;
			case SDLK_a:
				states[SDLK_a] += 1;
				break;
			case SDLK_b:
				states[SDLK_b] += 1;
				break;
			case SDLK_c:
				states[SDLK_c] += 1;
				break;
			case SDLK_d:
				states[SDLK_d] += 1;
				break;
			case SDLK_e:
				states[SDLK_e] += 1;
				break;
			case SDLK_f:
				states[SDLK_f] += 1;
				break;
			case SDLK_g:
				states[SDLK_g] += 1;
				break;
			case SDLK_h:
				states[SDLK_h] += 1;
				break;
			case SDLK_i:
				states[SDLK_i] += 1;
				break;
			case SDLK_j:
				states[SDLK_j] += 1;
				break;
			case SDLK_k:
				states[SDLK_k] += 1;
				break;
			case SDLK_l:
				states[SDLK_l] += 1;
				break;
			case SDLK_m:
				states[SDLK_m] += 1;
				break;
			case SDLK_n:
				states[SDLK_n] += 1;
				break;
			case SDLK_o:
				states[SDLK_o] += 1;
				break;
			case SDLK_p:
				states[SDLK_p] += 1;
				break;
			case SDLK_q:
				states[SDLK_q] += 1;
				break;
			case SDLK_r:
				states[SDLK_r] += 1;
				break;
			case SDLK_s:
				states[SDLK_s] += 1;
				break;
			case SDLK_t:
				states[SDLK_t] += 1;
				break;
			case SDLK_u:
				states[SDLK_u] += 1;
				break;
			case SDLK_v:
				states[SDLK_v] += 1;
				break;
			case SDLK_w:
				states[SDLK_w] += 1;
				break;
			case SDLK_x:
				states[SDLK_x] += 1;
				break;
			case SDLK_y:
				states[SDLK_y] += 1;
				break;
			case SDLK_z:
				states[SDLK_z] += 1;
				break;
			}
			break;
		}
	}
}


bool KeyStates::getOptionButtonPressed(){
	return optionButtonPressed;
}

std::string KeyStates::to_string() const{
	ostringstream oss;

	for(int i = 0; i < NUM_KEYS; i++){
		if(states[i] == 1){
			oss <<  states[i] << "\n";
		}
	}

	oss << "*************************\n"
		<< "optionButtonPressed = " << optionButtonPressed << "\n"
		<< "optionButtonPressedCount = " << optionButtonPressedCount << "\n"
		<< "optionsDisplayed = " << optionsDisplayed << "\n"
		<< "aboutButtonPressed = " << aboutButtonPressed << "\n"
		<< "aboutButtonPressedCount = " << aboutButtonPressedCount << "\n"
		<< "aboutDisplayed = " << aboutDisplayed << "\n"
		<< "helpButtonPressed = " << helpButtonPressed << "\n"
		<< "helpButtonPressedCount = " << helpButtonPressedCount << "\n"
		<< "helpDisplayed = " << helpDisplayed << "\n"
		<< "mouseClicked = " << mouseClicked << "\n"
		<< "mouseClickedCount = " << mouseClickedCount << "\n"
		<< "mouseX = " << mouseX << "\n"
		<< "mouseY = " << mouseY << "\n";
	return oss.str();
}


