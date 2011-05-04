#include "header.h"
#include "Infected.h"
#include <math.h>
#include <list>

#define PI 3.14159265

// -- Constructor --

Infected::Infected(GameMain *pGameMain, Player *pPlayer)
: Actor(pGameMain),
_pPlayer(pPlayer)
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

// -- Public methods --

void Infected::HasBeenRemoved()
{
	if (_pPlayer->ShouldScorePoints(this))
		_pGameMain->AddPoints();
	else
		_pGameMain->Penalise();
}

bool Infected::IsInfectable()
{
	return true;
}

void Infected::DoUpdate(int iCurrentTime)
{
	// Calculate the time since the last update
	double delta = iCurrentTime - _previousTime;

	// The distnace in the x and y axist between the
	// Player and this Infected
	double dx = _pPlayer->GetX() - _x;
	double dy =  _pPlayer->GetY() - _y;

	// The Euclidean distance between the Player and this Infected
	double dist = sqrt(pow(dx, 2) + pow(dy, 2));

	// Set the speed as a function of the distance from the player
	if (dist > 2 && dist <= 100)
		SetSpeed(_maxVelocity);
	else if (dist > 100 && dist <= 200)
		SetSpeed(_maxVelocity * (200 - dist) / 100);
	else
		SetSpeed(0);

	// Calculate the angle required to point towards the player
	int newAngle = (int) ((atan2(dy, dx) * 180 / PI) + 450) % 360;

	// Point the other way if this Infected should be running away
	// from the player
	if (! _pPlayer->ShouldAttract(this))
		newAngle -= 180;

	// Finally set the angle for this Infected
	SetAngle(newAngle);

	CheckIntersections();

	// Make a call to the super classes method
	Actor::DoUpdate(iCurrentTime);
}

// -- Private methods

void Infected::CheckIntersections()
{
	list<Actor*>* pActors = _pGameMain->GetActors();

	// For each other Infected in the level and this Infected
	// check to see if both are intersecting.
	// If they are then set the slower one to the colour of
	// the faster one, almost as if the faster one is bashing
	// its colour into the slower one.
	for (list<Actor*>::iterator it = pActors->begin(); it != pActors->end(); it++)
		if ((*it)->IsInfectable() && this->IsIntersecting(*it) && _speed > (*it)->GetSpeed())
			(*it)->SetColour(_colour);

	// If this Infected is intersecting with the Player then
	// remove if from play
	if (_pPlayer->IsIntersecting(this) && IsVisible())
		_pGameMain->RemoveActor(this);
}