#include "header.h"

#include "Player.h"
#include <math.h>

#define PI 3.14159265

// -- Constructor --

Player::Player(GameMain *pGameMain)
: Actor(pGameMain),
_powerPowerupEnabled(false),
_speedPowerupEnabled(false),
_spaceIsDown(false),
_powerPowerupColour(0x000000),
_speedPowerupColour(0xffffff)
{
	_colour = 0xff0000;
	_maxVelocity = 0.1;

	// a fallback position if not set by the game correctly
	SetPosition(100,200);
}

// -- Public methods --

void Player::HasBeenRemoved()
{
	// If the player gets removed from the game,
	// end the game.
	_pGameMain->EndGame();
}

bool Player::ShouldScorePoints(Actor *pActor)
{
	if (_powerPowerupEnabled || _speedPowerupEnabled)
		return true;
	else if (pActor->GetColour() != _colour)
		return true;

	return false;
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

// -- Overridden methods --

void Player::CheckKeys(int iCurrentTime)
{
	// Get the amount of time since the last update
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

	// Call to the super call DoUpdate method
	Actor::DoUpdate(iCurrentTime);
}

void Player::Draw()
{
	// This oval is drawn behind the Player ball
	// if the power powerup is enabled.
	// The radius of the Player ball is reduced,
	// and this oval is drawn larger to fill the
	// redraw region creating a border around
	// the Player ball.
	if (_powerPowerupEnabled)
		GetEngine()->DrawScreenOval(
			m_iCurrentScreenX - _radius - 3,
			m_iCurrentScreenY - _radius - 3,
			m_iCurrentScreenX + _radius + 3,
			m_iCurrentScreenY + _radius + 3,
			_powerPowerupColour
		);

	// The Player ball is drawn as a circle in the super call
	Actor::Draw();
}

// -- Private methods --

void Player::AddSpeedPowerup()
{
	// Update the state
	_speedPowerupEnabled = true;

	// Swap the colours over
	_savedColour = _colour;
	_colour = _speedPowerupColour;

	// Swap the velocities
	_savedMaxVelocity = _maxVelocity;
	_maxVelocity = 0.3;

	// Set a time in the future to remove this powerup
	_timeToStopSpeedPowerup = _pGameMain->GetTime() + 5000;
}

void Player::RemoveSpeedPowerup()
{
	// Update the state
	_speedPowerupEnabled = false;

	// Restore the original colour
	_colour = _savedColour;

	// Restore the original velocity
	_maxVelocity = _savedMaxVelocity;
}


void Player::AddPowerPowerup()
{
	// Update the state
	_powerPowerupEnabled = true;

	// Set a time in the future to remove the power powerup
	_timeToStopPowerPowerup = _pGameMain->GetTime() + 10000;

	// Decrease the radius of the Player ball
	// so that when it is drawn against a circle of
	// the Player balls original size, that background
	// circle creates a border effect, within the same
	// redraw region.
	_radius = _radius - 2;
}

void Player::RemovePowerPowerup()
{
	// Update the state
	_powerPowerupEnabled = false;

	// Restore the original radius size
	_radius = _radius + 2;
}

void Player::CheckPowerups(int iCurrentTime)
{
	// Get the tile this Player ball is on
	GameTileManager *_pGameTileManager = _pGameMain->GetGameTileManager();
	int tileX = _pGameTileManager->GetTileXForPositionOnScreen(_x);
	int tileY = _pGameTileManager->GetTileYForPositionOnScreen(_y);
	int value = _pGameTileManager->GetValue(tileX, tileY);

	// If the tile is a powerup (a 6 or 7)
	if (value == 6 || value == 7)
	{
		// Then calculate the coordinates of the tile on screen
		int tileScreenX = tileX * _pGameTileManager->GetTileWidth();
		int tileScreenY = tileY * _pGameTileManager->GetTileHeight();

		// Determine the distance from the center of that tile
		// (which is the center of the powerup circle)
		double dx = tileScreenX + _pGameTileManager->GetTileWidth() / 2 - _x;
		double dy = tileScreenY + _pGameTileManager->GetTileHeight() / 2 - _y;
		double dist = sqrt(pow(dx, 2) + pow(dy, 2));

		// If the powerup circle and and the Player ball are colliding
		if (dist < _radius + _pGameTileManager->GetPowerupRadius())
		{
			// Then Set this tile to be empty since we are
			// consuming this powerup
			_pGameTileManager->SetValue(tileX, tileY, 3 /* Empty Tile */);
			// Instruct the tile manager to draw the tile again
			_pGameTileManager->DrawTileAt(_pGameMain, _pGameMain->GetBackground(), tileX, tileY, tileScreenX, tileScreenY);
			// Efficiently redraw the area of the scren which this tile is in
			_pGameMain->CopyBackgroundPixels(tileScreenX, tileScreenY, _pGameTileManager->GetTileWidth(), _pGameTileManager->GetTileHeight());

			// Add the powerup to the Player which the tile held.
			if (value == 6)
				AddSpeedPowerup();
			if (value == 7)
				AddPowerPowerup();
		}
	}

	if (_speedPowerupEnabled)
	{
		// If there is only 1 second left until this powerup
		// is disabled change the colour to signify that.
		if (iCurrentTime + 1000 > _timeToStopSpeedPowerup)
			_colour = 0xcccccc;

		// If the time has come to disable the powerup,
		// do it.
		if (iCurrentTime >= _timeToStopSpeedPowerup)
			RemoveSpeedPowerup();
	}

	if (_powerPowerupEnabled)
	{
		// If there is only 1 second left until this powerup
		// is disabled change the colour to signify that.
		if (iCurrentTime + 1000 > _timeToStopPowerPowerup)
			_powerPowerupColour = 0x666666;

		// If the time has come to disable the powerup,
		// do it.
		if (iCurrentTime >= _timeToStopPowerPowerup)
			RemovePowerPowerup();
	}
}