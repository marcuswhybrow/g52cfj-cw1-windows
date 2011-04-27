#ifndef GAMETILEMANAGER_H
#define GAMETILEMANAGER_H

#include "TileManager.h"

class GameTileManager : public TileManager
{
public:
	virtual int GetTileWidth();
	virtual int GetTileHeight();
	virtual void DrawTileAt(
		BaseEngine* pEngine, 
		SDL_Surface* pSurface, 
		int iMapX, int iMapY,
		int iStartPositionScreenX,
		int iStartPositionScreenY
	);
};

#endif