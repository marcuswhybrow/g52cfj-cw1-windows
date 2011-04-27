#ifndef INFECTED_H
#define INFECTED_H

#include "Actor.h"
#include "Player.h"

class Infected : public Actor
{
public:
	Infected(Main *pEngine, int id, Player *pPlayer);
	virtual ~Infected(void);

	void Draw();
	void DoUpdate(int iCurrentTime);

private:
	Player *_pPlayer;
};

#endif