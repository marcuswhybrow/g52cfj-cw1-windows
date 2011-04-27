#include "header.h"

#include "Actor.h"

Actor::Actor(Main *pEngine)
: DisplayableObject(pEngine)
{
	_radius = 10;
	_colour = 0xff0000;

	_speedX = 0.1;
	_speedY = 0.1;

	m_iStartDrawPosX = m_iStartDrawPosY = - _radius;
	m_iDrawWidth = m_iDrawHeight = _radius * 2;

	_realX = m_iPreviousScreenX = m_iCurrentScreenX = - _radius;
	_realY = m_iPreviousScreenY = m_iCurrentScreenY = - _radius;

	SetVisible(true);
}

Actor::~Actor(void) {}

void Actor::Draw()
{
	if (!IsVisible())
		return;
	
	// Draw a circle
	GetEngine()->DrawScreenOval(
		m_iCurrentScreenX - _radius,
		m_iCurrentScreenY - _radius,
		m_iCurrentScreenX + _radius - 2,
		m_iCurrentScreenY + _radius - 2,
		_colour
	);
	
	// This call must be made apparently
	StoreLastScreenPositionAndUpdateRect();
}

void Actor::DoUpdate(int iCurrentTime)
{
	_realX += _speedX;
	_realY += _speedY;

	CheckForBounce();

	// Convert the precise x an y values into integers
	m_iCurrentScreenX = (int)(_realX + 0.5);
	m_iCurrentScreenY = (int)(_realY + 0.5);

	RedrawObjects();
}

void Actor::CheckForBounce()
{
	// Check for bounce
	if ((_realX + m_iStartDrawPosX) < 0)
	{
		_realX = - m_iStartDrawPosX;
		if (_speedX < 0)
			_speedX = - _speedX;
	}
	if ((_realX + m_iStartDrawPosX + m_iDrawWidth) > (GetEngine()->GetScreenWidth()-1))
	{
		_realX = GetEngine()->GetScreenWidth() -1 - m_iStartDrawPosX - m_iDrawWidth;
		if ( _speedX > 0 )
			_speedX = -_speedX;
	}
	if ((_realY + m_iStartDrawPosY) < 0)
	{
		_realY = -m_iStartDrawPosY;
		if (_speedY < 0)
			_speedY = -_speedY;
	}
	if ((_realY + m_iStartDrawPosY + m_iDrawHeight) > (GetEngine()->GetScreenHeight()-1))
	{
		_realY = GetEngine()->GetScreenHeight() -1 - m_iStartDrawPosY - m_iDrawHeight;
		if (_speedY > 0)
			_speedY = -_speedY;
	}
}