#ifndef GAMETILEMANAGER_H
#define GAMETILEMANAGER_H

#include "TileManager.h"
#include "JPGImage.h"

class Actor;

class GameTileManager : public TileManager
{
public:
	GameTileManager(void);
	~GameTileManager();

	// Getters
	int GetPowerupRadius();
	int GetWidth(); // The number of tiles wide
	int GetHeight(); // The number of tile high
	virtual int GetTileWidth();
	virtual int GetTileHeight();

	// Overriden to specify what should be drawn for
	// different types of tiles given each tiles stored integer value
	virtual void DrawTileAt(
		BaseEngine* pEngine, 
		SDL_Surface* pSurface, 
		int iMapX, int iMapY,
		int iStartPositionScreenX,
		int iStartPositionScreenY
	);
	
	// Compares an Actor agains potential obsticles in the x axis
	// and returns True if it will collide with something
	bool CheckCollisionHorizontal(int iMapX, int iMapY, Actor *pActor);
	// Does the same as the previous method but in the y axis
	bool CheckCollisionVertical(int iMapX, int iMapY, Actor *pActor);
	
private:
	// This reference to an image data instance is used to load in
	// and draw wall and hole images for the wall and hole tiles
	ImageData *_pImageData;

	// These methods encapsulate specific collosion detections algorithms
	// for each type of tile as measured in the x axis
	bool CollisionVerticalWallVertical(int iMapX, int iMapY, Actor *pActor);
	bool CollisionVerticalWallHorizontal(int iMapX, int iMapY, Actor *pActor);
	bool CollisionVerticalWallCorner(int iMapX, int iMapY, Actor *pActor);
	bool CollisionVerticalHole(int iMapX, int iMapY, Actor *pActor);

	// These methods do the same thing as the previous buch but in the y axis
	bool CollisionHorizontalWallVertical(int iMapX, int iMapY, Actor *pActor);
	bool CollisionHorizontalWallHorizontal(int iMapX, int iMapY, Actor *pActor);
	bool CollisionHorizontalWallCorner(int iMapX, int iMapY, Actor *pActor);
	bool CollisionHorizontalHole(int iMapX, int iMapY, Actor *pActor);
};

#endif