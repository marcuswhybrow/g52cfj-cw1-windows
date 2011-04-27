#include "header.h"
#include "Actor.h"
#include <math.h>

#define PI 3.14159265

Actor::Actor(Main *pEngine, int id)
: DisplayableObject(pEngine),
_id(id)
{
	_radius = 10;
	_colour = 0xff0000;

	_redrawTop = _redrawBottom = _redrawLeft = _redrawRight = _radius;

	_speed = 0;
	_angle = 0;
	UpdateSpeedComponents();

	_x = _radius + 10;
	_y = _radius + 10;

	m_iStartDrawPosX = - _redrawLeft - 1;
	m_iStartDrawPosY = - _redrawTop - 1;
	m_iDrawWidth = _redrawLeft + _redrawRight + 3;
	m_iDrawHeight = _redrawTop + _redrawBottom + 3;

	m_iPreviousScreenX = m_iCurrentScreenX = _x;
	m_iPreviousScreenY = m_iCurrentScreenY = _y;

	_previousTime = 0;

	SetVisible(true);
}

Actor::~Actor(void) {}

void Actor::Draw()
{
	if (!IsVisible())
		return;
	
	// Draw a circle
	GetEngine()->DrawScreenOval(
		m_iCurrentScreenX - _radius - 1,
		m_iCurrentScreenY - _radius - 1,
		m_iCurrentScreenX + _radius + 1,
		m_iCurrentScreenY + _radius + 1,
		_colour
	);
	
	// This call must be made apparently
	StoreLastScreenPositionAndUpdateRect();
}

void Actor::DoUpdate(int iCurrentTime)
{
	double delta = iCurrentTime - _previousTime;
	_x += _speedX * delta;
	_y += _speedY * delta;

	CheckForBounce();
	UpdatePixelPositionFromRealPosition();
	RedrawObjects();

	_previousTime = iCurrentTime;
}

void Actor::SetPosition(double x, double y)
{
	_x = x;
	_y = y;
	UpdatePixelPositionFromRealPosition();
}

void Actor::SetSpeed(double speed)
{
	_speed = speed;
	UpdateSpeedComponents();
}

void Actor::SetDirection(int angle)
{
	_angle = angle % 360;
	UpdateSpeedComponents();
}

double Actor::GetX()
{
	return _x;
}

double Actor::GetY()
{
	return _y;
}

int Actor::GetRadius()
{
	return _radius;
}

int Actor::GetId()
{
	return _id;
}



void Actor::UpdatePixelPositionFromRealPosition()
{
	// Convert the precise x an y values into integers
	m_iCurrentScreenX = (int)(_x + 0.5);
	m_iCurrentScreenY = (int)(_y + 0.5);
}

void Actor::CheckForBounce()
{
	// Left Side
	if ((_x + m_iStartDrawPosX) < 0)
	{
		_x = - m_iStartDrawPosX;
		if (_speedX < 0)
			SetDirection(360 - _angle);
	}

	// Right Side
	if ((_x + m_iStartDrawPosX + m_iDrawWidth) > (GetEngine()->GetScreenWidth()-1))
	{
		_x = GetEngine()->GetScreenWidth() -1 - m_iStartDrawPosX - m_iDrawWidth;
		if ( _speedX > 0 )
			SetDirection(360 - _angle);
	}

	// Top
	if ((_y + m_iStartDrawPosY) < 0)
	{
		_y = -m_iStartDrawPosY;
		if (_speedY < 0)
			SetDirection(360 - _angle);
	}

	// Bottom
	if ((_y + m_iStartDrawPosY + m_iDrawHeight) > (GetEngine()->GetScreenHeight()-1))
	{
		_y = GetEngine()->GetScreenHeight() -1 - m_iStartDrawPosY - m_iDrawHeight;
		if (_speedY > 0)
			SetDirection(360 - _angle);
	}
}

void Actor::UpdateSpeedComponents()
{
	double radians = ((_angle % 360) - 90) * PI / 180;
	_speedX =  _speed * cos(radians);
	_speedY = _speed * sin(radians);
}