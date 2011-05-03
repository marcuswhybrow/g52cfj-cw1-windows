#include "header.h"

#include "Player.h"
#include <math.h>

#define PI 3.14159265

Player::Player(GameMain *pGameMain, int id)
: Actor(pGameMain, id),
_spaceIsDown(false)
{
	_colour = 0xff0000;
	//_maxVelocity = 0.1;
	_maxVelocity = 0.5;

	SetPosition(100,200);
}

Player::~Player() {}

void Player::Draw()
{
	Actor::Draw();
}

void Player::DoUpdate(int iCurrentTime)
{
	double delta = iCurrentTime - _previousTime;

	double dx = _pGameMain->GetCurrentMouseX() - _x;
	double dy =  _pGameMain->GetCurrentMouseY() - _y;

	double dist = sqrt(pow(dx, 2) + pow(dy, 2));

	if (dist > 1 && dist < 50)
		SetSpeed(_maxVelocity * dist / 50);
	else if (dist > 1)
		SetSpeed(_maxVelocity);
	else
		SetSpeed(0);

	int angle = (int) ((atan2(dy, dx) * 180 / PI) + 450) % 360;
	SetAngle(angle);

	CheckKeys(iCurrentTime);
	Actor::DoUpdate(iCurrentTime);
}

void Player::CheckKeys(int iCurrentTime)
{
	int delta = iCurrentTime - _previousTime;

	bool spaceWasDown = _spaceIsDown;
	_spaceIsDown = GetEngine()->IsKeyPressed(SDLK_SPACE);

	if (spaceWasDown && _spaceIsDown == false)
	{
		switch (_colour)
		{
		case 0xff0000:
			_colour = 0x00ff00;
			break;
		case 0x00ff00:
			_colour = 0x0000ff;
			break;
		case 0x0000ff:
			_colour = 0xff0000;
			break;
		}
	}
}

void Player::HasBeenRemoved()
{
	_pGameMain->EndGame();
}