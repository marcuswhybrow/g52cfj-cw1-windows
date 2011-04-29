#ifndef ACTOR_H
#define ACTOR_H

#include "DisplayableObject.h"
#include "GameMain.h"

class GameMain;

class Actor : public DisplayableObject
{
public:
	Actor(GameMain *pGameMain, int id);

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
	bool IsIntersecting(Actor *pActor);
	void SetAngle(int angle);
	double GetVelocityX();
	double GetVelocityY();
	void SetInHole(int iMapX, int iMapY);

protected:
	// A pointer to the main engine object
	GameMain *_pGameMain;

	// The radius of this actor
	double _radius;
	// The colour of this actor
	int _colour;

	bool _isFallingInHole;
	int _holeCenterX;
	int _holeCenterY;
	double _radiusFallReductionRate;
	
	double _mass;
	double _acceleration;

	double _maxVelocity;

	int _angle;
	double _speed;

	// The velocity of this actor
	double _velocityX;
	double _velocityY;

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

	bool IsIntersecting();
	void UpdateVelocityComponents();
};

#endif