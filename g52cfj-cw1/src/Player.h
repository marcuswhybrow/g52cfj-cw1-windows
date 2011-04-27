#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.h"

class Player : public Actor
{
public:
	Player(GameMain *pGameMain, int id);
	virtual ~Player(void);

	void Draw();
	void DoUpdate(int iCurrentTime);

private:
	void CheckKeys(int iCurrentTime);
};

#endif