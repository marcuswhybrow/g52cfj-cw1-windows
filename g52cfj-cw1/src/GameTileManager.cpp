#include "header.h"

#include "GameTileManager.h"

int GameTileManager::GetTileWidth()
{
	return 50;
}

int GameTileManager::GetTileHeight()
{
	return 50;
}

void GameTileManager::DrawTileAt(
		BaseEngine* pEngine, 
		SDL_Surface* pSurface, 
		int iMapX, int iMapY,
		int iStartPositionScreenX,
		int iStartPositionScreenY
	)
{
	//pEngine->DrawRectangle( 
	//	iStartPositionScreenX,
	//	iStartPositionScreenY, 
	//	iStartPositionScreenX + GetTileWidth() - 1,
	//	iStartPositionScreenY + GetTileHeight() - 1,
	//	pEngine->GetColour( 0xffffff ),
	//	pSurface );

	switch( GetValue(iMapX,iMapY) )
	{
	case 0: // '|'
		pEngine->DrawRectangle( 
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			0x888888,
			pSurface );
		break;
	case 1: // '-'
		pEngine->DrawRectangle( 
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			0x888888,
			pSurface );
		break;
	case 2: // '+'
		pEngine->DrawRectangle( 
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			0x888888,
			pSurface );
		break;
	case 3: // ' '
		pEngine->DrawRectangle( 
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			0x666666,
			pSurface );
		break;
	case 4: // 'x'
		pEngine->DrawRectangle( 
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			0x000000,
			pSurface );
		break;
	}
}