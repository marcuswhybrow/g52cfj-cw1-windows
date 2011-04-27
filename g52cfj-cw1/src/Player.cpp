#include "header.h"

#include "Player.h"

Player::Player(GameMain *pGameMain, int id)
: Actor(pGameMain, id)
{
	_colour = 0xff0000;

	SetPosition(100,100);
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
		_velocityY -= 0.001 * delta;
	if (GetEngine()->IsKeyPressed(SDLK_DOWN))
		_velocityY += 0.001 * delta;
	if (GetEngine()->IsKeyPressed(SDLK_LEFT))
		_velocityX -= 0.001 * delta;
	if (GetEngine()->IsKeyPressed(SDLK_RIGHT))
		_velocityX += 0.001 * delta;
	if (GetEngine()->IsKeyPressed(SDLK_SPACE))
		_velocityX = _velocityY = 0.0;
}