#define SDL_MAIN_HANDLED
#include<SDL.h>
#include<GL/glew.h>
#include<stdio.h>
#include<string>
#include<assert.h>
#include"DrawUtils.h"
#include"game_data.h"
#include"sprite.h"
#include"keystates.h"
#include"TextBlock.h"
#include<fmod.hpp> // For Sound Functionality
#include<time.h>
#include"timer.h"
#include<vector>
#include<iostream>
#include<fstream>
#include"gui.h"
#include"TextWriter.h"
#include"stats.h"
#include"hud.h"
#include"hit.h"

/*
	main.cpp
	
	Program Name - SpeedTyper
	Author - Frank Mock
	Project Start Date - 06/01/2017
	First Draft Finish - 07/01/2017
*/

typedef Game_Data GD;

Game_Data *gameData;

// Set this to true to make the game loop exit.
char shouldExit = 0;

// The previous frame's keyboard state.
unsigned char kbPrevState[SDL_NUM_SCANCODES] = { 0 };

// The current frame's keyboard state.
const unsigned char* kbState = NULL;

// To get keyboard state via SDL
const Uint8 *keyState;

// tracks key states
KeyStates *keyStates;

// Sprite Amount Constants
const int NUM_BLOCKS = 24;

// Containers
std::vector<TextBlock> blocks = std::vector<TextBlock>();
std::vector<std::string> words = std::vector<std::string>();
std::map<std::string, GLuint> stringToImageMap;

// Containers for hit animations
std::vector<GLuint> imagesHit;
std::vector<AnimFrameData> frameInfoHit;
AnimationData animDataHit; //Hit animation data
int animWidth = 64;
int animHeight = 64;

// Container for hit objects
std::vector<Hit *> hits = std::vector<Hit *>();

int font_width = 30;
int font_height = 30;

// Objects
GUI *gui;
Stats stats;
Hud hud;
TextWriter *textWriter;
int overlayWidth = 500;
int overlayHeight = 200;
GLuint gameOverOverlay;
GLuint playing;
GLuint pausedImg;
GLuint startImg;
GLuint newHighScore;
GLuint end_game_instructions;
GLuint get_ready;
GLuint begin;

// To regulate frame rate
int previousTime = 0;
int currentTime = 0;
float deltaTime = 0.0f;

int wordCount = 0;
std::ifstream inFile; // for reading files
std::ofstream outFile; // to write highscore
std::string wordsFile = "..\\..\\Config\\words.txt";
std::string highScoreFile = "..\\..\\Config\\high.txt";
std::string testing = "";
std::string highScoreInitials = "";
bool addNewHighScoreInitials = false;

Timer timer;
int blockInterval = 5; // Interval in seconds at which a new block appears

// Function protoTypes
void readWords(std::ifstream &in);
void readLines(std::ifstream &in);
int readHighScore(std::ifstream &in);
void writeHighScore(std::ofstream &out);
int string_to_int(std::string);
bool AABBIntersect(AABB box1, AABB box2);
void createSound(const char *name, FMOD::Sound **sound);

int high; // track highscore
int newHighScoreWidth = 400;
int newHighScoreHeight = 200;
bool start = true;
bool gameOver = false;
bool paused = true;
float previousPausePressTime = 0.0f;
float currentPausePressTime = 0.0f;
bool kaboomUsed = false;
bool showEndGameInstructions = false;
bool showGetReady = false;

static const int Y_POSITION_THRESHOLD = 90;
static int yPosReached = GD::WINDOW_HEIGHT - GD::BL_FLOOR_TO_BOTTOM;

// For sound functionality
FMOD::System *fmod_sys;
FMOD::Sound *scoreSound;
FMOD::Sound *explosion;
FMOD::Sound *lostPoints;
FMOD::Sound *optionSelectSound;
FMOD::Sound *bgMusic;  // for background music
FMOD::Channel *bgChan; // for background music

// category start numbers
int defaultNum = 0;
int animals;
int places;
int things;
int transportation;
int music;
int anime;


