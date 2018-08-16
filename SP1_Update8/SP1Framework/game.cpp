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

//Monsters
SMapMonster tutorialMonster;
SMapMonster level1Monster1;
SMapMonster level1Monster2;
SMapMonster level1Monster3;

EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once
double  g_dMonsterTime; // for monster

int whichMap = 0; // to identify current map being played
char myArray[25][80] = { 0, }; // array version of the map
int spawnedOrNot = 0; // check if player spawned or not
int monsterCounter = 0; // for delay of monster movement

// identify if button has been pressed or not
bool button0 = 0;
bool button2 = 0;
bool button4 = 0;
bool button6 = 0;
bool button8 = 0;

// Console object
Console g_Console(80, 25, "SP1 Framework");

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

	// monsters, direction 0 for down, 1 for up, 2 for left, 3 for right
	tutorialMonster.m_cLocation.X = 40;
	tutorialMonster.m_cLocation.Y = 7;
	tutorialMonster.m_iDirection = 0;
	//tutorialMonster.m_iCounter = 0;
	//tutorialMonster.m_iDistance = 14;

	level1Monster1.m_cLocation.X = 50;
	level1Monster1.m_cLocation.Y = 4;
	level1Monster1.m_iDirection = 0;
	//level1Monster1.m_iCounter = 0;
	//level1Monster1.m_iDistance = 16;

	level1Monster2.m_cLocation.X = 52;
	level1Monster2.m_cLocation.Y = 11;
	level1Monster2.m_iDirection = 1;
	//level1Monster2.m_iCounter = 0;
	//level1Monster2.m_iDistance = 16;

	level1Monster3.m_cLocation.X = 38;
	level1Monster3.m_cLocation.Y = 13;
	level1Monster3.m_iDirection = 3;
	//level1Monster3.m_iCounter = 0;
	//level1Monster3.m_iDistance = 12;

    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"Consolas");
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
        case S_GAME: renderGame();
            break;
    }
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 3.0) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
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

