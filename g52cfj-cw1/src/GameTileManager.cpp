#include "header.h"

#include "GameTileManager.h"
#include "JPGImage.h"
#include "Actor.h"

int GameTileManager::GetWidth()
{
	return m_iMapWidth;
}

int GameTileManager::GetHeight()
{
	return m_iMapHeight;
}

int GameTileManager::GetTileWidth()
{
	return 50;
}

int GameTileManager::GetTileHeight()
{
	return 50;
}

bool GameTileManager::CheckCollisionHorizontal(int iMapX, int iMapY, Actor *pActor)
{
	if (pActor->GetVelocityX() > 0 && pActor->GetX() > iMapX * GetTileWidth() + GetTileWidth() / 2)
		return false;
	if (pActor->GetVelocityX() < 0 && pActor->GetX() <= iMapX *  GetTileWidth() +  GetTileWidth() / 2)
		return false;

	switch (GetValue(iMapX, iMapY))
	{
	case 0: // '|'
		return CollisionHorizontalWallVertical(iMapX, iMapY, pActor);
	case 1: // '-'
		return CollisionHorizontalWallHorizontal(iMapX, iMapY, pActor);
	case 2: // '+'
		return CollisionHorizontalWallCorner(iMapX, iMapY, pActor);
	case 3: // ' '
		return false;
	case 4: // 'x'
		return CollisionHorizontalHole(iMapX, iMapY, pActor);
	}
}

bool GameTileManager::CheckCollisionVertical(int iMapX, int iMapY, Actor *pActor)
{
	if (pActor->GetVelocityY() > 0 && pActor->GetY() > iMapY * GetTileHeight() + GetTileHeight() / 2)
		return false;
	if (pActor->GetVelocityY() < 0 && pActor->GetY() <= iMapY *  GetTileHeight() +  GetTileHeight() / 2)
		return false;

	switch (GetValue(iMapX, iMapY))
	{
	case 0: // '|'
		return CollisionVerticalWallVertical(iMapX, iMapY, pActor);
	case 1: // '-'
		return CollisionVerticalWallHorizontal(iMapX, iMapY, pActor);
	case 2: // '+'
		return CollisionVerticalWallCorner(iMapX, iMapY, pActor);
	case 3: // ' '
		return false;
	case 4: // 'x'
		return CollisionVerticalHole(iMapX, iMapY, pActor);;
	}
}

void GameTileManager::DrawTileAt(
		BaseEngine* pEngine, 
		SDL_Surface* pSurface, 
		int iMapX, int iMapY,
		int iStartPositionScreenX,
		int iStartPositionScreenY
	)
{
	ImageData *pWall = new ImageData();

	switch(GetValue(iMapX, iMapY))
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

bool GameTileManager::CollisionVerticalWallVertical(int iMapX, int iMapY, Actor *pActor)
{
	int width = GetTileWidth();
	int height = GetTileHeight();
	double actorVelocityY = pActor->GetVelocityY();
	int actorX = pActor->GetX() + pActor->GetVelocityX();
	int actorY = pActor->GetY() + actorVelocityY;
	int actorRadius = pActor->GetRadius();

	return
		actorY + actorRadius + 2 >= iMapY * height &&
		actorY - actorRadius - 2 <= ((iMapY + 1) * height - 1) &&
		actorX + actorRadius + 1 >= iMapX * width + 5 &&
		actorX - actorRadius - 1 <= ((iMapX + 1) * width - 1) - 5;
}
bool GameTileManager::CollisionVerticalWallHorizontal(int iMapX, int iMapY, Actor *pActor)
{
	int width = GetTileWidth();
	int height = GetTileHeight();
	double actorVelocityY = pActor->GetVelocityY();
	int actorX = pActor->GetX() + pActor->GetVelocityX();
	int actorY = pActor->GetY() + actorVelocityY;
	int actorRadius = pActor->GetRadius();

	return
		actorY + actorRadius + 2 >= iMapY * height + 5 &&
		actorY - actorRadius - 2 <= ((iMapY + 1) * height - 1) - 5 &&
		actorX + actorRadius + 1 >= iMapX * width &&
		actorX - actorRadius - 1 <= ((iMapX + 1) * width - 1);
}
bool GameTileManager::CollisionVerticalWallCorner(int iMapX, int iMapY, Actor *pActor)
{
	return CollisionVerticalWallHorizontal(iMapX, iMapY, pActor) || 
		CollisionVerticalWallVertical(iMapX, iMapY, pActor);
}
bool GameTileManager::CollisionVerticalHole(int iMapX, int iMapY, Actor *pActor)
{
	return false;
}


bool GameTileManager::CollisionHorizontalWallVertical(int iMapX, int iMapY, Actor *pActor)
{
	int width = GetTileWidth();
	int height = GetTileHeight();
	double actorVelocityX = pActor->GetVelocityX();
	int actorX = pActor->GetX() + actorVelocityX;
	int actorY = pActor->GetY() + pActor->GetVelocityY();
	int actorRadius = pActor->GetRadius();

	return
		actorX + actorRadius + 2 >= iMapX * width + 5 &&
		actorX - actorRadius - 2 <= ((iMapX + 1) * width - 1) - 5 &&
		actorY + actorRadius + 1 >= iMapY * height &&
		actorY - actorRadius - 1 <= ((iMapY + 1) * height - 1);
}
bool GameTileManager::CollisionHorizontalWallHorizontal(int iMapX, int iMapY, Actor *pActor)
{
	int width = GetTileWidth();
	int height = GetTileHeight();
	double actorVelocityX = pActor->GetVelocityX();
	int actorX = pActor->GetX() + actorVelocityX;
	int actorY = pActor->GetY() + pActor->GetVelocityY();
	int actorRadius = pActor->GetRadius();

	return
		actorX + actorRadius + 2 >= iMapX * width &&
		actorX - actorRadius - 2 <= ((iMapX + 1) * width - 1) &&
		actorY + actorRadius + 1 >= iMapY * height + 5 &&
		actorY - actorRadius - 1 <= ((iMapY + 1) * height - 1) - 5;
}
bool GameTileManager::CollisionHorizontalWallCorner(int iMapX, int iMapY, Actor *pActor)
{
	return CollisionHorizontalWallHorizontal(iMapX, iMapY, pActor) || 
		CollisionHorizontalWallVertical(iMapX, iMapY, pActor);
}
bool GameTileManager::CollisionHorizontalHole(int iMapX, int iMapY, Actor *pActor)
{
	return false;
}