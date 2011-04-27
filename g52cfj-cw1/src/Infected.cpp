#include "header.h"
#include "Infected.h"
#include <cmath>

#define PI 3.14159265

Infected::Infected(Main *pEngine, Player *pPlayer)
: Actor(pEngine),
_pPlayer(pPlayer)
{
	_colour = 0x00ff00;

	double x = abs(rand()) % pEngine->GetScreenWidth();
	double y = abs(rand()) % pEngine->GetScreenHeight();
	SetPosition(x, y);
	SetDirection(rand());
	SetSpeed(((rand() % 200) + 100) / 1000.0);
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

	Actor::DoUpdate(iCurrentTime);
}