#include "header.h"
#include "Infected.h"
#include <math.h>

#define PI 3.14159265

Infected::Infected(GameMain *pGameMain, int id, Player *pPlayer)
: Actor(pGameMain, id),
_pPlayer(pPlayer),
_previousLOSDifference(0)
{
	_radius = rand() % 5 + 2;
	_colour =  0x00dd00;
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

	if (sqrt(pow(dx, 2) + pow(dy, 2)) < 2)
		SetSpeed(0);
	else
		SetSpeed(_maxVelocity);

	int newAngle = (int) ((atan2(dy, dx) * 180 / PI) + 450) % 360;
	SetAngle(newAngle);

	Actor::DoUpdate(iCurrentTime);

	if (_pPlayer->IsIntersecting(this) && IsVisible())
		_pGameMain->RemoveActor(this);
}

void Infected::HasBeenRemoved()
{
	_pGameMain->AddKill();
}