#ifndef ACTOR_H
#define ACTOR_H

#include "DisplayableObject.h"
#include "GameMain.h"

class GameMain;

/*
The Actor class is the base class for all balls which
move in the game.
*/
class Actor : public DisplayableObject
{
public:
	Actor(GameMain *pGameMain);

	// Inherited method overrides
	void Draw();
	void DoUpdate(int iCurrentTime);

	// Setter methods
	void SetPosition(double x, double y);
	void SetSpeed(double speed);
	void SetDirection(int angle);
	void SetColour(int colour);
	void SetPreviousTime(int previousTime);
	void SetAngle(int angle);

	// Getter methods
	double GetX();
	double GetY();
	int GetRadius();
	int GetColour();
	double GetSpeed();
	double GetVelocityX();
	double GetVelocityY();
	virtual bool IsInfectable();

	// Called when this actor is removed from play
	virtual void HasBeenRemoved();

	// Determines if this actor is intersection with pActor
	bool IsIntersecting(Actor *pActor);

	// Instructs this actor to start falling to a hole a tile location iMapX,iMapY
	void SetInHole(int iMapX, int iMapY);

protected:
	// A pointer to the main engine object
	GameMain *_pGameMain;

	// The radius of this actor
	double _radius;
	// The colour of this actor
	int _colour;

	// True if this actor is falling into a hole
	bool _isFallingInHole;
	// The center of the hole this actor might be falling into
	int _holeCenterX;
	int _holeCenterY;
	// Used to make the actors radius decrease giving the illusion
	// of falling into the distance
	double _radiusFallReductionRate;

	double _maxVelocity;

	int _angle;
	double _speed;

	// The velocity of this actor
	double _velocityX;
	double _velocityY;

	// The center of the actor ball (not the top left of the re-draw box)
	double _x;
	double _y;

	// Offsets from the center of the actor ball defining the redraw box
	int _redrawTop;
	int _redrawRight;
	int _redrawBottom;
	int _redrawLeft;

	// The time in milliseconds which this actor last had a call to DoUpdate
	int _previousTime;

private:
	// Sets the m_iCurrentScreen variables inherited from DrawableObject from
	// th emore accurate double versions
	void UpdatePixelPositionFromRealPosition();

	// Takes the angle and speed variables and converts them into an
	// x axis velocity and a y axis velocity.
	void UpdateVelocityComponents();

	// Looks at the tiles underneath and surrounding the ball and handles
	// velocity appropriately such that the ball appears to collide with
	// walls.
	void CheckCollisions();

	// If this ball is falling into a holl, this method will adjust
	// the balls velocity to direct it to the center of the hole,
	// and will eventually remove the ball from play.
	void CheckFallingInHole(int delta);
};

#endif