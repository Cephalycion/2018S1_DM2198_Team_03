#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"

using namespace std;

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;

// Enumeration to store the control keys that your game will have
enum EKEYS
{
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_ESCAPE,
    K_SPACE,
    K_COUNT
};

// Enumeration for the different screen states
enum EGAMESTATES
{
	S_DEATH,
    S_SPLASHSCREEN,
	S_ENDSCREEN,
    S_GAME,
    S_COUNT,
	S_TITLE,  // title screen to display after splash screen
	S_LEVELS, // level select screen
	S_CUSTOM  // custom level editor
};

// struct for the game character
struct SGameChar
{
    COORD m_cLocation;
    bool  m_bActive;
};

// struct for the game monster
struct SMapMonster
{
	COORD m_cLocation;
	int m_iDirection;
	char m_cType;
};

void init        ( void );      // initialize your variables, allocate memory, etc
void getInput    ( void );      // get input from player
void update      ( double dt ); // update the game and the state of the game
void render      ( void );      // renders the current state of the game to the console
void shutdown    ( void );      // do clean up, free memory

void splashScreenWait();    // waits for time to pass in splash screen
void gameplay();            // gameplay logic
void moveCharacter();       // moves the character, collision detection, physics, etc
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
void clearScreen();         // clears the current screen and draw from scratch 
void renderSplashScreen();  // renders the splash screen
void renderGame();          // renders the game stuff
void renderMap();           // renders the map to the buffer first
void renderCharacter();     // renders the character into the buffer
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game

void renderMonsters();      // render monsters
void moveMonsters();        // moves monsters
void getMonsterXYType();    // get monster XY

void titleLogic();
void titleRender();
void levelsLogic();
void levelsRender();
void customLogic();
void customRender();
void endLogic();
void endRender();

void deathRender();         // displays death screen
void deathLogic();
void deathReset();

#endif // _GAME_H