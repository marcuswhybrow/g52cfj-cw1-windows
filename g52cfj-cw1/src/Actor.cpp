#include "header.h"
#include "Actor.h"
#include <math.h>

#define PI 3.14159265

// Constructor

Actor::Actor(GameMain *pGameMain)
: DisplayableObject(pGameMain),
_pGameMain(pGameMain),
_velocityX(0),
_velocityY(0),
_maxVelocity(2),
_isFallingInHole(false),
_radiusFallReductionRate(0.02),
_radius(10),
_colour(0xff0000)
{
	

	// Start the ball 10 pixels from the top left of the screen by default
	_x = _radius + 10;
	_y = _radius + 10;

	// Specify the redraw box based upon the radius of the ball
	_redrawTop = _redrawBottom = _redrawLeft = _redrawRight = _radius;
	m_iStartDrawPosX = - _redrawLeft - 1;
	m_iStartDrawPosY = - _redrawTop - 1;
	m_iDrawWidth = _redrawLeft + _redrawRight + 3;
	m_iDrawHeight = _redrawTop + _redrawBottom + 3;

	// Also set the important x and y values used for rendering
	m_iPreviousScreenX = m_iCurrentScreenX = _x;
	m_iPreviousScreenY = m_iCurrentScreenY = _y;

	//Set the previous draw time to now
	_previousTime = _pGameMain->GetTime();

	// Ensure this ball is visible by default
	SetVisible(true);
}

// -- Public Methods --

void Actor::Draw()
{
	if (!IsVisible())
		return;

	// Get an integer version of the radius
	int radius = (int) (_radius + 0.5);
	
	// Draw the oval representing the ball
	GetEngine()->DrawScreenOval(
		m_iCurrentScreenX - radius - 1,
		m_iCurrentScreenY - radius - 1,
		m_iCurrentScreenX + radius + 1,
		m_iCurrentScreenY + radius + 1,
		_colour
	);
	
	// Let the engine know about the drawing
	StoreLastScreenPositionAndUpdateRect();
}

void Actor::DoUpdate(int iCurrentTime)
{
	// If a ball is invisibile it is removed from play
	// and therefor does not need to update
	if (!IsVisible())
		return;

	// Calculate the time since the last update
	double delta = iCurrentTime - _previousTime;
	_previousTime = iCurrentTime;

	// If the screen has not redrawn within an acceptable time
	// frame, the screen must have frozen for some reason, so
	// don't redraw, as the delta value will move everything
	// very large distances possible allowing balls to jump
	// over obstacles.
	if (delta > 100)
		return;

	// These two functions do all the work to stop balls moving
	// through walls, and to allow them to fall down holes.
	CheckCollisions();
	CheckFallingInHole(delta);
	
	// Move the ball to its new location after the velocity has been
	// adjusted by the previous method calls.
	_x += _velocityX * delta;
	_y += _velocityY * delta;
	
	UpdatePixelPositionFromRealPosition();
	RedrawObjects();
}

void Actor::SetInHole(int iMapX, int iMapY)
{
	GameTileManager *pGameTileManager = _pGameMain->GetGameTileManager();
	int width = pGameTileManager->GetTileWidth();
	int height = pGameTileManager->GetTileHeight();

	_isFallingInHole = true;
	_holeCenterX = iMapX * width + width / 2;
	_holeCenterY = iMapY * height + height / 2;
}

void Actor::HasBeenRemoved()
{
	// nothing to do in the base class
}

bool Actor::IsIntersecting(Actor *pActor)
{
	double distanceToActor = sqrt(pow(pActor->GetX() - _x, 2) + pow(pActor->GetYCentre() - _y, 2));
	return distanceToActor < (_radius + pActor->GetRadius());
}

// -- Setters --

void Actor::SetPosition(double x, double y)
{
	_x = x;
	_y = y;
	UpdatePixelPositionFromRealPosition();
}

void Actor::SetAngle(int angle)
{
	_angle = angle % 360;
	UpdateVelocityComponents();
}

void Actor::SetSpeed(double speed)
{
	_speed = speed;
	UpdateVelocityComponents();
}

void Actor::SetPreviousTime(int previousTime){
	_previousTime = previousTime;
}

void Actor::SetColour(int colour)
{
	_colour = colour;
}

// -- Getters --

double Actor::GetX() { return _x; }
double Actor::GetY() { return _y; }
double Actor::GetVelocityX() { return _velocityX; }
double Actor::GetVelocityY() { return _velocityY; }
int Actor::GetRadius() { return (int) (_radius + 0.5); }
int Actor::GetColour() { return _colour; }
double Actor::GetSpeed() { return _speed; }
bool Actor::IsInfectable() { return false; }


// -- Private methods --

void Actor::UpdatePixelPositionFromRealPosition()
{
	// Convert the precise x an y values into integers
	m_iCurrentScreenX = (int)(_x + 0.5);
	m_iCurrentScreenY = (int)(_y + 0.5);
}

void Actor::UpdateVelocityComponents()
{
	double radians = ((_angle % 360) - 90) * PI / 180;
	_velocityX = _speed * cos(radians);
	_velocityY = _speed * sin(radians);
}

void Actor::CheckCollisions()
{
	GameTileManager *pGameTileManager = _pGameMain->GetGameTileManager();
	int x = pGameTileManager->GetTileXForPositionOnScreen(_x);
	int y = pGameTileManager->GetTileYForPositionOnScreen(_y);

	int maxWidth = pGameTileManager->GetWidth();
	int maxHeight = pGameTileManager->GetHeight();

	// Check the tile underneath the ball, and every consecutive tile,
	// as these are the tiles which will a ball could be colliding
	for (int i = max(x - 1, 0); i <= min(x + 1, maxWidth - 1); i++)
	{
		for (int j = max(y - 1, 0); j <= min(y + 1 , maxHeight - 1); j++)
		{
			if (pGameTileManager->CheckCollisionHorizontal(i, j, this))
				_velocityX = 0;
			if (pGameTileManager->CheckCollisionVertical(i, j, this))
				_velocityY = 0;
		}
	}
}

void Actor::CheckFallingInHole(int delta)
{
	if (_isFallingInHole)
	{
		// Point the ball towards the center of the hole
		double dx = _holeCenterX - _x;
		double dy =  _holeCenterY - _y;
		int angle = (int) ((atan2(dy, dx) * 180 / PI) + 450) % 360;
		SetAngle(angle);

		// Reduce the radius to make the ball's size shrink over time
		_radius -= _radiusFallReductionRate * delta;

		// Once the ball shrinks out of site, remove it from play
		if (_radius < 0)
			_pGameMain->RemoveActor(this);
		
	}
}