// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>

using namespace std; // don't really know the consequences

double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];

// Game specific variables here
SGameChar   g_sChar;
SGameChar	g_sSelectort;
SGameChar	g_sSelectorl;

//Monsters: + up down, = left right, $ clockwise, % anti
SMapMonster monsterList[50];

EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime;				// this is to prevent key bouncing, so we won't trigger keypresses more than once
double  g_dMonsterTime;				// for monster
int monsterRenderOrNot = 0;

// for Jeremiah's minigame
vector<int> sequence;				// vector for sequence for minigame
int redTrue, blueTrue, yellowTrue, greenTrue = 2; // for showing the values of the vector in the interface
int indicatorTrue = 1;
int currentIndex = 0;				// to slowly check array of sequence
double g_dScanTime;					// to have delay for displaying in minigame
int sequenceNumber = 0;				// to display number showing
int inputState = 0;					// for minigame to enter the input state
double g_dSequenceTime;				// to prevent key bouncing when entering input for sequence
int inputSequence[8];				// to store player's inputs
int inputIndex = 0;					// index to save into inputSequence
int miniGameCorrectOrNot = 0;       // check if minigame is correct
double g_dDelayDisplayTime;			// to show players all lights dimmed
bool runMinigame = 0;				// to check if delay is over
bool bMinigameDelayed = 0;			// to make delay happen once only
double g_dButtonColorTime;          // for button to light up for a while

// for Darren's minigame
int mistakes = 0;
int correctguesses = 0;
int currNum = 0;
int nextNum = 0;

int whichMap = 0;					// to identify current map being played
char myArray[25][80] = { 0, };		// array version of the map
char splashArray[25][80] = { 0, };	// array version of splash screen
char customArray[27][80] = { 0, };	// array version of custom map
char customGArray[27][80] = { 0, }; // array version of custom map editor guide
char saveArray[6];					// stores data for loading and saving
int spawnedOrNot = 0;				// check if player spawned or not

// identify if button has been pressed or not
bool button0 = 0;
bool button2 = 0;
bool button4 = 0;
bool button6 = 0;
bool button8 = 0;

string m0 = "Map_Tutorial.txt";
string m1 = "Map_01.txt";
string m2 = "Map_02.txt";
string m3 = "Map_03.txt";
string m4 = "Map_04.txt";
string m5 = "Map_05.txt";
string death = "death.txt";
string ending = "Game_end.txt";
string splash = "Splash_Screen.txt";
string custom = "CMap_Template.txt";
string title = "title.txt";
string mini = "Minigame.txt";

int titleChoice = 0; // init to 0
int levelChoice = 0; // init to 0

bool isCustomOrNot = 0;

// Console object
Console g_Console(80, 25, "SP1 Framework");

void customGuide()
{
	string LineData;
	ifstream MapDataFile;

	MapDataFile.open("CMap_User_Guide.txt");

	//getline(MapDataFile, LineData);
	//getline(MapDataFile, LineData);

	for (unsigned int row = 0; row < 27; ++row)
	{
		if (!getline(MapDataFile, LineData))
		{
			break;
		}

		for (unsigned int column = 0; column < 80; ++column)
		{
			customGArray[row][column] = (char)LineData[column];
		}
	}
	MapDataFile.close();
}

void customLoading()
{
	string LineData;
	ifstream MapDataFile;

	MapDataFile.open("CMap_Template.txt");

	//getline(MapDataFile, LineData);
	//getline(MapDataFile, LineData);

	for (unsigned int row = 0; row < 27; ++row)
	{
		if (!getline(MapDataFile, LineData))
		{
			break;
		}

		for (unsigned int column = 0; column < 80; ++column)
		{
			customArray[row][column] = (char)LineData[column];
		}
	}
	MapDataFile.close();
}

void splashLoading(string splashString)
{
	COORD c = g_Console.getConsoleSize();

	string splashData;
	ifstream splashDataFile;

	splashDataFile.open(splashString);

	getline(splashDataFile, splashData);
	getline(splashDataFile, splashData);

	for (unsigned int row = 0; row < 25; ++row)
	{
		if (!getline(splashDataFile, splashData))
		{
			break;
		}

		for (unsigned int column = 0; column < 80; ++column)
		{
			splashArray[row][column] = (char)splashData[column];
		}
	}
	splashDataFile.close();
}

