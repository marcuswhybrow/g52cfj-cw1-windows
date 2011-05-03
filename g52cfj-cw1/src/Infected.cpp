#include "header.h"
#include "Infected.h"
#include <math.h>
#include <list>

#define PI 3.14159265

Infected::Infected(GameMain *pGameMain, int id, Player *pPlayer)
: Actor(pGameMain, id),
_pPlayer(pPlayer),
_previousLOSDifference(0)
{
	_radius = rand() % 5 + 2;
	switch (rand() % 3)
	{
	case 0:
		_colour = 0xff0000;
		break;
	case 1:
		_colour = 0x00ff00;
		break;
	case 2:
		_colour = 0x0000ff;
		break;
	}
	_acceleration = 0.0001;

	_radiusFallReductionRate = 0.01;
	
	GameTileManager *pGameTileManager = _pGameMain->GetGameTileManager();

	int tileX;
	int tileY;
	
	do
	{
		tileX = rand() % pGameTileManager->GetWidth();
		tileY = rand() % pGameTileManager->GetHeight();
	} while(pGameTileManager->GetValue(tileX, tileY) != 3);

	int offsetX = rand() % pGameTileManager->GetTileWidth();
	int offsetY = rand() % pGameTileManager->GetTileHeight();

	int x = tileX * pGameTileManager->GetTileWidth() + offsetX;
	int y = tileY * pGameTileManager->GetTileHeight() + offsetY;

	SetPosition(x, y);
	_speed = _maxVelocity = 0.05;
}

void Infected::DoUpdate(int iCurrentTime)
{
	double delta = iCurrentTime - _previousTime;

	double dx = _pPlayer->GetX() - _x;
	double dy =  _pPlayer->GetY() - _y;

	double dist = sqrt(pow(dx, 2) + pow(dy, 2));

	if (dist > 2 && dist <= 100)
		SetSpeed(_maxVelocity);
	else if (dist > 100 && dist <= 200)
		SetSpeed(_maxVelocity * (200 - dist) / 100);
	else
		SetSpeed(0);

	int newAngle = (int) ((atan2(dy, dx) * 180 / PI) + 450) % 360;

	if (! _pPlayer->ShouldAttract(this))
		newAngle -= 180;

	SetAngle(newAngle);

	list<Actor*>* pActors = _pGameMain->GetActors();

	for (list<Actor*>::iterator it = pActors->begin(); it != pActors->end(); it++)
		if ((*it)->IsInfectable() && this->IsIntersecting(*it) && _speed > (*it)->GetSpeed())
			(*it)->SetColour(_colour);

	
	if (_pPlayer->IsIntersecting(this) && IsVisible())
		_pGameMain->RemoveActor(this);

	Actor::DoUpdate(iCurrentTime);
}

void Infected::HasBeenRemoved()
{
	if (_pPlayer->GetColour() == _colour)
		_pGameMain->Penalise();
	else
		_pGameMain->AddPoints();
}

bool Infected::IsInfectable()
{
	return true;
}