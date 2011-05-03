#ifndef TUTORIALINFECTED_H
#define TUTORIALINFECTED_H

#include "Infected.h"

class TutorialInfected : public Infected
{
public:
	TutorialInfected(GameMain *pEngine, int id, Player *pPlayer);

	virtual void HasBeenRemoved();
};

#endif