#include "header.h"
#include "Actor.h"
#include <math.h>

#define PI 3.14159265

Actor::Actor(GameMain *pGameMain, int id)
: DisplayableObject(pGameMain),
_id(id),
_pGameMain(pGameMain),
_mass(200),
_velocityX(0),
_velocityY(0),
_acceleration(10),
_maxVelocity(2)
{
	_radius = 10;
	_colour = 0xff0000;

	_redrawTop = _redrawBottom = _redrawLeft = _redrawRight = _radius;

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
	if (! IsIntersecting())
	{
		double delta = iCurrentTime - _previousTime;
		
		_x += _velocityX * delta;
		_y += _velocityY * delta;

		if (_velocityX != 0 || _velocityY != 0)
		{
			double vSquared = pow(_velocityX, 2) + pow(_velocityY, 2);
			double vLength = sqrt(vSquared);
			double drag = _pGameMain->GetFrictionCoefficient() * vSquared;

			if (vLength != 0)
			{
				double dragX = drag * _velocityX / vLength;
				double dragY = drag * _velocityY / vLength;

				_x -= dragX * delta / _mass;
				_y -= dragY * delta / _mass;
			}
		}
	}

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

bool Actor::IsIntersecting(Actor *pActor)
{
	double distanceToActor = sqrt(pow(pActor->GetX() - _x, 2) + pow(pActor->GetYCentre() - _y, 2));
	return distanceToActor < (_radius + pActor->GetRadius());
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
		if (_velocityX < 0)
			_velocityX = - _velocityX;
	}

	// Right Side
	if ((_x + m_iStartDrawPosX + m_iDrawWidth) > (GetEngine()->GetScreenWidth()-1))
	{
		_x = GetEngine()->GetScreenWidth() -1 - m_iStartDrawPosX - m_iDrawWidth;
		if ( _velocityX > 0 )
			_velocityX = - _velocityX;
	}

	// Top
	if ((_y + m_iStartDrawPosY) < 0)
	{
		_y = -m_iStartDrawPosY;
		if (_velocityY < 0)
			_velocityY = - _velocityY;
	}

	// Bottom
	if ((_y + m_iStartDrawPosY + m_iDrawHeight) > (GetEngine()->GetScreenHeight()-1))
	{
		_y = GetEngine()->GetScreenHeight() -1 - m_iStartDrawPosY - m_iDrawHeight;
		if (_velocityY > 0)
			_velocityY = - _velocityY;
	}
}

bool Actor::IsIntersecting()
{
	bool intersecting = false;

	vector<Actor*> actors = *_pGameMain->GetActors();

	for (vector<Actor*>::iterator it = actors.begin(); it != actors.end(); it++)
	{
		Actor* pActor = *it;
		if(pActor != this && pActor->IsIntersecting(this))
		{
			intersecting = true;
			break;
		}
	}

	return intersecting;
}

void Actor::UpdateVelocityComponents()
{
	double radians = ((_angle % 360) - 90) * PI / 180;
	_velocityX = _speed * cos(radians);
	_velocityY = _speed * sin(radians);
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