void mapLoading(string stringMap)
{
	string LineData;
	ifstream MapDataFile;

	MapDataFile.open(stringMap);

	getline(MapDataFile, LineData);
	getline(MapDataFile, LineData);

	for (unsigned int row = 0; row < 25; ++row)
	{
		if (!getline(MapDataFile, LineData))
		{
			break;
		}

		for (unsigned int column = 0; column < 80; ++column)
		{
			myArray[row][column] = (char)LineData[column];
		}
	}
	MapDataFile.close();
}

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;
    g_dBounceTime = 0.0;

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 2;
    g_sChar.m_bActive = true;

    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");
	
	splashLoading(splash);

	g_sSelectort.m_cLocation.Y = 11;
	g_sSelectort.m_cLocation.X = 31;

	g_sSelectorl.m_cLocation.Y = 7;
	g_sSelectorl.m_cLocation.X = 31;

	customGuide();

	// to make rand random
	srand(time(NULL));
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Getting all the key press states
//            This function checks if any key had been pressed since the last time we checked
//            If a key is pressed, the value for that particular key will be true
//
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{    
    g_abKeyPressed[K_UP]     = isKeyPressed(VK_UP);
    g_abKeyPressed[K_DOWN]   = isKeyPressed(VK_DOWN);
    g_abKeyPressed[K_LEFT]   = isKeyPressed(VK_LEFT);
    g_abKeyPressed[K_RIGHT]  = isKeyPressed(VK_RIGHT);
    g_abKeyPressed[K_SPACE]  = isKeyPressed(VK_SPACE);
    g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);

	// added on top of the framework; used for minigames and custom map builder
	g_abKeyPressed[K_RSHIFT]	= isKeyPressed(VK_RSHIFT);
	g_abKeyPressed[K_LSHIFT]	= isKeyPressed(VK_LSHIFT);
	g_abKeyPressed[K_0]			= isKeyPressed(0x30);
	g_abKeyPressed[K_1]			= isKeyPressed(0x31);
	g_abKeyPressed[K_2]			= isKeyPressed(0x32);
	g_abKeyPressed[K_3]			= isKeyPressed(0x33);
	g_abKeyPressed[K_4]			= isKeyPressed(0x34);
	g_abKeyPressed[K_5]			= isKeyPressed(0x35);
	g_abKeyPressed[K_6]			= isKeyPressed(0x36);
	g_abKeyPressed[K_7]			= isKeyPressed(0x37);
	g_abKeyPressed[K_8]			= isKeyPressed(0x38);
	g_abKeyPressed[K_9]			= isKeyPressed(0x39);
	g_abKeyPressed[K_PLUS]		= isKeyPressed(VK_OEM_PLUS);
	g_abKeyPressed[K_MINUS]		= isKeyPressed(VK_OEM_MINUS);
	g_abKeyPressed[K_COMMA]		= isKeyPressed(VK_OEM_COMMA);
	g_abKeyPressed[K_PERIOD]	= isKeyPressed(VK_OEM_PERIOD);
	g_abKeyPressed[K_TAB]		= isKeyPressed(VK_TAB);

	// for Jeremiah's minigame
	g_abKeyPressed[K_N1] = isKeyPressed(VK_NUMPAD1);
	g_abKeyPressed[K_N2] = isKeyPressed(VK_NUMPAD2);
	g_abKeyPressed[K_N3] = isKeyPressed(VK_NUMPAD3);
	g_abKeyPressed[K_N4] = isKeyPressed(VK_NUMPAD4);
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
		case S_ENDSCREEN: endLogic();
			break;
		case S_TITLE: titleLogic(); // logic for title
			break;
		case S_LEVELS: levelsLogic();
			break;
		case S_CUSTOM: customLogic();
			break;
		case S_DEATH: deathLogic(); // game logic for the death screen
			break;
        case S_GAME: gameplay(); // gameplay logic when we are in the game
            break;
		case S_USERG: userGLogic();
			break;
		case S_MINIGAME: minigameLogic();
			break;
		case S_HIGHORLOW: highorlow();
			break;
    }
}
//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
        case S_SPLASHSCREEN: renderSplashScreen();
            break;
		case S_ENDSCREEN: endRender();
			break;
		case S_DEATH: deathRender();
			break;
		case S_TITLE: titleRender();
			break;
		case S_LEVELS: levelsRender();
			break;
		case S_CUSTOM: customRender();
			break;
        case S_GAME: renderGame();
            break;
		case S_USERG: customGRender();
			break;
		case S_MINIGAME: renderMinigame();
			break;
		case S_HIGHORLOW: highorlowrender();
			break;
    }
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 3.0) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_TITLE;
}

void endLogic()
{
	bool bSomethingHappened = false;
	if (g_dBounceTime > g_dElapsedTime)
		return;

	if (g_abKeyPressed[K_ESCAPE])
	{
		bSomethingHappened = true;
		g_eGameState = S_TITLE;
	}

	if (g_abKeyPressed[K_SPACE])
	{
		bSomethingHappened = true;
		g_eGameState = S_HIGHORLOW;
	}

	if (bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.15;
	}
}

void levelsLogic()
{
	bool bSomethingHappened = false;
	if (g_dBounceTime > g_dElapsedTime)
		return;

	// update the saveArray with contents from the txt file
	string saveData;
	ifstream saveLoad("Saveload.txt");

	getline(saveLoad, saveData);

	for (int i = 0; i < 6; i++)
	{
		saveArray[i] = (char)saveData[i];
	}
	saveLoad.close();

	if ((g_abKeyPressed[K_DOWN]) && (levelChoice < 5))
	{
		levelChoice++;
		g_sSelectorl.m_cLocation.Y += 2;
		bSomethingHappened = true;
	}

	if ((g_abKeyPressed[K_UP]) && (levelChoice > 0))
	{
		levelChoice--;
		g_sSelectorl.m_cLocation.Y -= 2;
		bSomethingHappened = true;
	}

	if (g_abKeyPressed[K_ESCAPE])
	{
		bSomethingHappened = true;
		g_eGameState = S_TITLE;
	}

	// if the array says that the player has unlocked the next level, allow loading
	if ((g_abKeyPressed[K_SPACE]) && (saveArray[levelChoice] == '1'))
	{
		bSomethingHappened = true;
		whichMap = levelChoice;
		mapIdentifier();
		getMonsterXYType();
		g_eGameState = S_GAME;
	}

	if (bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.15;
	}
}

void userGLogic()
{
	bool bSomethingHappened = false;
	if (g_dBounceTime > g_dElapsedTime)
		return;

	if (g_abKeyPressed[K_TAB])
	{
		bSomethingHappened = true;
		g_eGameState = S_CUSTOM;
	}

	if (bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.15;
	}
}

// note: might want to add LSHIFT alongside RSHIFT later on
// note: make a user guide
void customKeyReg()
{
	if (g_abKeyPressed[K_0])
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '0';
	}
	if (g_abKeyPressed[K_1])
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '1';
	}
	if (g_abKeyPressed[K_2])
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '2';
	}
	if (g_abKeyPressed[K_3])
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '3';
	}
	if (g_abKeyPressed[K_4])
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '4';
	}
	if (g_abKeyPressed[K_5])
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '5';
	}
	if (g_abKeyPressed[K_6])
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '6';
	}
	if (g_abKeyPressed[K_7])
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '7';
	}
	if (g_abKeyPressed[K_8])
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '8';
	}
	if (g_abKeyPressed[K_9])
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '9';
	}
	if (((g_abKeyPressed[K_2]) && (g_abKeyPressed[K_RSHIFT])) ||
		((g_abKeyPressed[K_2]) && (g_abKeyPressed[K_LSHIFT])))
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '@';
	}
	if (((g_abKeyPressed[K_3]) && (g_abKeyPressed[K_RSHIFT])) ||
		((g_abKeyPressed[K_3]) && (g_abKeyPressed[K_LSHIFT])))
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '#';
	}
	if (((g_abKeyPressed[K_4]) && (g_abKeyPressed[K_RSHIFT])) ||
		((g_abKeyPressed[K_4]) && (g_abKeyPressed[K_LSHIFT])))
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '$';
	}
	if (((g_abKeyPressed[K_5]) && (g_abKeyPressed[K_RSHIFT])) ||
		((g_abKeyPressed[K_5]) && (g_abKeyPressed[K_LSHIFT])))
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '%';
	}
	if (((g_abKeyPressed[K_6]) && (g_abKeyPressed[K_RSHIFT])) ||
		((g_abKeyPressed[K_6]) && (g_abKeyPressed[K_LSHIFT])))
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '^';
	}
	if (g_abKeyPressed[K_MINUS])
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '-';
	}
	if (((g_abKeyPressed[K_PLUS]) && (g_abKeyPressed[K_RSHIFT])) ||
		((g_abKeyPressed[K_PLUS]) && (g_abKeyPressed[K_LSHIFT])))
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '+';
	}
	else if (g_abKeyPressed[K_PLUS])
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '=';
	}
	if (((g_abKeyPressed[K_COMMA]) && (g_abKeyPressed[K_RSHIFT])) ||
		((g_abKeyPressed[K_COMMA]) && (g_abKeyPressed[K_LSHIFT])))
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '<';
	}
	if (((g_abKeyPressed[K_PERIOD]) && (g_abKeyPressed[K_RSHIFT])) ||
		((g_abKeyPressed[K_PERIOD]) && (g_abKeyPressed[K_LSHIFT])))
	{
		customArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] = '>';
	}
}

