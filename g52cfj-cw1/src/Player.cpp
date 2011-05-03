#include "header.h"

#include "Player.h"
#include <math.h>

#define PI 3.14159265

Player::Player(GameMain *pGameMain, int id)
: Actor(pGameMain, id)
{
	_colour = 0xffffff;
	_speed = _maxVelocity = 0.1;

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

	// Change speed if player presses a key
	//if (GetEngine()->IsKeyPressed(SDLK_UP))
	//	_velocityY -= 0.001 * delta;
	//if (GetEngine()->IsKeyPressed(SDLK_DOWN))
	//	_velocityY += 0.001 * delta;
	//if (GetEngine()->IsKeyPressed(SDLK_LEFT))
	//	_velocityX -= 0.001 * delta;
	//if (GetEngine()->IsKeyPressed(SDLK_RIGHT))
	//	_velocityX += 0.001 * delta;
	//if (GetEngine()->IsKeyPressed(SDLK_SPACE))
	//	_velocityX = _velocityY = 0.0;
}