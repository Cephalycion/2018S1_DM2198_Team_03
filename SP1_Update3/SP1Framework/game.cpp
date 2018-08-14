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

using namespace std; // don't really know the consequences

double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];

// Game specific variables here
SGameChar   g_sChar;
SMapMonster tutorialMonster;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once
int whichMap = 0;

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

	tutorialMonster.m_mLocation.X = 40;
	tutorialMonster.m_mLocation.Y = 7;
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
        case S_GAME: renderGame();
            break;
    }
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 5.0) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
}

void gameplay()            // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
	moveMonsters();      // moves monster
}

// detect the player movement and make according function calls.
void detection(char x, char y)
{
	if (whichMap == 0)
	{
		// WIP
	}
}

void moveCharacter()
{
    bool bSomethingHappened = false;
    if (g_dBounceTime > g_dElapsedTime)
        return;

    // Updating the location of the character based on the key press
    // providing a beep sound whenver we shift the character
    if (g_abKeyPressed[K_UP] && g_sChar.m_cLocation.Y > 0)
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
    if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
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
        g_sChar.m_bActive = !g_sChar.m_bActive;
        bSomethingHappened = true;
    }

    if (bSomethingHappened)
    {
        // set the bounce time to some time in the future to prevent accidental triggers
        g_dBounceTime = g_dElapsedTime + 0.125; // 125ms should be enough
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
		break;
	case 1:
		MapDataFile.open("Map_01.txt");
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
			default:
				g_Console.writeToBuffer(c, LineData[column], FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED);
				break;
			}
		}
	}
	MapDataFile.close();
}


void renderMonsters()
{
	//WIP
	WORD monsterColor = FOREGROUND_RED;
	switch (whichMap) 
	{
	case 0:
		g_Console.writeToBuffer(tutorialMonster.m_mLocation, (char)219, monsterColor);
		break;
	}
	
}

void moveMonsters()
{
	//Need to make tutorial monster go down 6 and back up 6 in repeat
	tutorialMonster.m_mLocation.Y++;
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
