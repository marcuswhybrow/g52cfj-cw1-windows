#ifndef INFECTED_H
#define INFECTED_H

#include "Actor.h"
#include "Player.h"

class Infected : public Actor
{
public:
	Infected(GameMain *pEngine, int id, Player *pPlayer);

	void DoUpdate(int iCurrentTime);

	virtual void HasBeenRemoved();
	virtual bool IsInfectable();

private:
	Player *_pPlayer;
	int _previousLOSDifference;
};

#endif