void gameplay()            // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
	moveMonsters();     // moves monsters

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

	switch (whichMap)
	{
	case 0:
		if ((g_sChar.m_cLocation.Y == tutorialMonster.m_cLocation.Y) &&
			(g_sChar.m_cLocation.X == tutorialMonster.m_cLocation.X))
		{
			deathReset();
		}
		break;
	case 1:
		if ((g_sChar.m_cLocation.Y == level1Monster1.m_cLocation.Y) &&
			(g_sChar.m_cLocation.X == level1Monster1.m_cLocation.X))
		{
			deathReset();
		}
		if ((g_sChar.m_cLocation.Y == level1Monster2.m_cLocation.Y) &&
			(g_sChar.m_cLocation.X == level1Monster2.m_cLocation.X))
		{
			deathReset();
		}
		if ((g_sChar.m_cLocation.Y == level1Monster3.m_cLocation.Y) &&
			(g_sChar.m_cLocation.X == level1Monster3.m_cLocation.X))
		{
			deathReset();
		}
		break;
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
	}
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
			if (((button0 == 1) && (myArray[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == '1')) ||
				((button2 == 1) && (myArray[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == '3')) ||
				((button4 == 1) && (myArray[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == '5')) ||
				((button6 == 1) && (myArray[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == '7')) ||
				((button8 == 1) && (myArray[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == '9')))
			{
				g_sChar.m_cLocation.Y--;
			}
			if ((myArray[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == '-') ||
				(myArray[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == '%') ||
				(myArray[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == '/') ||
				(myArray[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == '@') ||
				(myArray[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] == '0') ||
				((myArray[g_sChar.m_cLocation.Y - 1][g_sChar.m_cLocation.X] - 48) % 2 == 0))
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
			if (((button0 == 1) && (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == '1')) ||
				((button2 == 1) && (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == '3')) ||
				((button4 == 1) && (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == '5')) ||
				((button6 == 1) && (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == '7')) ||
				((button8 == 1) && (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == '9')))
			{
				g_sChar.m_cLocation.X--;
			}
			if ((myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == '-') ||
				(myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == '%') ||
				(myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == '/') ||
				(myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == '@') ||
				(myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] == '0') ||
				((myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X - 1] - 48) % 2 == 0))
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
			if (((button0 == 1) && (myArray[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == '1')) ||
				((button2 == 1) && (myArray[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == '3')) ||
				((button4 == 1) && (myArray[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == '5')) ||
				((button6 == 1) && (myArray[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == '7')) ||
				((button8 == 1) && (myArray[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == '9')))
			{
				g_sChar.m_cLocation.Y++;
			}
			if ((myArray[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == '-') ||
				(myArray[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == '%') ||
				(myArray[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == '/') ||
				(myArray[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == '@') ||
				(myArray[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] == '0') ||
				((myArray[g_sChar.m_cLocation.Y + 1][g_sChar.m_cLocation.X] - 48) % 2 == 0))
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
			if (((button0 == 1) && (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == '1')) ||
				((button2 == 1) && (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == '3')) ||
				((button4 == 1) && (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == '5')) ||
				((button6 == 1) && (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == '7')) ||
				((button8 == 1) && (myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == '9')))
			{
				g_sChar.m_cLocation.X++;
			}
			if ((myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == '-') ||
				(myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == '%') ||
				(myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == '/') ||
				(myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == '@') ||
				(myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] == '0') ||
				((myArray[g_sChar.m_cLocation.Y][g_sChar.m_cLocation.X + 1] - 48) % 2 == 0))
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
    COORD c = g_Console.getConsoleSize();

	/* PROTOTYPE SPLASH

    c.Y /= 3;
    c.X = c.X / 2 - 9;
    g_Console.writeToBuffer(c, "A game in 3 seconds", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 20;
    g_Console.writeToBuffer(c, "Press <Space> to change character colour", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 9;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);

	*/

	unsigned int numCol;
	unsigned int numRow;
	string LineData;
	ifstream MapDataFile("Splash_Screen.txt");

	getline(MapDataFile, LineData);
	numCol = stoi(LineData);

	getline(MapDataFile, LineData);
	numRow = stoi(LineData);

	for (unsigned int row = 0; row < numRow; ++row)
	{
		if (!getline(MapDataFile, LineData))
		{
			break;
		}
		c.Y = row;

		for (unsigned int column = 0; (column < numCol) && (column < LineData.length()); ++column)
		{
			c.X = column;

			switch (LineData[column])
			{
			case '#':
				g_Console.writeToBuffer(c, 219, 0x00);
				break;
			default:
				g_Console.writeToBuffer(c, LineData[column], FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				break;
			}
		}
	}
	MapDataFile.close();
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
	renderMonsters();
}

void mapLoader()
{
	;
}

void renderMap()
{
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    COORD c;

	/* FOR REFERENCE ONLY

    for (int i = 0; i < 12; ++i)
    {
        c.X = 5 * i;
        c.Y = i + 1;
        colour(colors[i]);
        g_Console.writeToBuffer(c, " °±²Û", colors[i]);
    }

	*/

	// For map loading
	unsigned int numCol;
	unsigned int numRow;
	string LineData;
	ifstream MapDataFile;

	switch (whichMap)
	{
	case 0:
		MapDataFile.open("Map_Tutorial.txt");

		getline(MapDataFile, LineData);
		numCol = stoi(LineData);

		getline(MapDataFile, LineData);
		numRow = stoi(LineData);

		// continously updating array that acts as a coordinate system. Not very efficient but works.
		for (unsigned int row = 0; row < numRow; ++row)
		{
			if (!getline(MapDataFile, LineData))
			{
				break;
			}

			for (unsigned int column = 0; (column < numCol) && (column < LineData.length()); ++column)
			{
				myArray[row][column] = (char)LineData[column];
			}
		}
		MapDataFile.close();
		MapDataFile.open("Map_Tutorial.txt");
		break;
	case 1:
		MapDataFile.open("Map_01.txt");

		getline(MapDataFile, LineData);
		numCol = stoi(LineData);

		getline(MapDataFile, LineData);
		numRow = stoi(LineData);

		// continously updating array that acts as a coordinate system. Not very efficient but works.
		for (unsigned int row = 0; row < numRow; ++row)
		{
			if (!getline(MapDataFile, LineData))
			{
				break;
			}

			for (unsigned int column = 0; (column < numCol) && (column < LineData.length()); ++column)
			{
				myArray[row][column] = (char)LineData[column];
			}
		}
		MapDataFile.close();
		MapDataFile.open("Map_01.txt");
		break;
	case 2:
		MapDataFile.open("Map_02.txt");

		getline(MapDataFile, LineData);
		numCol = stoi(LineData);

		getline(MapDataFile, LineData);
		numRow = stoi(LineData);

		// continously updating array that acts as a coordinate system. Not very efficient but works.
		for (unsigned int row = 0; row < numRow; ++row)
		{
			if (!getline(MapDataFile, LineData))
			{
				break;
			}

			for (unsigned int column = 0; (column < numCol) && (column < LineData.length()); ++column)
			{
				myArray[row][column] = (char)LineData[column];
			}
		}
		MapDataFile.close();
		MapDataFile.open("Map_02.txt");
		break;
	case 3:
		MapDataFile.open("Map_03.txt");

		getline(MapDataFile, LineData);
		numCol = stoi(LineData);

		getline(MapDataFile, LineData);
		numRow = stoi(LineData);

		// continously updating array that acts as a coordinate system. Not very efficient but works.
		for (unsigned int row = 0; row < numRow; ++row)
		{
			if (!getline(MapDataFile, LineData))
			{
				break;
			}

			for (unsigned int column = 0; (column < numCol) && (column < LineData.length()); ++column)
			{
				myArray[row][column] = (char)LineData[column];
			}
		}
		MapDataFile.close();
		MapDataFile.open("Map_03.txt");
		break;
	default:
		break;
	}

	getline(MapDataFile, LineData);
	numCol = stoi(LineData);

	getline(MapDataFile, LineData);
	numRow = stoi(LineData);

	for (unsigned int row = 0; row < numRow; ++row)
	{
		if (!getline(MapDataFile, LineData))
		{
			break;
		}
		c.Y = row;

		// There is a better way to do this but we need the different characters to mark positions on the maps
		for (unsigned int column = 0; (column < numCol) && (column < LineData.length()); ++column)
		{
			c.X = column;

			switch (LineData[column])
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
			case '/': // The monster path
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				break;

			/* OLD CODE CONTAINING PROTOTYPE CHAR VALUES. FOR REFERENCE ONLY.

			case '(': // Keycard tier 1
				g_Console.writeToBuffer(c, '1', BACKGROUND_RED | BACKGROUND_BLUE);
				break;
			case ')': // Keycard tier 2
				g_Console.writeToBuffer(c, '2', BACKGROUND_RED | BACKGROUND_BLUE);
				break;
			case '0': // Door tier 0
				g_Console.writeToBuffer(c, '0', BACKGROUND_GREEN | BACKGROUND_RED);
				break;
			case '1': // Door tier 1
				g_Console.writeToBuffer(c, '1', BACKGROUND_GREEN | BACKGROUND_RED);
				break;
			case '2': // Door tier 2
				g_Console.writeToBuffer(c, '2', BACKGROUND_GREEN | BACKGROUND_RED);
				break;
			case '$': // Button
				g_Console.writeToBuffer(c, 219, FOREGROUND_GREEN | FOREGROUND_BLUE);
				break;

			*/
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
				g_Console.writeToBuffer(c, LineData[column], FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
				break;
			}

			if ((LineData[column] >= '0') && (LineData[column] <= '9'))
			{
				switch ((LineData[column] - 48) % 2)
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
	MapDataFile.close();
}

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
	}
}

//Function for moving monsters in a circle
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
	}
}

void renderMonsters()
{
	WORD monsterColor = FOREGROUND_RED;
	switch (whichMap)
	{
	case 0:
		g_Console.writeToBuffer(tutorialMonster.m_cLocation, (char)219, monsterColor);
		break;
	case 1:
		g_Console.writeToBuffer(level1Monster1.m_cLocation, (char)219, monsterColor);
		g_Console.writeToBuffer(level1Monster2.m_cLocation, (char)219, monsterColor);
		g_Console.writeToBuffer(level1Monster3.m_cLocation, (char)219, monsterColor);
	}
}

void moveMonsters()
{
	bool bSomethingHappened = false;
	if (g_dMonsterTime > g_dElapsedTime)
		return;

	switch (whichMap)
	{
	case 0:
		moveUpDownMonster(tutorialMonster, bSomethingHappened);

		if (bSomethingHappened)
		{
			g_dMonsterTime = g_dElapsedTime + 0.1;
		}
		break;
	case 1:
		moveUpDownMonster(level1Monster1, bSomethingHappened);
		moveUpDownMonster(level1Monster2, bSomethingHappened);
		moveClockwiseMonster(level1Monster3, bSomethingHappened);

		if (bSomethingHappened)
		{
			g_dMonsterTime = g_dElapsedTime + 0.1;
		}
	}

	/*
	bool bSomethingHappened = false;
	if (g_dMonsterTime > g_dElapsedTime)
		return;

	switch (whichMap)
	{
	case 0:
		if (tutorialMonster.m_iCounter >= 0 && tutorialMonster.m_iCounter < tutorialMonster.m_iDistance/2)
		{
			tutorialMonster.m_cLocation.Y++;
			bSomethingHappened = true;
			tutorialMonster.m_iCounter++;
		}

		if (tutorialMonster.m_iCounter >= tutorialMonster.m_iDistance/2 && tutorialMonster.m_iCounter < tutorialMonster.m_iDistance)
		{
			tutorialMonster.m_cLocation.Y--;
			bSomethingHappened = true;
			tutorialMonster.m_iCounter++;
		}

		if (tutorialMonster.m_iCounter == tutorialMonster.m_iDistance)
		{
			tutorialMonster.m_iCounter = 0;
		}

		if (bSomethingHappened)
		{
			g_dMonsterTime = g_dElapsedTime + 0.2;
		}
		break;
	case 1:
		if (monsterCounter >= 0 && monsterCounter < 8)
		{
			level1Monster1.m_cLocation.Y++;
			level1Monster2.m_cLocation.Y--;
			bSomethingHappened = true;
			monsterCounter++;
		}

		if (monsterCounter >= 8 && monsterCounter < 16)
		{
			level1Monster1.m_cLocation.Y--;
			level1Monster2.m_cLocation.Y++;
			bSomethingHappened = true;
			monsterCounter++;
		}

		if (monsterCounter == 16)
		{
			monsterCounter = 0;
		}

		if (bSomethingHappened)
		{
			g_dMonsterTime = g_dElapsedTime + 0.2;
		}
	}
	*/
}

void renderCharacter()
{
    // Draw the location of the character
	WORD charColor = FOREGROUND_BLUE;
    if (g_sChar.m_bActive)
    {
		charColor = FOREGROUND_BLUE;
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
