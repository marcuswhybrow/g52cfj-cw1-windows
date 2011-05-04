#ifndef INFECTED_H
#define INFECTED_H

#include "Actor.h"
#include "Player.h"

class Infected : public Actor
{
public:
	Infected(GameMain *pEngine, Player *pPlayer);

	// Inherited from Actor and the Displayable Object
	// An Infected trys to always point towards or away
	// from Player, and detects intersections with other
	// Infected for passing on the colour of one Infected
	// onto another.
	virtual void DoUpdate(int iCurrentTime);

	// Called when this infected (coloured) ball
	// is removed from play
	virtual void HasBeenRemoved();

	// Returns true, meaning that this Actor
	// can have its colour changed by another
	// Actor colliding into it.
	virtual bool IsInfectable();

private:
	// A reference to the Player this Infected is
	// either trying to catch or run away from
	Player *_pPlayer;

	void CheckIntersections();
};

#endif