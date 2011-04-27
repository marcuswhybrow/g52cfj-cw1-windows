#include "header.h"

#include "GameTileManager.h"
#include "JPGImage.h"

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

	ImageData *pWall = new ImageData();

	switch( GetValue(iMapX,iMapY) )
	{
	case 0: // '|'
		pWall->LoadImage("wall-vertical.png");
		pWall->RenderImage(
			pEngine->GetBackground(),
			0, 0,
			iStartPositionScreenX, iStartPositionScreenY,
			pWall->GetWidth(), pWall->GetHeight()
		);
		break;
	case 1: // '-'
		pWall->LoadImage("wall-horizontal.png");
		pWall->RenderImage(
			pEngine->GetBackground(),
			0, 0,
			iStartPositionScreenX, iStartPositionScreenY,
			pWall->GetWidth(), pWall->GetHeight()
		);
		break;
	case 2: // '+'
		pWall->LoadImage("wall-corner.png");
		pWall->RenderImage(
			pEngine->GetBackground(),
			0, 0,
			iStartPositionScreenX, iStartPositionScreenY,
			pWall->GetWidth(), pWall->GetHeight()
		);
		break;
	case 3: // ' '
		pEngine->DrawRectangle( 
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			0x999999,
			pSurface );
		break;
	case 4: // 'x'
		pWall->LoadImage("hole-dark.png");
		pWall->RenderImage(
			pEngine->GetBackground(),
			0, 0,
			iStartPositionScreenX, iStartPositionScreenY,
			pWall->GetWidth(), pWall->GetHeight()
		);
		break;
	}
}