#include "header.h"
#include "Infected.h"
#include <cmath>

#define PI 3.14159265

Infected::Infected(Main *pEngine, int id, Player *pPlayer)
: Actor(pEngine, id),
_pPlayer(pPlayer)
{
	_radius = 3;
	_colour = 0x00ff00;

	double x = abs(rand()) % pEngine->GetScreenWidth();
	double y = abs(rand()) % pEngine->GetScreenHeight();
	SetPosition(x, y);
	SetDirection(rand());
	SetSpeed((rand() % 50 + 50) / 1000.0);
}

Infected::~Infected() {}

void Infected::Draw()
{
	Actor::Draw();
}

void Infected::DoUpdate(int iCurrentTime)
{
	int currentAngle = _angle;
	int preferedAngle = ((int) (atan2(_pPlayer->GetY() - _y, _pPlayer->GetX() - _x) * 180 / PI) + 450) % 360;
	SetDirection(preferedAngle);

	double distanceToPlayer = sqrt(pow(_pPlayer->GetX() - _x, 2) + pow(_pPlayer->GetYCentre() - _y, 2));

	if (distanceToPlayer < (_radius + _pPlayer->GetRadius()))
		_pEngine->RemoveActor(this);

	Actor::DoUpdate(iCurrentTime);
}