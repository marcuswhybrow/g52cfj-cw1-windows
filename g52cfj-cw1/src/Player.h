#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.h"

class Player : public Actor
{
public:
	Player(GameMain *pGameMain, int id);

	virtual void Draw();
	
	void DoUpdate(int iCurrentTime);
	virtual void HasBeenRemoved();

	bool ShouldAttract(Actor *pActor);
	bool ShouldScorePoints(Actor *pActor);

protected:
	int _timeToStopSpeedPowerup;
	int _timeToStopPowerPowerup;

	int _powerPowerupColour;
	int _speedPowerupColour;
	int _savedColour;
	double _savedMaxVelocity;

	bool _speedPowerupEnabled;
	//bool _speedPowerupVisible;
	bool _powerPowerupEnabled;
	//bool _powerPowerupVisible;

private:
	void CheckKeys(int iCurrentTime);
	bool _spaceIsDown;
	void AddPowerPowerup();
	void AddSpeedPowerup();
	void RemovePowerPowerup();
	void RemoveSpeedPowerup();
	void CheckPowerups(int iCurrentTime);
};

#endif