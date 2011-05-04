#ifndef MAIN_H
#define MAIN_H

#include "BaseEngine.h"
#include "GameTileManager.h"
#include <list>
#include "FontManager.h"
using namespace std;

class Actor;
class Player;

#define NUM_LEVELS			8
#define NUM_TRAINING_LEVELS	5
#define NUM_TILE_COLUMNS	16
#define NUM_TILE_ROWS		12

class GameMain : public BaseEngine
{
public:
	GameMain(void);
	~GameMain();

	// -- Overridden methods from BaseEngine --
	
	// Overriden to take control of the background
	virtual void SetupBackgroundBuffer();

	// Defines the objects needed to start the game
	virtual int InitialiseObjects();

	// Overriden to draw text to the screen for different states
	virtual void DrawStrings();

	// Actions to be taken each frame
	virtual void GameAction();

	// Handle mouse clicks
	virtual void MouseUp(int iButton, int iX, int iY);

	// Handle key presses
	virtual void KeyDown(int iKeyCode);
	virtual void KeyUp(int iKeyCode);

public:
	
	// Removes and actor from play
	void RemoveActor(Actor *pActor);
	
	// Getters
	list<Actor*>* GetActors();
	GameTileManager* GetGameTileManager();

	// Gameplay controllers
	void AddPoints();
	void Penalise();
	void EndLevel();
	void EndGame();

	// The states this game can be in
	const enum State {INTRO, PLAYING, COMPLETED, GAME_OVER, HIGH_SCORES};

protected:
	// A reference to the tile manager used whilst in the PLAYING state
	GameTileManager* _pGameTileManager;

	// A list of Actors
	list<Actor*> _actors;

	// The single player ball
	Player *_pPlayer;

	// The level files loaded into memory, this variable
	// will be initialised before the game starts
	char ***_levels;
	// True if the _levels variables has been initialised
	bool _levelsLoaded;

	// The current level being played
	int _currentLevel;
	// The number of points accumulated by the player
	int _points;
	// The number of infected (coloured) balls in this level
	int _numInfected;
	// The number of actors removed from play this level,
	// the level is completed when this number equals _numInfected
	int _killsThisLevel;

	// Arrays of the x and y positions for placing the player
	// in each level.
	int *_playerStartX;
	int *_playerStartY;

private:
	// Returns the numerical representation for use with the tile manager
	// given a level file character
	int GetNumber(char c);

	// The state this game is currently in
	State _state;

	// All the fonts used throughout the game
	Font *_titleFont;
	Font *_normalFont;
	Font *_statsFont;
	Font *_highScoresFont;

	// Allows the game to move into a new state, this method
	// takes care of any cleanup necessary for the previous state
	void ChangeState(State newState);

	// Move the game into the PLAYING state and starting playing
	// a specific level
	void StartLevel(int levelNumber);

	// Load a leve from file to a specific index in the _level variable
	void LoadLevel(char *pathToFile, int level);

	// Renders the high scores list, by reading them from a file
	// and then displaying text on the screen.
	// If called in an end of gameplay state such as GAME_OVER, or
	// COMPLETED, this method will also log that score in the high
	// scores list and write it to file.
	void SetupHighScores();
};

#endif
