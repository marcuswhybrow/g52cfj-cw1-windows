#include "header.h"
#include "Infected.h"
#include <math.h>

#define PI 3.14159265

Infected::Infected(GameMain *pGameMain, int id, Player *pPlayer)
: Actor(pGameMain, id),
_pPlayer(pPlayer),
_previousLOSDifference(0)
{
	_radius = 3;
	_colour = 0x00ff00;
	_acceleration = 0.0001;

	double x = abs(rand()) % _pGameMain->GetScreenWidth();
	double y = abs(rand()) % _pGameMain->GetScreenHeight();
	SetPosition(x, y);
	_speed = 0.3;
}

Infected::~Infected() {}

void Infected::Draw()
{
	Actor::Draw();
}

void Infected::DoUpdate(int iCurrentTime)
{
	double delta = iCurrentTime - _previousTime;

	double dx = _pPlayer->GetX() - _x;
	double dy =  _pPlayer->GetY() - _y;

	int LOS = (int) ((atan2(dy, dx) * 180 / PI) + 450) % 360;
	int difference = LOS - _angle;
	
	double rotation = 0.5 * difference;

	SetAngle(_angle + rotation);
	
	//if (_pPlayer->GetY() > _y)
	//	_velocityY += _acceleration * delta;
	//else
	//	_velocityY -= _acceleration * delta;
		
	//int currentAngle = _angle;
	//int preferedAngle = ((int) (atan2(_pPlayer->GetY() - _y, _pPlayer->GetX() - _x) * 180 / PI) + 450) % 360;
	//SetDirection(preferedAngle);

	//double distanceToPlayer = sqrt(pow(_pPlayer->GetX() - _x, 2) + pow(_pPlayer->GetYCentre() - _y, 2));

	//if (distanceToPlayer < (_radius + _pPlayer->GetRadius()))
	//	_pGameMain->RemoveActor(this);

	_previousLOSDifference = difference;

	Actor::DoUpdate(iCurrentTime);
}