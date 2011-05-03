#ifndef INFECTED_H
#define INFECTED_H

#include "Actor.h"
#include "Player.h"

class Infected : public Actor
{
public:
	Infected(GameMain *pEngine, int id, Player *pPlayer);
	virtual ~Infected(void);

	void Draw();
	void DoUpdate(int iCurrentTime);

	virtual void HasBeenRemoved();

private:
	Player *_pPlayer;
	int _previousLOSDifference;
};

#endif