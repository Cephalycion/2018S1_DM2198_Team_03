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
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once
double  g_dMonsterTime; // for monster
int monsterRenderOrNot = 0;

int whichMap = 0; // to identify current map being played
char myArray[25][80] = { 0, }; // array version of the map
char splashArray[25][80] = { 0, }; // array version of splash screen
int spawnedOrNot = 0; // check if player spawned or not

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
string splash = "Splash_Screen.txt";

int titleChoice = 0; // init to 0
int levelChoice = 0; // init to 0

// Console object
Console g_Console(80, 25, "SP1 Framework");

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

	// initialise with the tutorial map
	mapLoading(m0);
	splashLoading(splash);

	g_sSelectort.m_cLocation.Y = 10;
	g_sSelectort.m_cLocation.X = 29;

	g_sSelectorl.m_cLocation.Y = 10;
	g_sSelectorl.m_cLocation.X = 29;
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
		case S_TITLE: titleLogic(); // logic for title
			break;
		case S_LEVELS: levelsLogic();
			break;
		case S_DEATH: deathScreenWait(); // game logic for the death screen
			break;
        case S_GAME: gameplay(); // gameplay logic when we are in the game
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
		case S_DEATH: deathScreen();
			break;
		case S_TITLE: titleRender();
			break;
		case S_LEVELS: levelsRender();
			break;
        case S_GAME: renderGame();
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

void levelsLogic()
{
	bool bSomethingHappened = false;
	if (g_dBounceTime > g_dElapsedTime)
		return;

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

	if (g_abKeyPressed[K_SPACE])
	{
		whichMap = levelChoice;
		g_eGameState = S_GAME;
	}

	if (bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.1; // 125ms should be enough
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

	if ((g_abKeyPressed[K_SPACE] && titleChoice == 0))
	{
		g_eGameState = S_LEVELS;
	}

	if ((g_abKeyPressed[K_SPACE] && titleChoice == 1))
	{
		; // pending further code for custom map editor
	}

	if (bSomethingHappened)
	{
		// set the bounce time to some time in the future to prevent accidental triggers
		g_dBounceTime = g_dElapsedTime + 0.1; // 125ms should be enough
	}
}

void deathScreenWait()
{
	if (g_abKeyPressed[K_SPACE])
	{
		g_eGameState = S_GAME;
	}

	if (g_abKeyPressed[K_ESCAPE])
	{
		g_bQuitGame = true;
	}
}

void deathReset()
{
	spawnedOrNot = 0;
	g_eGameState = S_DEATH;
	button0 = 0;
	button2 = 0;
	button4 = 0;
	button6 = 0;
	button8 = 0;
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

void gameplay()         // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
	moveMonsters();     // moves monsters
	teleport(); // two-way teleport mechanics

	while (spawnedOrNot != 1)
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

	while (monsterRenderOrNot != 1)
	{
		getMonsterXYType();
		monsterRenderOrNot = 1;
	}

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

	for (int i = 0; i < 50; i++)
	{
		if ((g_sChar.m_cLocation.Y == monsterList[i].m_cLocation.Y) &&
			(g_sChar.m_cLocation.X == monsterList[i].m_cLocation.X))
		{
			deathReset();
		}
	}

	// change the map when reach the end point '*'
	// reset the button states for reuse
	if (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X] == '*')
	{
		whichMap++;
		spawnedOrNot = 0;
		button0 = 0;
		button2 = 0;
		button4 = 0;
		button6 = 0;
		button8 = 0;
		monsterRenderOrNot = 0;

		switch (whichMap)
		{
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
		default:
			break;
		}
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
		if (myArray[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] != '#')
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
		if (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] != '#')
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
		if (myArray[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] != '#')
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
        
		if (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] != '#')
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

void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_abKeyPressed[K_ESCAPE])
        g_bQuitGame = true;    
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x00);
}

void levelsRender()
{
	COORD c = g_Console.getConsoleSize();
	c.Y = 10;
	c.X = 31;
	g_Console.writeToBuffer(c, "LEVEL I", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	c.Y += 2;
	g_Console.writeToBuffer(c, "LEVEL II", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	c.Y += 2;
	g_Console.writeToBuffer(c, "LEVEL III", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	c.Y += 2;
	g_Console.writeToBuffer(c, "LEVEL IV", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	c.Y += 2;
	g_Console.writeToBuffer(c, "LEVEL V", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

	g_Console.writeToBuffer(g_sSelectorl.m_cLocation, '>', FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}

void titleRender()
{
	COORD c = g_Console.getConsoleSize();
	c.Y = 10;
	c.X = 31;
	g_Console.writeToBuffer(c, "LEVEL SELECT", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	c.Y += 2;
	g_Console.writeToBuffer(c, "CUSTOM MAP EDITOR", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

	g_Console.writeToBuffer(g_sSelectort.m_cLocation, '>', FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
}

void deathScreen()
{
	COORD c = g_Console.getConsoleSize();
	c.Y /= 3;
	c.X = c.X / 2 - 11;
	g_Console.writeToBuffer(c, "You should leave. Now.", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	c.Y += 8;
	c.X -= 12;
	g_Console.writeToBuffer(c, "Press the <SPACE> key to restart at checkpoint", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
	c.Y += 1;
	c.X += 5;
	g_Console.writeToBuffer(c, "Press the <ESC> key to quit the game", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
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