void customLogic()
{
	bool bSomethingHappened = false;
	if (g_dBounceTime > g_dElapsedTime)
		return;

	customMoveCharacter();
	customKeyReg();

	if (g_abKeyPressed[K_ESCAPE])
	{
		bSomethingHappened = true;
		g_eGameState = S_TITLE;
	}

	if (g_abKeyPressed[K_SPACE])
	{
		bSomethingHappened = true;

		// save the contents of customArray into the txt file
		ofstream custom;
		custom.open("CMap_Template.txt");

		for (int i = 0; i < 27; i++)
		{
			for (int j = 0; j < 80; j++)
			{
				custom << customArray[i][j];
			}
			custom << "\n";
		}
		custom.close();

		for (int i = 0; i < 25; i++)
		{
			for (int j = 0; j < 80; j++)
			{
				myArray[i][j] = customArray[i][j];
			}
		}
		getMonsterXYType();
		g_eGameState = S_GAME;
	}

	if (g_abKeyPressed[K_TAB])
	{
		bSomethingHappened = true;
		g_eGameState = S_USERG;
	}

	if (bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.15;
	}
}

void titleLogic()
{
	bool bSomethingHappened = false;
	if (g_dBounceTime > g_dElapsedTime)
		return;

	// use the value of 1 since there are only 2 choices (as of time of writing), 0 for level select and 1 for custom map editor.
	if ((g_abKeyPressed[K_DOWN]) && (titleChoice < 1)) 
	{
		titleChoice++;
		g_sSelectort.m_cLocation.Y += 2;
		bSomethingHappened = true;
	}

	if ((g_abKeyPressed[K_UP]) && (titleChoice > 0))
	{
		titleChoice--;
		g_sSelectort.m_cLocation.Y -= 2;
		bSomethingHappened = true;
	}

	if ((g_abKeyPressed[K_SPACE]) && (titleChoice == 0))
	{
		isCustomOrNot = 0;
		bSomethingHappened = true;
		g_eGameState = S_LEVELS;
	}

	if ((g_abKeyPressed[K_SPACE]) && (titleChoice == 1))
	{
		isCustomOrNot = 1;
		bSomethingHappened = true;
		customLoading();
		g_eGameState = S_CUSTOM;
	}

	if (g_abKeyPressed[K_ESCAPE])
	{
		g_bQuitGame = true;
	}

	if (bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.15;
	}
}

void deathLogic()
{
	bool bSomethingHappened = false;
	if (g_dBounceTime > g_dElapsedTime)
		return;

	if (g_abKeyPressed[K_SPACE])
	{
		mapIdentifier();
		getMonsterXYType();
		bSomethingHappened = true;
		g_eGameState = S_GAME;
	}

	if (g_abKeyPressed[K_ESCAPE])
	{
		bSomethingHappened = true;
		g_eGameState = S_TITLE;
	}

	if (bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.1; // 125ms should be enough
	}
}

void deathReset()
{
	clearMonsters();
	spawnedOrNot = 0;
	monsterRenderOrNot = 0;
	button0 = 0;
	button2 = 0;
	button4 = 0;
	button6 = 0;
	button8 = 0;
	g_eGameState = S_DEATH;
}

void teleport() // one way teleporting from < to >
{
	if (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] == '<')
	{
		for (int row = 0; row < 25; row++)
		{
			for (int column = 0; column < 80; column++)
			{
				if (myArray[row][column] == '>')
				{
					g_sChar.m_cLocation.Y = row;
					g_sChar.m_cLocation.X = column;
				}
			}
		}
	}
}

void playerSpawn()
{
	if (spawnedOrNot == 0)
	{
		// find the '@' starting point of the player and spawn there
		for (int lesserno = 0; lesserno < 25; lesserno++)
		{
			for (int greaterno = 0; greaterno < 80; greaterno++)
			{
				if (myArray[lesserno][greaterno] == '@')
				{
					g_sChar.m_cLocation.X = greaterno;
					g_sChar.m_cLocation.Y = lesserno;
					spawnedOrNot = 1;
				}
			}
		}
	}
	else
	{
		;
	}

	// if its still not spawned (means no '@' in the map)
	if (spawnedOrNot == 0)
	{
		g_sChar.m_cLocation.X = g_Console.getConsoleSize().X / 2;
		g_sChar.m_cLocation.Y = g_Console.getConsoleSize().Y / 2;
		spawnedOrNot = 1;
	}
}

void buttonReg()
{
	// register buttons walked over
	switch (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X])
	{
	case '0':
		button0 = 1;
		break;
	case '2':
		button2 = 1;
		break;
	case '4':
		button4 = 1;
		break;
	case '6':
		button6 = 1;
		break;
	case '8':
		button8 = 1;
		break;
	default:
		break;
	}
}

void mapIdentifier()
{
	switch (whichMap)
	{
	case 0:
		mapLoading(m0);
		break;
	case 1:
		mapLoading(m1);
		break;
	case 2:
		mapLoading(m2);
		break;
	case 3:
		mapLoading(m3);
		break;
	case 4:
		mapLoading(m4);
		break;
	case 5:
		mapLoading(m5);
		break;
	case 6:
		splashLoading(ending);
		g_eGameState = S_ENDSCREEN;
		break;
	default:
		break;
	}
}

