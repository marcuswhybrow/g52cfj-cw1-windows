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

	virtual int GetTileWidth();
	virtual int GetTileHeight();
	virtual void DrawTileAt(
		BaseEngine* pEngine, 
		SDL_Surface* pSurface, 
		int iMapX, int iMapY,
		int iStartPositionScreenX,
		int iStartPositionScreenY
	);
	int GetWidth();
	int GetHeight();

	bool CheckCollisionHorizontal(int iMapX, int iMapY, Actor *pActor);
	bool CheckCollisionVertical(int iMapX, int iMapY, Actor *pActor);
private:
	bool CollisionVerticalWallVertical(int iMapX, int iMapY, Actor *pActor);
	bool CollisionVerticalWallHorizontal(int iMapX, int iMapY, Actor *pActor);
	bool CollisionVerticalWallCorner(int iMapX, int iMapY, Actor *pActor);
	bool CollisionVerticalHole(int iMapX, int iMapY, Actor *pActor);

	bool CollisionHorizontalWallVertical(int iMapX, int iMapY, Actor *pActor);
	bool CollisionHorizontalWallHorizontal(int iMapX, int iMapY, Actor *pActor);
	bool CollisionHorizontalWallCorner(int iMapX, int iMapY, Actor *pActor);
	bool CollisionHorizontalHole(int iMapX, int iMapY, Actor *pActor);

	ImageData *_pImageData;
};

#endif