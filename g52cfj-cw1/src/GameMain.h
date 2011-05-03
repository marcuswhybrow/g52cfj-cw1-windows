#ifndef MAIN_H
#define MAIN_H

#include "BaseEngine.h"
#include "GameTileManager.h"
#include <list>
#include "FontManager.h"
using namespace std;

class Actor;
class Player;

class GameMain : public BaseEngine
{
public:
	/**
	Constructor
	*/
	GameMain(void);
	
	// Do any setup of back buffer prior to locking the screen buffer
	// Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
	virtual void SetupBackgroundBuffer();

	// Create any moving objects
	int InitialiseObjects();

	/** Draw any strings */
	void DrawStrings();

	/**
	The game logic - move things and change the state if necessary.
	Must call Redraw(true/false) if anything changes that shows on the screen.
	*/
	virtual void GameAction();

	// Handle pressing of a mouse button
	void MouseDown(int iButton, int iX, int iY);
	void MouseUp(int iButton, int iX, int iY);

	// Handle pressing of a key
	virtual void KeyDown(int iKeyCode);

	void RemoveActor(Actor *pActor);

	list<Actor*>* GetActors();

	double GetFrictionCoefficient();

	GameTileManager* GetGameTileManager();

	void AddKill();

protected:
	GameTileManager* _pGameTileManager;

	list<Actor*> _actors;
	Player *_pPlayer;
	double _frictionCoefficient;

	int _currentLevel;
	char ***_levels;
	bool _levelsLoaded;

	int _kills;
private:
	int GetNumber(char c);

	enum State {INTRO, PLAYING, PAUSED, END_OF_LEVEL, GAME_OVER};
	State _state;

	Font *_titleFont;
	Font *_normalFont;
	Font *_statsFont;

	void ChangeState(State newState);
	void StartLevel(int levelNumber);

	void LoadLevel(char *pathToFile, int level);
};

#endif