// links S_GAME to S_CUSTOM from custom map maker.
void customEndingReg()
{
	clearMonsters();
	spawnedOrNot = 0;
	button0 = 0;
	button2 = 0;
	button4 = 0;
	button6 = 0;
	button8 = 0;
	monsterRenderOrNot = 0;

	g_eGameState = S_CUSTOM;
}

void greenReg()
{
	// change the map when reach the end point '*'
	// reset the button states for reuse
	if (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] == '*')
	{
		// when player finishes a level, update the array
		saveArray[whichMap + 1] = '1';

		// and update the txt file with the new array
		if (whichMap < 5)
		{
			ofstream saveLoad("Saveload.txt");

			for (int i = 0; i < 6; i++)
			{
				saveLoad << saveArray[i];
			}

			saveLoad.close();
		}

		clearMonsters();
		
		spawnedOrNot = 0;
		button0 = 0;
		button2 = 0;
		button4 = 0;
		button6 = 0;
		button8 = 0;
		monsterRenderOrNot = 0;

		if (whichMap == 5)
		{
			g_eGameState = S_MINIGAME;
		}
		else
		{
			whichMap++;
		}

		mapIdentifier();
	}
}

void gameplay()         // gameplay logic
{
	playerSpawn();		// spawns the player

	// registers player death when touching monsters
	for (int i = 0; i < 50; i++)
	{
		if ((g_sChar.m_cLocation.Y == monsterList[i].m_cLocation.Y) &&
			(g_sChar.m_cLocation.X == monsterList[i].m_cLocation.X))
		{
			if (isCustomOrNot == 0)
			{
				deathReset();
			}
			else
			{
				customEndingReg();
			}
		}
	}

	while (monsterRenderOrNot != 1)
	{
		getMonsterXYType();
		monsterRenderOrNot = 1;
	}

	moveMonsters();     // moves monsters
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
	teleport();			// one-way teleport mechanics
	buttonReg();		// register button presses
	greenReg();			// register if player has reached the end of a level

	if (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] == '^')
	{
		customEndingReg();
	}
}

int playerCollision(int Y, int X)
{
	if (((button0 == 1) && (myArray[Y][X] == '1')) ||
		((button2 == 1) && (myArray[Y][X] == '3')) ||
		((button4 == 1) && (myArray[Y][X] == '5')) ||
		((button6 == 1) && (myArray[Y][X] == '7')) ||
		((button8 == 1) && (myArray[Y][X] == '9')))
	{
		return 1;
	}

	if ((myArray[Y][X] == '0') ||
		(myArray[Y][X] == '2') ||
		(myArray[Y][X] == '4') ||
		(myArray[Y][X] == '6') ||
		(myArray[Y][X] == '8'))
	{
		return 1;
	}

	if ((myArray[Y][X] < '0') || (myArray[Y][X] > '9'))
	{
		return 1;
	}
	return 0;
}

void moveCharacter()
{
    bool bSomethingHappened = false;
    if (g_dBounceTime > g_dElapsedTime)
        return;

    // Updating the location of the character based on the key press
    // providing a beep sound whenever we shift the character
    if (g_abKeyPressed[K_UP] && g_sChar.m_cLocation.Y > 0)
    {
        //Beep(1440, 30);
		if (myArray[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] != '~')
		{
			if (playerCollision(g_sChar.m_cLocation.Y - 1, g_sChar.m_cLocation.X) == 1)
			{
				g_sChar.m_cLocation.Y--;
			}
		}
        bSomethingHappened = true;
    }
    if (g_abKeyPressed[K_LEFT] && g_sChar.m_cLocation.X > 0)
    {
        //Beep(1440, 30);
		if (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] != '~')
		{
			if (playerCollision(g_sChar.m_cLocation.Y, g_sChar.m_cLocation.X - 1) == 1)
			{
				g_sChar.m_cLocation.X--;
			}
		}
        bSomethingHappened = true;
    }
    if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
        //Beep(1440, 30);
		if (myArray[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] != '~')
		{
			if (playerCollision(g_sChar.m_cLocation.Y + 1, g_sChar.m_cLocation.X) == 1)
			{
				g_sChar.m_cLocation.Y++;
			}
		}
        bSomethingHappened = true;
    }
    if (g_abKeyPressed[K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
        //Beep(1440, 30);
        
		if (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] != '~')
		{
			if (playerCollision(g_sChar.m_cLocation.Y, g_sChar.m_cLocation.X + 1) == 1)
			{
				g_sChar.m_cLocation.X++;
			}
		}
        bSomethingHappened = true;
    }
    if (g_abKeyPressed[K_SPACE])
    {
		; // removed code due to lack of use in the game (pending future implementation)
    }

    if (bSomethingHappened)
    {
        // set the bounce time to some time in the future to prevent accidental triggers
        g_dBounceTime = g_dElapsedTime + 0.1; // 125ms should be enough
    }
}

void customMoveCharacter()
{
	bool bSomethingHappened = false;
	if (g_dBounceTime > g_dElapsedTime)
		return;

	// Updating the location of the character based on the key press
	// providing a beep sound whenever we shift the character
	if ((g_abKeyPressed[K_UP] && g_sChar.m_cLocation.Y > 0) &&
		customArray[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] != '_')
	{
		//Beep(1440, 30);
		g_sChar.m_cLocation.Y--;
		bSomethingHappened = true;
	}
	if (g_abKeyPressed[K_LEFT] && g_sChar.m_cLocation.X > 0)
	{
		//Beep(1440, 30);
		g_sChar.m_cLocation.X--;
		bSomethingHappened = true;
	}
	if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1 &&
		customArray[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] != '_')
	{
		//Beep(1440, 30);
		g_sChar.m_cLocation.Y++;
		bSomethingHappened = true;
	}
	if (g_abKeyPressed[K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
	{
		//Beep(1440, 30);
		g_sChar.m_cLocation.X++;
		bSomethingHappened = true;
	}
	if (g_abKeyPressed[K_SPACE])
	{
		; // removed code due to lack of use in the game (pending future implementation)
	}

	if (bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.1; // 125ms should be enough
	}
}

void processUserInput()
{
	bool bSomethingHappened = false;
	if (g_dBounceTime > g_dElapsedTime)
		return;

    // change to level select if player hits the escape key
	if (g_abKeyPressed[K_ESCAPE])
	{
		spawnedOrNot = 0;
		button0 = 0;
		button2 = 0;
		button4 = 0;
		button6 = 0;
		button8 = 0;
		monsterRenderOrNot = 0;
		clearMonsters();
		bSomethingHappened = true;
		g_eGameState = S_TITLE;
	}

	// change to custom map editor
	if ((g_abKeyPressed[K_SPACE]) && isCustomOrNot == 1)
	{
		spawnedOrNot = 0;
		button0 = 0;
		button2 = 0;
		button4 = 0;
		button6 = 0;
		button8 = 0;
		monsterRenderOrNot = 0;
		clearMonsters();
		bSomethingHappened = true;
		g_eGameState = S_CUSTOM;
	}

	if (bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.2;
	}
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x00);
}