int main(void)
{
	// Sound Setup
	FMOD_RESULT fmodResult = System_Create(&fmod_sys);
	fmod_sys->init(100, FMOD_INIT_NORMAL, 0);

	// If createSound fails, continue game anyway
	createSound("../../Resources/sounds/score_sound.wav", &scoreSound);
	createSound("../../Resources/sounds/explode.wav", &explosion);
	createSound("../../Resources/sounds/lost_points.wav", &lostPoints);
	createSound("../../Resources/sounds/option_select.wav", &optionSelectSound);

	// Use a stream for background music, set it to loop
	fmod_sys->createStream("../../Resources/sounds/background_music.mp3", FMOD_LOOP_NORMAL, 0, &bgMusic);

	// Initialize SDL.
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fprintf(stderr, "Could not initialize SDL. ErrorCode=%s\n", SDL_GetError());
		return 1;
	}

	// Create the window and OpenGL context.
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_Window* window = SDL_CreateWindow(
		"Kaboom Typer",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		800, 600,
		SDL_WINDOW_OPENGL);
	if (!window) {
		fprintf(stderr, "Could not create window. ErrorCode=%s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	SDL_GL_CreateContext(window);

	// Make sure we have a recent version of OpenGL.
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK) {
		fprintf(stderr, "Could not initialize glew. ErrorCode=%s\n", glewGetErrorString(glewError));
		SDL_Quit();
		return 1;
	}
	if (GLEW_VERSION_2_0) {
		fprintf(stderr, "OpenGL 2.0 or greater supported: Version=%s\n",
			glGetString(GL_VERSION));
	}
	else {
		fprintf(stderr, "OpenGL max supported version is too low.\n");
		SDL_Quit();
		return 1;
	}

	// Setup OpenGL state.
	glViewport(0, 0, GD::WINDOW_WIDTH, GD::WINDOW_HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glOrtho(0, GD::WINDOW_WIDTH, GD::WINDOW_HEIGHT, 0, 0, 100);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//********** Game initialization goes here. **********************

	// Read in the words from text file
	inFile.open(wordsFile);
	if(inFile.fail()){
		printf("Error opening word file %s ", wordsFile.c_str());
		SDL_Quit();
		return 1;
	}
	readLines(inFile);
	inFile.close();

	// Read high score from text file
	inFile.open(highScoreFile);
	if(inFile.fail()){
		printf("Error opening high score file %s ", highScoreFile.c_str());
		SDL_Quit();
		return 1;
	}
	high = readHighScore(inFile);
	inFile.close();


	// **********  LOAD IMAGE RESOURCES ***************************************************
	gameOverOverlay = glTexImageTGAFile("../../Resources/images/endGame.tga", &overlayWidth, &overlayHeight);
	playing = glTexImageTGAFile("../../Resources/images/playing.tga", &overlayWidth, &overlayHeight);
	pausedImg = glTexImageTGAFile("../../Resources/images/paused.tga", &overlayWidth, &overlayHeight);
	startImg = glTexImageTGAFile("../../Resources/images/startImg.tga", &overlayWidth, &overlayHeight);
	newHighScore = glTexImageTGAFile("../../Resources/images/new_high_score.tga", &newHighScoreWidth, &newHighScoreHeight);
	end_game_instructions = glTexImageTGAFile("../../Resources/images/end_game_instructions.tga", &newHighScoreWidth, &newHighScoreHeight);
	get_ready = glTexImageTGAFile("../../Resources/images/get_ready.tga", &newHighScoreWidth, &newHighScoreHeight);
	begin = glTexImageTGAFile("../../Resources/images/begin.tga", &newHighScoreWidth, &newHighScoreHeight);


	//******* Hit Animation **********************************************************
	frameInfoHit.push_back(AnimFrameData(0, 12)); // Animation #0 
	imagesHit.push_back(glTexImageTGAFile("../../Resources/images/hit_00.tga", &animWidth, &animHeight)); // #0
	imagesHit.push_back(glTexImageTGAFile("../../Resources/images/hit_01.tga", &animWidth, &animHeight));
	imagesHit.push_back(glTexImageTGAFile("../../Resources/images/hit_02.tga", &animWidth, &animHeight));
	imagesHit.push_back(glTexImageTGAFile("../../Resources/images/hit_03.tga", &animWidth, &animHeight));
	imagesHit.push_back(glTexImageTGAFile("../../Resources/images/hit_04.tga", &animWidth, &animHeight)); // #4
	imagesHit.push_back(glTexImageTGAFile("../../Resources/images/hit_05.tga", &animWidth, &animHeight));
	imagesHit.push_back(glTexImageTGAFile("../../Resources/images/hit_06.tga", &animWidth, &animHeight));
	imagesHit.push_back(glTexImageTGAFile("../../Resources/images/hit_07.tga", &animWidth, &animHeight));
	imagesHit.push_back(glTexImageTGAFile("../../Resources/images/hit_08.tga", &animWidth, &animHeight));
	imagesHit.push_back(glTexImageTGAFile("../../Resources/images/hit_09.tga", &animWidth, &animHeight));
	imagesHit.push_back(glTexImageTGAFile("../../Resources/images/hit_10.tga", &animWidth, &animHeight));
	imagesHit.push_back(glTexImageTGAFile("../../Resources/images/hit_11.tga", &animWidth, &animHeight));
	animDataHit = AnimationData(imagesHit, frameInfoHit);



	/*Initialize the stringToImageMap
	* TextBlock and TextWriter objects will share this map
	*/
	GLuint red = glTexImageTGAFile("../../Resources/images/red_block.tga", &font_width, &font_height);  // red
	stringToImageMap["red"] = red;
	GLuint blue = glTexImageTGAFile("../../Resources/images/blue_block.tga", &font_width, &font_height);  // blue
	stringToImageMap["blue"] = blue;
	GLuint green = glTexImageTGAFile("../../Resources/images/green_block.tga", &font_width, &font_height);  // green
	stringToImageMap["green"] = green;
	GLuint yellow = glTexImageTGAFile("../../Resources/images/yellow_block.tga", &font_width, &font_height);  // yellow
	stringToImageMap["yellow"] = yellow;
	GLuint purple = glTexImageTGAFile("../../Resources/images/purple_block.tga", &font_width, &font_height);  // purple
	stringToImageMap["purple"] = purple;
	GLuint white = glTexImageTGAFile("../../Resources/images/white_block.tga", &font_width, &font_height);  // white
	stringToImageMap["white"] = white;
	GLuint orange = glTexImageTGAFile("../../Resources/images/orange_block.tga", &font_width, &font_height);  // orange
	stringToImageMap["orange"] = orange;
	GLuint zero = glTexImageTGAFile("../../Resources/images/num0.tga", &font_width,&font_height); // 0
	stringToImageMap["0"] = zero;
	GLuint one = glTexImageTGAFile("../../Resources/images/num1.tga", &font_width,&font_height);  // 1
	stringToImageMap["1"] = one;
	GLuint two = glTexImageTGAFile("../../Resources/images/num2.tga", &font_width,&font_height);  // 2
	stringToImageMap["2"] = two;
	GLuint three = glTexImageTGAFile("../../Resources/images/num3.tga", &font_width,&font_height);  // 3
	stringToImageMap["3"] = three;
	GLuint four = glTexImageTGAFile("../../Resources/images/num4.tga", &font_width,&font_height);  // 4
	stringToImageMap["4"] = four;
	GLuint five = glTexImageTGAFile("../../Resources/images/num5.tga", &font_width,&font_height);  // 5
	stringToImageMap["5"] = five;
	GLuint six = glTexImageTGAFile("../../Resources/images/num6.tga", &font_width,&font_height);  // 6
	stringToImageMap["6"] = six;
	GLuint seven = glTexImageTGAFile("../../Resources/images/num7.tga", &font_width,&font_height);  // 7
	stringToImageMap["7"] = seven;
	GLuint eight = glTexImageTGAFile("../../Resources/images/num8.tga", &font_width,&font_height);  // 8
	stringToImageMap["8"] = eight;
	GLuint nine = glTexImageTGAFile("../../Resources/images/num9.tga", &font_width,&font_height);  // 9
	stringToImageMap["9"] = nine;
	GLuint A = glTexImageTGAFile("../../Resources/images/a.tga", &font_width,&font_height);  // A
	stringToImageMap["A"] = A;
	GLuint B = glTexImageTGAFile("../../Resources/images/b.tga", &font_width,&font_height);  // B
	stringToImageMap["B"] = B;
	GLuint C = glTexImageTGAFile("../../Resources/images/c.tga", &font_width,&font_height);  // C
	stringToImageMap["C"] = C;
	GLuint D = glTexImageTGAFile("../../Resources/images/d.tga", &font_width,&font_height);  // D
	stringToImageMap["D"] = D;
	GLuint E = glTexImageTGAFile("../../Resources/images/e.tga", &font_width,&font_height);  // E
	stringToImageMap["E"] = E;
	GLuint F = glTexImageTGAFile("../../Resources/images/f.tga", &font_width,&font_height);  // F
	stringToImageMap["F"] = F;
	GLuint G = glTexImageTGAFile("../../Resources/images/g.tga", &font_width,&font_height);  // G
	stringToImageMap["G"] = G;
	GLuint H = glTexImageTGAFile("../../Resources/images/h.tga", &font_width,&font_height);  // H
	stringToImageMap["H"] = H;
	GLuint I = glTexImageTGAFile("../../Resources/images/i.tga", &font_width,&font_height);  // I
	stringToImageMap["I"] = I;
	GLuint J = glTexImageTGAFile("../../Resources/images/j.tga", &font_width,&font_height);  // J
	stringToImageMap["J"] = J;
	GLuint K = glTexImageTGAFile("../../Resources/images/k.tga", &font_width,&font_height);  // K
	stringToImageMap["K"] = K;
	GLuint L = glTexImageTGAFile("../../Resources/images/l.tga", &font_width,&font_height);  // L
	stringToImageMap["L"] = L;
	GLuint M = glTexImageTGAFile("../../Resources/images/m.tga", &font_width,&font_height);  // M
	stringToImageMap["M"] = M;
	GLuint N = glTexImageTGAFile("../../Resources/images/n.tga", &font_width,&font_height);  // N
	stringToImageMap["N"] = N;
	GLuint O = glTexImageTGAFile("../../Resources/images/o.tga", &font_width,&font_height);  // O
	stringToImageMap["O"] = O;
	GLuint P = glTexImageTGAFile("../../Resources/images/p.tga", &font_width,&font_height);  // P
	stringToImageMap["P"] = P;
	GLuint Q = glTexImageTGAFile("../../Resources/images/q.tga", &font_width,&font_height);  // Q
	stringToImageMap["Q"] = Q;
	GLuint R = glTexImageTGAFile("../../Resources/images/r.tga", &font_width,&font_height);  // R
	stringToImageMap["R"] = R;
	GLuint S = glTexImageTGAFile("../../Resources/images/s.tga", &font_width,&font_height);  // S
	stringToImageMap["S"] = S;
	GLuint T = glTexImageTGAFile("../../Resources/images/t.tga", &font_width,&font_height);  // T
	stringToImageMap["T"] = T;
	GLuint U = glTexImageTGAFile("../../Resources/images/u.tga", &font_width,&font_height);  // U
	stringToImageMap["U"] = U;
	GLuint V = glTexImageTGAFile("../../Resources/images/v.tga", &font_width,&font_height);  // V
	stringToImageMap["V"] = V;
	GLuint W = glTexImageTGAFile("../../Resources/images/w.tga", &font_width,&font_height);  // W
	stringToImageMap["W"] = W;
	GLuint X = glTexImageTGAFile("../../Resources/images/x.tga", &font_width,&font_height);  // X
	stringToImageMap["X"] = X;
	GLuint Y = glTexImageTGAFile("../../Resources/images/y.tga", &font_width,&font_height);  // Y
	stringToImageMap["Y"] = Y;
	GLuint Z = glTexImageTGAFile("../../Resources/images/z.tga", &font_width,&font_height);  // Z
	stringToImageMap["Z"] = Z;
	GLuint dollar = glTexImageTGAFile("../../Resources/images/dollarsign.tga", &font_width,&font_height);  // $
	stringToImageMap["$"] = dollar;
	GLuint exclamation = glTexImageTGAFile("../../Resources/images/exclamation.tga", &font_width,&font_height);  // !
	stringToImageMap["!"] = exclamation;
	GLuint hashtag = glTexImageTGAFile("../../Resources/images/hashtag.tga", &font_width,&font_height);  // #
	stringToImageMap["#"] = hashtag;
	GLuint minus = glTexImageTGAFile("../../Resources/images/minussign.tga", &font_width,&font_height);  // -
	stringToImageMap["-"] = minus;
	GLuint plus = glTexImageTGAFile("../../Resources/images/plussign.tga", &font_width,&font_height);  // +
	stringToImageMap["+"] = plus;
	GLuint percent = glTexImageTGAFile("../../Resources/images/percentsign.tga", &font_width,&font_height);  // %
	stringToImageMap["%"] = percent;
	GLuint equal = glTexImageTGAFile("../../Resources/images/equal.tga", &font_width,&font_height);  // =
	stringToImageMap["="] = equal;
	GLuint question = glTexImageTGAFile("../../Resources/images/question.tga", &font_width,&font_height);  // ?
	stringToImageMap["?"] = question;
	GLuint space = glTexImageTGAFile("../../Resources/images/space.tga", &font_width,&font_height);  // <blank space>
	stringToImageMap[""] = space;
	GLuint period = glTexImageTGAFile("../../Resources/images/period.tga", &font_width,&font_height);  // .
	stringToImageMap["."] = period;

	gameData = new Game_Data();
	gameData->highScore = high;
	gameData->highScoreInitials = highScoreInitials;
	hud = Hud(stringToImageMap, *gameData);
	stats = Stats(*gameData);
	keyStates = new KeyStates(*gameData);
	textWriter = new TextWriter(testing, stringToImageMap);
	gui = new GUI(*keyStates, *gameData);
	timer = Timer(SDL_GetTicks());

	blocks.reserve(24); // Try to prevent blocks vector resize overhead

	fmod_sys->playSound(bgMusic, 0, false, &bgChan); // Start playing background music

	//********** GAME LOOP *************************************************************
	kbState = SDL_GetKeyboardState(NULL);
	while (!shouldExit) {

		// Find out how much time has past since last loop iteration
		previousTime = currentTime;
		currentTime = SDL_GetTicks();
		deltaTime = (currentTime - previousTime) / 1000.0f;
		gameData->deltaTime = deltaTime;

		//*********** Get Player Input ***************************************
		if(gameData->playMenuSelectionSound){
			fmod_sys->playSound(optionSelectSound, 0, false, NULL);
			gameData->playMenuSelectionSound = false;
		}

		assert(glGetError() == GL_NO_ERROR);
		memcpy(kbPrevState, kbState, sizeof(kbPrevState));

		// Register any keys pressed in tracking array
		keyStates->setKeyPressed();
		

		// Loop through key states array and get key that was pressed
		for(int i = 0; i < 128; i++){
			if(keyStates->states[i] == 1){
				std::string temp = SDL_GetKeyName(i);
				//printf(temp.c_str());
				if(temp.compare("Return") == 0){
					if(!gameData->newHighScore){
						testing = "";
					}else{
						// A new highscore - get players initials
						if(testing.size() < 4){ // Initials entered have length < 4
							highScoreInitials = testing;
							gameData->highScoreInitials = highScoreInitials;
						}else{
							highScoreInitials = testing.substr(0, 3); //Only want three characters
							gameData->highScoreInitials = highScoreInitials;
						}
						testing = "";
						gameData->newHighScore = false;
						addNewHighScoreInitials = true;
						if(!gameData->newHighScore)
							showEndGameInstructions = true;
					}
				}else if(temp.compare("Space") == 0){
					if(testing.size() < GD::MAX_STRING_SIZE)
						testing.append(" ");
				}else if(temp.compare("Backspace") == 0){
					testing = testing.substr(0, testing.size()-1);
				}else{
					if(testing.size() < GD::MAX_STRING_SIZE)
						testing.append(SDL_GetKeyName(i));
				}

				//printf(testing.c_str());
				//printf("\n");
				
			}
		}


		// Take action if any of arrowkeys are pushed
		if (kbState[SDL_SCANCODE_RIGHT]) {
			//printf("Right arrow pressed\n");

			//******* RESET THE GAME ****************************
			// Flag blocks for removal
			for(int i = 0; i < blocks.size(); i++){
				blocks.at(i).remove = true;
			}

			// Remove TextBlocks that have been flagged for removal
			for(int i = 0; i < blocks.size(); i++){
				if(blocks.at(i).remove){
					blocks.erase(blocks.begin() + i);
				}
			}

			stats.reset();
			gameOver = false;
			paused = false;
			start = false;
			gameData->newHighScore = false;
			addNewHighScoreInitials = false;
			showEndGameInstructions = false;

			/* Show 'Get Ready' at beginning when right arrow is pressed
			 * When first TextBlock moves 'Get Ready' will go away */
			showGetReady = true;
		}
		else if (kbState[SDL_SCANCODE_LEFT]) {
			//printf("Left arrow pressed\n");

			/* Show 'Get Ready' at beginning when left arrow is pressed
			 * When first TextBlock moves get ready will go away */
			if(gameData->currentWord.compare("") == 0 && !gameOver)
				showGetReady = true;

			start = false;

			//******** PAUSE THE GAME ***************************

			// Determine how long it's been since left arrow was last pressed
			previousPausePressTime = currentPausePressTime;
			currentPausePressTime = SDL_GetTicks();
			float sinceLastPressed = (currentPausePressTime - previousPausePressTime) / 1000.0f;

			if(sinceLastPressed > 0.02){

				(paused && !gameData->helpDisplayed) ? paused = false : paused = true;

			}
		}
		else if (kbState[SDL_SCANCODE_UP]) {
			//printf("Up arrow pressed\n");
		}
		else if (kbState[SDL_SCANCODE_DOWN]) {
			//printf("Down arrow pressed\n");
		}else if(kbState[SDL_SCANCODE_F1]){
			//printf("F1 pressed\n");
		}
		else{
			//textBlock->stop();
		}

		// Game logic goes here.
		if (kbState[SDL_SCANCODE_ESCAPE]) {
			shouldExit = 1;
		}

		// ************* Do Updates  **************************************************
		fmod_sys->update(); // If you don't update the sound will play once

		gui->update(deltaTime);

		if(!paused){
			textWriter->update(testing);

			int var = rand() % 1001 + 1; // just to add further variation in word choices

			// Create a TextBlock every 4 seconds. blockInterval = 4
			if(timer.count == blockInterval && blocks.size() < NUM_BLOCKS && !gameOver){
				srand (SDL_GetTicks() + var);
				int xpos = rand() % GD::BLOCK_FALL_AREA_MAX_X + GD::BORDER_WIDTH;
				int ypos = 0;
				int i;
				int numWords; // number words in category
				std::string category = gameData->menuItemSelected;

				if(category.compare("animals") == 0){
					numWords = places - animals;
					i = rand() % numWords + animals; // animals category
				}else if(category.compare("places") == 0){
					numWords = things - places;
					i = rand() % numWords + places; // places category
				}else if(category.compare("things") == 0){
					numWords = transportation - things;
					i = rand() % numWords + things; // things category
				}else if(category.compare("transportation") == 0){
					numWords = music - transportation;
					i = rand() % numWords + transportation; // transportation category
				}else if(category.compare("music") == 0){
					numWords = anime - music;
					i = rand() % numWords + music; // music category
				}else if(category.compare("anime") == 0){
					numWords = wordCount - anime;
					i = rand() % numWords + anime; // anime category
				}
				else{
					i = rand() % wordCount; // default category == all the words
				}

				blocks.push_back(TextBlock(xpos, ypos, 30, 30, words.at(i), stringToImageMap));
				gameData->currentWord = words.at(i);
				stats.increaseTotalCount(1); // Keep track of the amount of words presented to player
			}

			if(timer.count == blockInterval)
				timer.reSet();

			timer.update(deltaTime);

			/*****  Update hits  *****/
			for(int i = 0; i < hits.size(); i++){
				hits.at(i)->updateAnim(deltaTime);
			}

			// update TextBlocks
			for(int i = 0; i < blocks.size(); i++){

				// if not flaged for removal, update
				if(!blocks.at(i).remove){
					blocks.at(i).update(deltaTime);
				}

				// If a block is moving, check for collisons
				if(blocks.at(i).moving){
					showGetReady = false;

					// Check for collisons with blocks in lower index
					if(i > 0){
						for(int j =  i - 1; j >= 0; j--){
							if(AABBIntersect(blocks.at(i).getBox(), blocks.at(j).getBox())){
								blocks.at(i).moving = false;
							}
						}
					}

					// check if player typed the correct string
					if(!blocks.at(i).remove && blocks.at(i).text.compare(testing) == 0){
						blocks.at(i).remove = true;
						fmod_sys->playSound(scoreSound, 0, false, NULL);
						fmod_sys->playSound(explosion, 0, false, NULL);

						// create a hit animation object ( A blowup animation )
						hits.push_back(new Hit(blocks.at(i).getXPos(), blocks.at(i).getYPos(), 64, 64, animDataHit, 0));
						testing = ""; // clear the letters typed
					}

					// Check if player typed 'KABOOM' to blowup a word (remove a word)
					if(testing.compare("KABOOM") == 0){
						// create a hit animation object ( A blowup animation )
						hits.push_back(new Hit(blocks.at(i).getXPos(), blocks.at(i).getYPos(), 64, 64, animDataHit, 0));
						kaboomUsed = true;
						blocks.at(i).remove = true;
						fmod_sys->playSound(lostPoints, 0, false, NULL);
						fmod_sys->playSound(explosion, 0, false, NULL);
						testing = ""; // clear the letters typed
					}
				}
			}


			// Get the Y position of the highest stacked block
			int lowestYPosition = GD::WINDOW_HEIGHT;
			for(int i = 0; i < blocks.size(); i++){
				if(!blocks.at(i).moving && blocks.at(i).getYPos() < lowestYPosition)
					lowestYPosition = blocks.at(i).getYPos();
			}

			yPosReached = lowestYPosition;
			// GAME OVER CONDITION - Heighest stacked block at or above threshold
			if(!gameOver &&  yPosReached <= Y_POSITION_THRESHOLD){
				gameOver = true;
			}

			// GAME OVER CONDITION - Amount of TextBlocks displayed == NUM_BLOCKS
			if(blocks.size() == NUM_BLOCKS && !gameOver && !blocks.at(NUM_BLOCKS - 1).moving){
				gameOver = true;
			}
		}// END !PAUSED


		//****** UPDATE HIGHSCORE FILE ***********************/
		if(gameOver){
			if(gameData->score > high){
				gameData->highScore = gameData->score;
				gameData->newHighScore = true;

				outFile.open(highScoreFile);
				if(outFile.fail()){
					printf("Error opening high score file %s ", highScoreFile.c_str());
					SDL_Quit();
					return 1;
				}

				writeHighScore(outFile);
				outFile.close();
			}
		}
		
		//*********** Drawing **********************************************************
		glClearColor(0, 0, 0, 1);  
		glClear(GL_COLOR_BUFFER_BIT); // Be sure to always draw objects after this

		if(paused){
			if(start){
				// Shown at start of a new game
				glDrawSprite(startImg, GD::HUD_START_X, GD::GAME_STATUS_Y, overlayWidth, overlayHeight);
			}else{
				glDrawSprite(pausedImg, GD::HUD_START_X, GD::GAME_STATUS_Y, overlayWidth, overlayHeight);
			}
		}
		else if(gameOver){
			glDrawSprite(gameOverOverlay, GD::HUD_START_X, GD::GAME_STATUS_Y, overlayWidth, overlayHeight);
		}else{
			glDrawSprite(playing, GD::HUD_START_X, GD::GAME_STATUS_Y, overlayWidth, overlayHeight);
		}

		gui->draw();

		// Show at the beginning of the game
		if(paused && start && !gameData->helpDisplayed){
			glDrawSprite(begin, 60, 200, newHighScoreWidth, newHighScoreHeight);
		}

		hud.draw();
		textWriter->draw();


		if(!gameData->helpDisplayed){ //Don't want textBlocks to obstruct the help instructions
			for(int i = 0; i < blocks.size(); i++){
				blocks.at(i).draw();
			}
		}

		// Should only be set at beginning after right arrow key pressed, but
		// before textBlock is shown
		if(showGetReady)
			glDrawSprite(get_ready, 60, 200, newHighScoreWidth, newHighScoreHeight);

		if(gameOver && gameData->newHighScore){
			//if new high score show the newHighScore image
			glDrawSprite(newHighScore, 60, 200, newHighScoreWidth, newHighScoreHeight);
			if(showEndGameInstructions){
				glDrawSprite(end_game_instructions, 60, 200, newHighScoreWidth, newHighScoreHeight);
			}
		}

		/*****  Draw hits  *****/
		for(int i = 0; i < hits.size(); i++){
			hits.at(i)->draw();
		}

		// Remove TextBlocks that have been flagged for removal and increase/decrease score
		for(int i = 0; i < blocks.size(); i++){
			if(blocks.at(i).moving && blocks.at(i).remove){
				std::string text = blocks.at(i).text;
				int charCount = text.length();
				if(!kaboomUsed){
					stats.increaseScore(charCount);
					stats.increaseCorrectCount(1);
				}else{
					// Amount of points lost is equal to half the number of characterss
					stats.decreaseScore(charCount/2);
				}
				blocks.erase(blocks.begin() + i);
			}
		}

		//******* Remove hits (a.k.a explosions) that have finished playing ********//
		for(int i = 0; i < hits.size(); i++){
			if(hits.at(i)->finished == true)
				hits.erase(hits.begin() + i);
		}

		kaboomUsed = false; // reset kaboom flag

		// ********** Background Music Play/No Play ******************
		if(!gameData->playMusic)
			fmod_sys->playSound(bgMusic, 0, false, &bgChan); // Play background music
		else
			fmod_sys->playSound(NULL, 0, false, &bgChan); // No  background music

		//*********** Troubleshooting *************************************************
		//printf(stats.to_string().c_str());
		//printf("animals = %i, places = %i, things = %i, transportation = %i, music = %i, anime = %i\n", 
			//animals, places, things, transportation, music, anime);
		//printf("resetGame = %i\n", resetGame);
		//printf("yPosReached = %i\n", yPosReached);
		//printf(gem->to_string().c_str());
		//printf(gui->to_string().c_str());
		//printf(gameData->to_string().c_str());
		//printf(keyStates->to_string().c_str());
		//printf(timer.to_string().c_str());
		//if(blocks.size() > 0)
			//printf(blocks.at(0)->to_string().c_str());

		// Clear all key states in key state tracking array
		keyStates->zeroAllKeyStates();
		
		// Present the most recent frame.
		SDL_GL_SwapWindow(window);

	} //***** END GAME LOOP ********************************************************

	SDL_Quit();

	return 0;
}

/*
* Checks AABB/AABB collisions
* Used to determine if a TextBlock has touched another TextBlock
*/
bool AABBIntersect(AABB box1, AABB box2){
	//box1 to the right
	if(box1.x > box2.x + box2.w){
		return false;
	}

	// box1 top the left
	if(box1.x + box1.w < box2.x){
		return false;
	}

	// box1 below
	if(box1.y > box2.y + box2.h){
		return false;
	}

	// box1 above
	if(box1.y + box1.h < box2.y){
		return false;
	}
	
	return true;
}

// Reads the words from text file and stores them in a String vector
void readWords(std::ifstream &in){
	std::string temp;
	while(in >> temp){
		words.push_back(temp);
		wordCount ++;
	}
}

// Reads lines from text file and stores them in a String vector
void readLines(std::ifstream &in){
	std::string line;
	while (std::getline(in, line)){
		if(line.compare("#ANIMALS") == 0){
			animals = wordCount;
		}else if(line.compare("#PLACES") == 0){
			places = wordCount;
		}else if(line.compare("#THINGS") == 0){
			things = wordCount;
		}else if(line.compare("#TRANSPORTATION") == 0){
			transportation = wordCount;
		}else if(line.compare("#MUSIC") == 0){
			music = wordCount;
		}else if(line.compare("#ANIME") == 0){
			anime = wordCount;
		}else{}

		words.push_back(line);
		wordCount ++;
	}
}

// Reads the high score file and updates gameData
int readHighScore(std::ifstream &in){
	std::string line;
	std::string line2;
	std::getline(in, line);  // First line has the highscore
	std::getline(in, line2); // Second line has players initials
	highScoreInitials = line2;
	return string_to_int(line);
}

// Takes a numeric string and returns its integer representation
int string_to_int(std::string s){
	std::istringstream instr(s);
	int n;
	instr >> n;
	return n;
}

// convert an integer to a string. return the string
std::string int_to_string(int n){
	ostringstream oss;
	oss << n;
	return oss.str();
}

// Writes a highscore to file and player's initials
void writeHighScore(std::ofstream &out){
	std::string highStr = int_to_string(gameData->highScore);
	if(!addNewHighScoreInitials){
		out << highStr << "\n";
	}else{
		out << highStr << "\n" << gameData->highScoreInitials;
	}
}

/*
 * Loads a sound into memory. If fails, print which file failed and continue
 */
void createSound(const char *name, FMOD::Sound **sound){
	if(!fmod_sys->createSound(name, FMOD_DEFAULT, 0, sound) == FMOD_OK)
		printf("Could not load sound %s\n", name);
}