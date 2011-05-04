#ifndef TUTORIALINFECTED_H
#define TUTORIALINFECTED_H

#include "Infected.h"

/*
This class which subclasses Infected, is used in the
training levels of this game.

The only difference is that when removed from play
it prompts the game to end the level.
*/
class TutorialInfected : public Infected
{
public:
	TutorialInfected(GameMain *pEngine, Player *pPlayer);

	// Overridden to end the game level if this Actor is
	// removed from play
	virtual void HasBeenRemoved();
};

#endif