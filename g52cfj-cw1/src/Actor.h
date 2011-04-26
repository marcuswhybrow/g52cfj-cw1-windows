#include "DisplayableObject.h"
#include "Main.h"

class Actor : public DisplayableObject
{
public:
	Actor(Main *pEngine);

	virtual ~Actor(void);
	void Draw();
	void DoUpdate(int iCurrentTime);

private:
	// A pointer to the main engine object
	Main *_pEngine;

	// The radius of this actor
	int _radius;
	// The colour of this actor
	int _colour;

	// The velocity of this actor
	double _speedX;
	double _speedY;

	// The precise point where this Actor should be
	double _realX;
	double _realY;

	void CheckForBounce();
	void CheckKeys();
};