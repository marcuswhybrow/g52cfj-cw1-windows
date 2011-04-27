#include "header.h"

#include "Player.h"

Player::Player(Main *pEngine) : Actor(pEngine)
{
	_colour = 0x00ff00;
}

Player::~Player() {}

void Player::Draw()
{
	CheckKeys();

	Actor::Draw();
}

void Player::DoUpdate(int iCurrentTime)
{
	Actor::DoUpdate(iCurrentTime);
}

void Player::CheckKeys()
{
	// Change speed if player presses a key
	if (GetEngine()->IsKeyPressed(SDLK_UP))
		_speedY -= 0.001;
	if (GetEngine()->IsKeyPressed(SDLK_DOWN))
		_speedY += 0.001;
	if (GetEngine()->IsKeyPressed(SDLK_LEFT))
		_speedX -= 0.001;
	if (GetEngine()->IsKeyPressed(SDLK_RIGHT))
		_speedX += 0.001;
	if (GetEngine()->IsKeyPressed(SDLK_SPACE))
		_speedX = _speedY = 0;
}