void endRender()
{
	splashLoading(ending);
	renderSplashScreen();
}

void levelsRender()
{
	COORD c = g_Console.getConsoleSize();
	c.Y = 7;
	c.X = 33;
	g_Console.writeToBuffer(c, "TUTORIAL", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	c.Y += 2;
	g_Console.writeToBuffer(c, "LEVEL I", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	c.Y += 2;
	g_Console.writeToBuffer(c, "LEVEL II", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	c.Y += 2;
	g_Console.writeToBuffer(c, "LEVEL III", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	c.Y += 2;
	g_Console.writeToBuffer(c, "LEVEL IV", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	c.Y += 2;
	g_Console.writeToBuffer(c, "LEVEL V", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

	c.Y = 9;
	c.X = 44;

	// starts at 1 because the first container [0] is reserved for tutorial
	for (int i = 1; i < 6; i++)
	{
		if (saveArray[i] == '1')
		{
			g_Console.writeToBuffer(c, "UNLOCKED", FOREGROUND_GREEN);
		}
		else
		{
			g_Console.writeToBuffer(c, "LOCKED", FOREGROUND_RED);
		}
		c.Y += 2;
	}

	g_Console.writeToBuffer(g_sSelectorl.m_cLocation, '>', FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}

void customGRender()
{
	COORD c;

	for (unsigned int row = 0; row < 27; row++)
	{
		c.Y = row;

		for (unsigned int column = 0; column < 80; column++)
		{
			c.X = column;
			
			switch (customGArray[c.Y][c.X])
			{
			case '#':
				g_Console.writeToBuffer(c, customGArray[c.Y][c.X], 0x00);
				break;
			case '!':
				g_Console.writeToBuffer(c, '#', FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				break;
			default:
				g_Console.writeToBuffer(c, customGArray[c.Y][c.X], FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				break;
			}
		}
	}
}

void customRender()
{
	COORD c;

	// render in raw state
	for (unsigned int row = 0; row < 27; row++)
	{
		c.Y = row;
		
		for (unsigned int column = 0; column < 80; column++)
		{
			c.X = column;
			g_Console.writeToBuffer(c, customArray[c.Y][c.X], FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
		}
	}

	renderCharacter();
}

void titleRender()
{
	splashLoading(title);
	renderSplashScreen();
	g_Console.writeToBuffer(g_sSelectort.m_cLocation, '>', FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}

void deathRender()
{
	splashLoading(death);
	renderSplashScreen();
}

void renderSplashScreen()  // renders the splash screen
{
	COORD c;

	for (unsigned int row = 0; row < 25; row++)
	{
		c.Y = row;

		for (unsigned int column = 0; column < 80; column++)
		{
			c.X = column;

			// renders from the array instead of the txt file now
			switch (splashArray[c.Y][c.X])
			{
			case '#':
				g_Console.writeToBuffer(c, 219, 0x00);
				break;
			case '-':
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				break;
			default:
				g_Console.writeToBuffer(c, splashArray[c.Y][c.X], FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				break;
			}
		}
	}
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
	renderMonsters();
}

void renderMap()
{
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    COORD c;

	for (unsigned int row = 0; row < 25; ++row)
	{
		c.Y = row;

		// There is a better way to do this but we need the different characters to mark positions on the maps
		for (unsigned int column = 0; column < 80; ++column)
		{
			c.X = column;

			switch (myArray[c.Y][c.X])
			{
			case '#': // The walls
				g_Console.writeToBuffer(c, 219, 0x00);
				break;
			case '-': // The spacing
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				break;
			case '*': // The end point
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN);
				break;
			case '^': // The end point for custom map
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN);
				break;
			case '@': // The player start point
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				break;
			case '%': // The monster start point
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				break;
			case '+': // The monster start point
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				break;
			case '=': // The monster start point
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				break;
			case '$': // The monster start point
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				break;
			case '/': // The monster path
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				break;
			case '<':
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN | FOREGROUND_RED);
				break;
			case '[':
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN | FOREGROUND_RED);
				break;
			case '>':
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN | FOREGROUND_BLUE);
				break;
			case ']':
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN | FOREGROUND_BLUE);
				break;
			case '!':
				g_Console.writeToBuffer(c, 219, FOREGROUND_RED);
				break;
			case '0':
				g_Console.writeToBuffer(c, 219, FOREGROUND_BLUE | FOREGROUND_RED);
				break;
			case '1':
				if (button0 == 0)
				{
					g_Console.writeToBuffer(c, 219, 0x00);
				}
				else if (button0 == 1)
				{
					g_Console.writeToBuffer(c, 219, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				break;
			case '3':
				if (button2 == 0)
				{
					g_Console.writeToBuffer(c, 219, 0x00);
				}
				else if (button2 == 1)
				{
					g_Console.writeToBuffer(c, 219, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				break;
			case '5':
				if (button4 == 0)
				{
					g_Console.writeToBuffer(c, 219, 0x00);
				}
				else if (button4 == 1)
				{
					g_Console.writeToBuffer(c, 219, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				break;
			case '7':
				if (button6 == 0)
				{
					g_Console.writeToBuffer(c, 219, 0x00);
				}
				else if (button6 == 1)
				{
					g_Console.writeToBuffer(c, 219, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				break;
			case '9':
				if (button8 == 0)
				{
					g_Console.writeToBuffer(c, 219, 0x00);
				}
				else if (button8 == 1)
				{
					g_Console.writeToBuffer(c, 219, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				break;
			default:
				g_Console.writeToBuffer(c, myArray[c.Y][c.X], FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				break;
			}

			if ((myArray[c.Y][c.X] >= '0') && (myArray[c.Y][c.X] <= '9'))
			{
				switch ((myArray[c.Y][c.X] - 48) % 2)
				{
				case 0:
					g_Console.writeToBuffer(c, 219, FOREGROUND_BLUE | FOREGROUND_RED);
					break;
				default:
					break;
				}
			}
		}
	}
}

//0 Down 1 Up 2 Left 3 Right
//Function for moving monsters up and down
void moveUpDownMonster(SMapMonster &monster, bool &somethingHappened)
{
	switch (monster.m_iDirection)
	{
	case 0:
		if (myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] != '#')
		{
			monster.m_cLocation.Y++;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 1;
		}
		break;
	case 1:
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] != '#')
		{
			monster.m_cLocation.Y--;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 0;
		}
		break;
	case 4:
		if (myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] != '#')
		{
			monster.m_iDirection = 0;
		}
		else
		{
			monster.m_iDirection = 1;
		}
		break;
	}
}

//Function for moving monsters left and right
void moveLeftRightMonster(SMapMonster &monster, bool &somethingHappened)
{
	switch (monster.m_iDirection)
	{
	case 2:
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] != '#')
		{
			monster.m_cLocation.X--;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 3;
		}
		break;
	case 3:
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] != '#')
		{
			monster.m_cLocation.X++;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 2;
		}
		break;
	case 4:
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] != '#')
		{
			monster.m_iDirection = 2;
		}
		else
		{
			monster.m_iDirection = 3;
		}
		break;
	}
}

//Function for moving monsters clockwise
void moveClockwiseMonster(SMapMonster &monster, bool &somethingHappened)
{
	switch (monster.m_iDirection)
	{
	case 0:
		if (myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] != '#')
		{
			monster.m_cLocation.Y++;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 2;
		}
		break;
	case 1:
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] != '#')
		{
			monster.m_cLocation.Y--;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 3;
		}
		break;
	case 2:
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] != '#')
		{
			monster.m_cLocation.X--;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 1;
		}
		break;
	case 3:
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] != '#')
		{
			monster.m_cLocation.X++;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 0;
		}
		break;
	case 4:
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] == '#' && myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] == '#')
		{
			monster.m_iDirection = 1;
		}
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] == '#' && myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] == '#')
		{
			monster.m_iDirection = 3;
		}
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] == '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] == '#')
		{
			monster.m_iDirection = 0;
		}
		if (myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] == '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] == '#')
		{
			monster.m_iDirection = 2;
		}
		break;
	}
}

