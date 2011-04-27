#ifndef MAIN_H
#define MAIN_H

#include "BaseEngine.h"
#include "GameTileManager.h"
#include <vector>
using namespace std;

class Actor;

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

	// Handle pressing of a key
	virtual void KeyDown(int iKeyCode);

	void RemoveActor(Actor *pActor);

	vector<Actor*>* GetActors();

	double GetFrictionCoefficient();

protected:
	GameTileManager* _pGameTileManager;

	vector<Actor*> _actors;
	double _frictionCoefficient;
private:
	int GetNumber(char c);
};

#endif
