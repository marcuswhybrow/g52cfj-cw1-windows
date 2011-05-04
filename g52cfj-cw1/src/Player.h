#ifndef PLAYER_H
#define PLAYER_H

#include "Actor.h"

class Player : public Actor
{
public:
	Player(GameMain *pGameMain);

	// Inherrited from Actor and then DisplayableObject.
	// The player is draw just like the base class Actor
	// however if one of the two powerups are enabled,
	// it has to be drawn differently.
	virtual void Draw();
	
	// Points the Player ball towards the mouse position,
	// and checks the current tile for powerups which
	// the Player may be intersecting with.
	virtual void DoUpdate(int iCurrentTime);

	// Called when Player is removed from play,
	// When this happedn the game is ended and the game
	// goes to the GAME_OVER state.
	virtual void HasBeenRemoved();

	// Return true if pActor should be attracted to the 
	// player, taking into consideration the colour of
	// both the Actor and Player, and any currently active
	// powerups
	bool ShouldAttract(Actor *pActor);

	// Returns true if when pActor collids with the Player
	// positive points should be awarded to the player
	bool ShouldScorePoints(Actor *pActor);

protected:
	// The exact time in milliseconds from the start
	// of the game to disabled the speed powerup
	int _timeToStopSpeedPowerup;
	// The same as the previous variable but for the
	// power powerup.
	int _timeToStopPowerPowerup;

	// The colour used for the ring drawn around the Player
	// when the power powerup is active
	int _powerPowerupColour;
	// The colour used for the Player ball, when the
	// speed powerup is active
	int _speedPowerupColour;

	// The original colour of the Player ball is stored
	// in here whilst the speed powerup is active.
	int _savedColour;
	// The original max velocity of the Player ball is
	// stored in here whilst the speed powerup is active.
	double _savedMaxVelocity;

	// True if the speed powerup is active
	bool _speedPowerupEnabled;
	// True if the power powerup is active
	bool _powerPowerupEnabled;

private:
	// True if the space key is currently down
	bool _spaceIsDown;

	// Performs checks to determine when the space key
	// has been released, in order to cyclde between the
	// three colours of the Player ball.
	virtual void CheckKeys(int iCurrentTime);

	// Enables the power powerup, and Converts the Player
	// ball into how it needs to look whilst it has in
	// this state.
	void AddPowerPowerup();
	// Does the same as the previous method for the speed
	// powerup.
	void AddSpeedPowerup();

	// Removes the power powerup and returns the Player ball
	// to its normal state.
	void RemovePowerPowerup();
	// Does the same as the previous method for the speed
	// powerup.
	void RemoveSpeedPowerup();

	// Checks the tile which the Player is currently 
	// occupying for a powerup, if one exists checks
	// are made to detect when the Player is intersecting
	// the powerup circle itself (which is at the center
	// of the tile) and then it enables a particular powerup
	// and changes the background tile to and empty space,
	// consuming the powerup.
	void CheckPowerups(int iCurrentTime);
};

#endif