//Function for moving monsters anticlockwise
void moveAntiClockwiseMonster(SMapMonster &monster, bool &somethingHappened)
{
	switch (monster.m_iDirection)
	{
	case 0:
		if (myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] != '#')
		{
			monster.m_cLocation.Y++;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 3;
		}
		break;
	case 1:
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] != '#')
		{
			monster.m_cLocation.Y--;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 2;
		}
		break;
	case 2:
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] != '#')
		{
			monster.m_cLocation.X--;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 0;
		}
		break;
	case 3:
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] != '#')
		{
			monster.m_cLocation.X++;
			somethingHappened = true;
		}
		else
		{
			monster.m_iDirection = 1;
		}
		break;
	case 4:
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] == '#' && myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] == '#')
		{
			monster.m_iDirection = 3;
		}
		if (myArray[monster.m_cLocation.Y][monster.m_cLocation.X - 1] == '#' && myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] == '#')
		{
			monster.m_iDirection = 0;
		}
		if (myArray[monster.m_cLocation.Y - 1][monster.m_cLocation.X] == '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] == '#')
		{
			monster.m_iDirection = 2;
		}
		if (myArray[monster.m_cLocation.Y + 1][monster.m_cLocation.X] == '#' && myArray[monster.m_cLocation.Y][monster.m_cLocation.X + 1] == '#')
		{
			monster.m_iDirection = 1;
		}
		break;
	}
}

void getMonsterXYType()
{
	int i = 0;

	for (int y = 0; y < 25; y++)
	{
		for (int x = 0; x < 80; x++)
		{
			switch (myArray[y][x])
			{
			case '+':
				monsterList[i].m_cLocation.Y = y;
				monsterList[i].m_cLocation.X = x;
				monsterList[i].m_cType = '+';
				monsterList[i].m_iDirection = 4;
				i++;
				break;
			case '=':
				monsterList[i].m_cLocation.Y = y;
				monsterList[i].m_cLocation.X = x;
				monsterList[i].m_cType = '=';
				monsterList[i].m_iDirection = 4;
				i++;
				break;
			case '$':
				monsterList[i].m_cLocation.Y = y;
				monsterList[i].m_cLocation.X = x;
				monsterList[i].m_cType = '$';
				monsterList[i].m_iDirection = 4;
				i++;
				break;
			case '%':
				monsterList[i].m_cLocation.Y = y;
				monsterList[i].m_cLocation.X = x;
				monsterList[i].m_cType = '%';
				monsterList[i].m_iDirection = 4;
				i++;
			}
		}
	}
}

void clearMonsters()
{
	for (int i = 0; i < 50; i++)
	{
		monsterList[i].m_cLocation = { 0, };
		monsterList[i].m_iDirection = 0;
		monsterList[i].m_cType = 0;
	}
	
	for (int i = 0; i < 25; i++)
	{
		for (int j = 0; j < 80; j++)
		{
			myArray[i][j] = 0;
		}
	}
}

void renderMonsters()
{
	WORD monsterColor = FOREGROUND_RED;
	for (int i = 0; i < 50; i++)
	{
		g_Console.writeToBuffer(monsterList[i].m_cLocation, (char)219, monsterColor);
	}
}

void moveMonsters()
 {
	bool bSomethingHappened = false;
	if (g_dMonsterTime > g_dElapsedTime)
		return;

	for (int i = 0; i < 50; i++)
	{
		switch (monsterList[i].m_cType)
		{
		case '+':
			moveUpDownMonster(monsterList[i], bSomethingHappened);
			break;
		case '=':
			moveLeftRightMonster(monsterList[i], bSomethingHappened);
			break;
		case '$':
			moveClockwiseMonster(monsterList[i], bSomethingHappened);
			break;
		case '%':
			moveAntiClockwiseMonster(monsterList[i], bSomethingHappened);
			break;
		}

		if (bSomethingHappened)
		{
			g_dMonsterTime = g_dElapsedTime + 0.1;
		}
	}
}

