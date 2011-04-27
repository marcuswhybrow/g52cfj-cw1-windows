#include "header.h"

#include "Player.h"

Player::Player(Main *pEngine, int id)
: Actor(pEngine, id)
{
	_colour = 0xff0000;
	SetSpeed(0);
}

Player::~Player() {}

void Player::Draw()
{
	Actor::Draw();
}

void Player::DoUpdate(int iCurrentTime)
{
	CheckKeys(iCurrentTime);
	Actor::DoUpdate(iCurrentTime);
}

void Player::CheckKeys(int iCurrentTime)
{
	int delta = iCurrentTime - _previousTime;

	// Change speed if player presses a key
	if (GetEngine()->IsKeyPressed(SDLK_UP))
		_speedY -= 0.001 * delta;
	if (GetEngine()->IsKeyPressed(SDLK_DOWN))
		_speedY += 0.001 * delta;
	if (GetEngine()->IsKeyPressed(SDLK_LEFT))
		_speedX -= 0.001 * delta;
	if (GetEngine()->IsKeyPressed(SDLK_RIGHT))
		_speedX += 0.001 * delta;
	if (GetEngine()->IsKeyPressed(SDLK_SPACE))
		_speedX = _speedY = 0;
}