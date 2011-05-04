#include "header.h"

#include "Player.h"
#include <math.h>

#define PI 3.14159265

Player::Player(GameMain *pGameMain, int id)
: Actor(pGameMain, id),
_powerPowerupEnabled(false),
_speedPowerupEnabled(false),
_spaceIsDown(false),
_powerPowerupColour(0x000000),
_speedPowerupColour(0xffffff)
{
	// The default colour
	_colour = 0xff0000;

	// The max velocity of the player
	_maxVelocity = 0.1;

	// a fallback position if not set by the game correctly
	SetPosition(100,200);
}

void Player::Draw()
{
	if (_powerPowerupEnabled)
		GetEngine()->DrawScreenOval(
			m_iCurrentScreenX - _radius - 3,
			m_iCurrentScreenY - _radius - 3,
			m_iCurrentScreenX + _radius + 3,
			m_iCurrentScreenY + _radius + 3,
			_powerPowerupColour
		);

	Actor::Draw();
}

void Player::DoUpdate(int iCurrentTime)
{
	// Get time since last update
	double delta = iCurrentTime - _previousTime;

	// Get x y components of distance to mouse
	double dx = _pGameMain->GetCurrentMouseX() - _x;
	double dy =  _pGameMain->GetCurrentMouseY() - _y;

	// Euclidean distance to mouse
	double dist = sqrt(pow(dx, 2) + pow(dy, 2));

	// Vary speed based upon distance from mouse
	if (dist > 1 && dist < 50)
		SetSpeed(_maxVelocity * dist / 50);
	else if (dist > 1)
		SetSpeed(_maxVelocity);
	else
		SetSpeed(0);

	// Set angle to point towards the mouse
	int angle = (int) ((atan2(dy, dx) * 180 / PI) + 450) % 360;
	SetAngle(angle);

	CheckPowerups(iCurrentTime);

	CheckKeys(iCurrentTime);
	Actor::DoUpdate(iCurrentTime);
}

void Player::CheckKeys(int iCurrentTime)
{
	int delta = iCurrentTime - _previousTime;

	// Detects if the space bar has been lifted
	bool spaceWasDown = _spaceIsDown;
	_spaceIsDown = GetEngine()->IsKeyPressed(SDLK_SPACE);
	if (spaceWasDown && _spaceIsDown == false)
	{
		// On space bar up, cylce colour of the player
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

void Player::CheckPowerups(int iCurrentTime)
{
	// Detect collision with powerups
	GameTileManager *_pGameTileManager = _pGameMain->GetGameTileManager();
	int tileX = _pGameTileManager->GetTileXForPositionOnScreen(_x);
	int tileY = _pGameTileManager->GetTileYForPositionOnScreen(_y);
	int value = _pGameTileManager->GetValue(tileX, tileY);
	if (value == 6 || value == 7)
	{
		int tileScreenX = tileX * _pGameTileManager->GetTileWidth();
		int tileScreenY = tileY * _pGameTileManager->GetTileHeight();
		double dx = tileScreenX + _pGameTileManager->GetTileWidth() / 2 - _x;
		double dy = tileScreenY + _pGameTileManager->GetTileHeight() / 2 - _y;
		double dist = sqrt(pow(dx, 2) + pow(dy, 2));
		if (dist < _radius + _pGameTileManager->GetPowerupRadius())
		{
			_pGameTileManager->SetValue(tileX, tileY, 3 /* Empty Tile */);
			_pGameTileManager->DrawTileAt(_pGameMain, _pGameMain->GetBackground(), tileX, tileY, tileScreenX, tileScreenY);
			_pGameMain->Redraw(true);
			if (value == 6)
				AddSpeedPowerup();
			if (value == 7)
				AddPowerPowerup();
		}
	}

	// Check for removal of powerups
	if (_speedPowerupEnabled)
	{
		if (iCurrentTime + 1000 > _timeToStopSpeedPowerup)
			_colour = 0xcccccc;
		if (iCurrentTime > _timeToStopSpeedPowerup)
			RemoveSpeedPowerup();
	}

	if (_powerPowerupEnabled)
	{
		if (iCurrentTime + 1000 > _timeToStopPowerPowerup)
			_powerPowerupColour = 0x666666;
		if (iCurrentTime > _timeToStopPowerPowerup)
			RemovePowerPowerup();
	}
}

void Player::HasBeenRemoved()
{
	// If the player gets removed from the game, then end the game.
	_pGameMain->EndGame();
}


void Player::AddSpeedPowerup()
{
	_speedPowerupEnabled = true;
	_savedColour = _colour;
	_colour = _speedPowerupColour;

	_savedMaxVelocity = _maxVelocity;
	_maxVelocity = 0.3;

	_timeToStopSpeedPowerup = _pGameMain->GetTime() + 5000;
}

void Player::RemoveSpeedPowerup()
{
	_speedPowerupEnabled = false;
	_colour = _savedColour;
	_maxVelocity = _savedMaxVelocity;
}


void Player::AddPowerPowerup()
{
	_powerPowerupEnabled = true;
	_timeToStopPowerPowerup = _pGameMain->GetTime() + 10000;
	_radius = _radius - 2;
}

void Player::RemovePowerPowerup()
{
	_powerPowerupEnabled = false;
	_radius = _radius + 2;
}

bool Player::ShouldAttract(Actor *pActor)
{
	if (_powerPowerupEnabled)
		return true;
	else if (_speedPowerupEnabled)
		return false;
	else if (pActor->GetColour() == _colour)
		return true;
	
	return false;
}

bool Player::ShouldScorePoints(Actor *pActor)
{
	if (_powerPowerupEnabled || _speedPowerupEnabled)
		return true;
	else if (pActor->GetColour() != _colour)
		return true;

	return false;
}