void renderCharacter()
{
    // Draw the location of the character
	WORD charColor = FOREGROUND_BLUE;
    if (g_sChar.m_bActive)
    {
		charColor = FOREGROUND_BLUE; // useless for now
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)219, charColor);
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}
void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

// Darren's code WIP 
void highorlow()
{
	if (mistakes >= 3)
	{
		// go back to main game
		g_eGameState = S_TITLE;
		mistakes = 0;
		correctguesses = 0;
		currNum = 0;
		return;
	}
	if (correctguesses >= 5)
	{
		// go back to main game
		g_eGameState = S_TITLE;
		mistakes = 0;
		correctguesses = 0;
		currNum = 0;
		return;
	}

	if (currNum == 0)
	{
		currNum = rand() % 9 + 48; // randomise current number
		do
		{
			nextNum = rand() % 9 + 48; //randomise next number until it is different from current number
		} while (nextNum == currNum);
	}

	bool bSomethingHappened = false;

	if (g_dBounceTime > g_dElapsedTime)
		return;

	if (g_abKeyPressed[K_UP])
	{
		if (currNum < nextNum)
		{
			correctguesses++;
		}
		else if (currNum > nextNum)
		{
			mistakes++;
		}
		bSomethingHappened = true;
	}
	if (g_abKeyPressed[K_DOWN])
	{
		if (currNum > nextNum)
		{
			correctguesses++;
		}
		else if (currNum < nextNum)
		{
			mistakes++;
		}
		bSomethingHappened = true;
	}
	currNum = nextNum;

	if (bSomethingHappened)
	{
		currNum = nextNum;
		do
		{
			nextNum = rand() % 9 + 48;
		} while (nextNum == currNum);

		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.1;
		highorlowrender();
	}
}

