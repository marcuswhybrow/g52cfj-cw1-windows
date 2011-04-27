#ifndef ACTOR_H
#define ACTOR_H

#include "DisplayableObject.h"
#include "Main.h"

class Actor : public DisplayableObject
{
public:
	Actor(Main *pEngine, int id);

	virtual ~Actor(void);
	void Draw();
	void DoUpdate(int iCurrentTime);
	void SetPosition(double x, double y);
	void SetSpeed(double speed);
	void SetDirection(int angle);
	double GetX();
	double GetY();
	int GetRadius();
	int GetId();

protected:
	// A pointer to the main engine object
	Main *_pEngine;

	// The radius of this actor
	int _radius;
	// The colour of this actor
	int _colour;

	double _speed;
	int _angle;

	// The velocity of this actor
	double _speedX;
	double _speedY;

	// The center of the actor (not the top left of the re-draw box)
	double _x;
	double _y;

	int _redrawTop;
	int _redrawRight;
	int _redrawBottom;
	int _redrawLeft;

	int _id;

	int _previousTime;

	void CheckForBounce();
	void UpdatePixelPositionFromRealPosition();

private:
	void UpdateSpeedComponents();
};

#endif