void highorlowrender()
{
	COORD c = g_Console.getConsoleSize();

	for (int x = 0; x < 80; x++)
	{
		for (int y = 0; y < 25; y++)
		{
			c.Y = 4;
			c.X = 32;
			g_Console.writeToBuffer(c, "Current Number is ", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

			c.Y = 6;
			c.X = 48;
			g_Console.writeToBuffer(c, "Enter DOWN ARROW for lower", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

			c.Y = 6;
			c.X = 6;
			g_Console.writeToBuffer(c, "Enter UP ARROW for higher", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

			c.Y = 12;
			c.X = 9;
			g_Console.writeToBuffer(c, "Correct Guesses : ", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

			c.Y = 12;
			c.X = 52;
			g_Console.writeToBuffer(c, "Wrong Guesses : ", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

			c.Y = 23;
			c.X = 25;
			g_Console.writeToBuffer(c, "3 chances to get 5 correct guesses", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

			c.Y = 2;
			c.X = 33;
			g_Console.writeToBuffer(c, "Higher Or Lower", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
			c.Y = 24;
			c.X = 28;
			g_Console.writeToBuffer(c, "Numbers are between 0 and 9", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

			c.Y = 4;
			c.X = 50;
			g_Console.writeToBuffer(c, currNum, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

			c.Y = 12;
			c.X = 27;
			g_Console.writeToBuffer(c, correctguesses, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

			c.Y = 12;
			c.X = 68;
			g_Console.writeToBuffer(c, mistakes, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
		}
	}
}

// Jeremiah's code
void delayDisplay()
{
	if (bMinigameDelayed == 0)
	{
		g_dDelayDisplayTime = g_dElapsedTime + 3.00;
		bMinigameDelayed = 1;
	}

	if (g_dElapsedTime >= g_dDelayDisplayTime)
	{
		runMinigame = 1;
		redTrue, blueTrue, yellowTrue, greenTrue = 0;
		indicatorTrue = 0;
	}
}

void initSequence()
{
	if (sequence.empty())
	{
		for (int i = 0; i < 7; i++)
		{
			sequence.push_back(rand() % 4 + 1);
		}
		sequence.push_back(0);
	}
}

void checkSequence()
{
	bool bSomethingHappened = false;
	if (g_dScanTime > g_dElapsedTime)
		return;

	if (inputState == 0)
	{
		switch (sequence[currentIndex])
		{
		case 0:
			redTrue = 0;
			blueTrue = 0;
			yellowTrue = 0;
			greenTrue = 0;
			inputState = 1;
			break;
		case 1:
			redTrue = 1;
			blueTrue = 0;
			yellowTrue = 0;
			greenTrue = 0;
			break;
		case 2:
			redTrue = 0;
			blueTrue = 1;
			yellowTrue = 0;
			greenTrue = 0;
			break;
		case 3:
			redTrue = 0;
			blueTrue = 0;
			yellowTrue = 1;
			greenTrue = 0;
			break;
		case 4:
			redTrue = 0;
			blueTrue = 0;
			yellowTrue = 0;
			greenTrue = 1;
			break;
		}
	}
}

void scanForDisplay()
{
	bool bSomethingHappened = false;
	if (g_dScanTime > g_dElapsedTime)
		return;

	if (currentIndex < 7)
	{
		currentIndex++;
		sequenceNumber++;
		bSomethingHappened = true;
	}
	else
	{
		return;
	}

	if (bSomethingHappened)
	{
		g_dScanTime = g_dElapsedTime + 1.25;
	}
}

void checkInput()
{
	bool bSomethingHappened = false;
	bool bSomethingPressed = false;
	if (g_dSequenceTime > g_dElapsedTime)
		return;

	if (inputState == 1 && inputIndex < 7)
	{
		inputSequence[7] = 0;

		if (g_dButtonColorTime < g_dElapsedTime)
		{
			redTrue = 0;
			blueTrue = 0;
			yellowTrue = 0;
			greenTrue = 0;
		}

		if (g_abKeyPressed[K_1] || g_abKeyPressed[K_N1])
		{
			inputSequence[inputIndex] = 1;
			inputIndex++;
			redTrue = 1;
			bSomethingHappened = true;
			bSomethingPressed = true;
		}

		if (g_abKeyPressed[K_2] || g_abKeyPressed[K_N2])
		{
			inputSequence[inputIndex] = 2;
			inputIndex++;
			blueTrue = 1;
			bSomethingHappened = true;
			bSomethingPressed = true;
		}

		if (g_abKeyPressed[K_3] || g_abKeyPressed[K_N3])
		{
			inputSequence[inputIndex] = 3;
			inputIndex++;
			yellowTrue = 1;
			bSomethingHappened = true;
			bSomethingPressed = true;
		}

		if (g_abKeyPressed[K_4] || g_abKeyPressed[K_N4])
		{
			inputSequence[inputIndex] = 4;
			inputIndex++;
			greenTrue = 1;
			bSomethingHappened = true;
			bSomethingPressed = true;
		}

		if (bSomethingPressed)
		{
			g_dButtonColorTime = g_dElapsedTime + 0.150;
		}
	}



	if (inputIndex >= 7)
	{
		inputState = 2;
	}

	if (bSomethingHappened)
	{
		g_dSequenceTime = g_dElapsedTime + 0.150;
	}
}

void cleanMinigame()
{
	redTrue = 0;
	blueTrue = 0;
	yellowTrue = 0;
	greenTrue = 0;
	currentIndex = 0;
	sequenceNumber = 0;
	inputState = 0;
	inputSequence[8] = { 0, };
	inputIndex = 0;
	sequence.clear();
	miniGameCorrectOrNot = 0;
	runMinigame = 0;
	bMinigameDelayed = 0;
	indicatorTrue = 1;
}

void compareArray()
{
	if (inputState == 2)
	{
		for (int i = 0; i < 7; i++)
		{
			// if player enters the sequence correctly
			if (inputSequence[i] == sequence[i])
			{
				miniGameCorrectOrNot++;
			}
		}

		if (miniGameCorrectOrNot == 7)
		{
			cleanMinigame();
			whichMap = 6;
			mapIdentifier();
		}
		else
		{
			cleanMinigame();
			g_eGameState = S_DEATH;
		}
	}
}

void minigameLogic()
{
	switch (runMinigame)
	{
	case 0:
		delayDisplay();
		break;
	case 1:
		initSequence();
		checkSequence();
		scanForDisplay();
		checkInput();
		compareArray();
		break;
	}
}

void renderMinigame()
{
	mapLoading(mini); //shift out

	COORD c;

	for (unsigned int row = 0; row < 25; ++row)
	{
		c.Y = row;
		for (unsigned int column = 0; column < 80; ++column)
		{
			c.X = column;
			switch (myArray[c.Y][c.X])
			{
			case '!':
				/*switch (redTrue)
				{
				case 0:
					g_Console.writeToBuffer(c, 219, 0x44);
					break;
				case 1:
					g_Console.writeToBuffer(c, 219, 0xCC);
					break;
				case 2:
					g_Console.writeToBuffer(c, 219, 0x44);
					break;
				}*/
				g_Console.writeToBuffer(c, 219, 0x44);
				break;

			case '-':
				switch (redTrue)
				{
				case 0:
					g_Console.writeToBuffer(c, 219, 0x00);
					break;
				case 1:
					g_Console.writeToBuffer(c, 219, 0xFF);
					break;
				case 2:
					g_Console.writeToBuffer(c, 219, 0x00);
					break;
				}
				break;

			case '@':
				/*switch (blueTrue)
				{
				case 0:
					g_Console.writeToBuffer(c, 219, 0x11);
					break;
				case 1:
					g_Console.writeToBuffer(c, 219, 0x99);
					break;
				case 2:
					g_Console.writeToBuffer(c, 219, 0x11);
					break;
				}*/
				g_Console.writeToBuffer(c, 219, 0x11);
				break;

			case '=':
				switch (blueTrue)
				{
				case 0:
					g_Console.writeToBuffer(c, 219, 0x00);
					break;
				case 1:
					g_Console.writeToBuffer(c, 219, 0xFF);
					break;
				case 2:
					g_Console.writeToBuffer(c, 219, 0x00);
					break;
				}
				break;

			case '$':
				/*switch (yellowTrue)
				{
				case 0:
					g_Console.writeToBuffer(c, 219, 0x66);
					break;
				case 1:
					g_Console.writeToBuffer(c, 219, 0xEE);
					break;
				case 2:
					g_Console.writeToBuffer(c, 219, 0x66);
					break;
				}*/
				g_Console.writeToBuffer(c, 219, 0x66);
				break;

			case '+':
				switch (yellowTrue)
				{
				case 0:
					g_Console.writeToBuffer(c, 219, 0x00);
					break;
				case 1:
					g_Console.writeToBuffer(c, 219, 0xFF);
					break;
				case 2:
					g_Console.writeToBuffer(c, 219, 0x00);
					break;
				}
				break;

			case '%':
				/*switch (greenTrue)
				{
				case 0:
					g_Console.writeToBuffer(c, 219, 0x22);
					break;
				case 1:
					g_Console.writeToBuffer(c, 219, 0xAA);
					break;
				case 2:
					g_Console.writeToBuffer(c, 219, 0x22);
					break;
				}*/
				g_Console.writeToBuffer(c, 219, 0x22);
				break;

			case ')':
				switch (greenTrue)
				{
				case 0:
					g_Console.writeToBuffer(c, 219, 0x00);
					break;
				case 1:
					g_Console.writeToBuffer(c, 219, 0xFF);
					break;
				case 2:
					g_Console.writeToBuffer(c, 219, 0x00);
					break;
				}
				break;

			case '(':
				switch (indicatorTrue)
				{
				case 0:
					if (inputState == 0)
					{
						g_Console.writeToBuffer(c, 219, 0xCC);
					}
					else
					{
						g_Console.writeToBuffer(c, 219, 0xAA);
					}
					break;
				case 1:
					g_Console.writeToBuffer(c, 219, 0x44);
					break;
				}
				break;
			case '^':
				if (sequenceNumber == 1)
				{
					g_Console.writeToBuffer(c, '1', FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				else if (sequenceNumber == 2)
				{
					g_Console.writeToBuffer(c, '2', FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				else if (sequenceNumber == 3)
				{
					g_Console.writeToBuffer(c, '3', FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				else if (sequenceNumber == 4)
				{
					g_Console.writeToBuffer(c, '4', FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				else if (sequenceNumber == 5)
				{
					g_Console.writeToBuffer(c, '5', FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				else if (sequenceNumber == 6)
				{
					g_Console.writeToBuffer(c, '6', FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				else if (sequenceNumber == 7)
				{
					g_Console.writeToBuffer(c, '7', FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				}
				break;
			case '#':
				g_Console.writeToBuffer(c, 219, 0x00);
				break;
			default:
				g_Console.writeToBuffer(c, myArray[c.Y][c.X], FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				break;
			}
		}
	}
}