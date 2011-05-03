#include "header.h"

#include "GameTileManager.h"
#include "Actor.h"

#define POWERUP_RADIUS 7
#define POWERUP_OUTER_RADIUS 9

GameTileManager::GameTileManager()
{
	_pImageData = new ImageData();
}

GameTileManager::~GameTileManager()
{
	delete _pImageData;
}

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
		if (CollisionHorizontalHole(iMapX, iMapY, pActor))
			pActor->SetInHole(iMapX, iMapY);
		return false;
	}
	return false;
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
		if (CollisionVerticalHole(iMapX, iMapY, pActor))
			pActor->SetInHole(iMapX, iMapY);
		return false;
	}
	return false;
}

void GameTileManager::DrawTileAt(
		BaseEngine* pEngine, 
		SDL_Surface* pSurface, 
		int iMapX, int iMapY,
		int iStartPositionScreenX,
		int iStartPositionScreenY
	)
{
	switch(GetValue(iMapX, iMapY))
	{
	case 0: // '|'
		_pImageData->LoadImage("wall-vertical.png");
		_pImageData->RenderImage(
			pEngine->GetBackground(),
			0, 0,
			iStartPositionScreenX, iStartPositionScreenY,
			_pImageData->GetWidth(), _pImageData->GetHeight()
		);
		break;
	case 1: // '-'
		_pImageData->LoadImage("wall-horizontal.png");
		_pImageData->RenderImage(
			pEngine->GetBackground(),
			0, 0,
			iStartPositionScreenX, iStartPositionScreenY,
			_pImageData->GetWidth(), _pImageData->GetHeight()
		);
		break;
	case 2: // '+'
		_pImageData->LoadImage("wall-corner.png");
		_pImageData->RenderImage(
			pEngine->GetBackground(),
			0, 0,
			iStartPositionScreenX, iStartPositionScreenY,
			_pImageData->GetWidth(), _pImageData->GetHeight()
		);
		break;
	case 3: // ' '
	case 5: // 'S' or 'C'
		pEngine->DrawRectangle( 
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			0x999999,
			pSurface
		);
		break;
	case 4: // 'x'
		_pImageData->LoadImage("hole-dark.png");
		_pImageData->RenderImage(
			pEngine->GetBackground(),
			0, 0,
			iStartPositionScreenX, iStartPositionScreenY,
			_pImageData->GetWidth(), _pImageData->GetHeight()
		);
		break;
	case 6: // 'Z'
		pEngine->DrawRectangle( 
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			0x999999,
			pSurface
		);
		pEngine->DrawOval(
			iStartPositionScreenX + GetTileWidth() / 2 - POWERUP_OUTER_RADIUS,
			iStartPositionScreenY + GetTileHeight() / 2 - POWERUP_OUTER_RADIUS,
			iStartPositionScreenX + GetTileWidth() / 2 + POWERUP_OUTER_RADIUS,
			iStartPositionScreenY + GetTileHeight() / 2 + POWERUP_OUTER_RADIUS,
			0x000000,
			pSurface
		);
		pEngine->DrawOval(
			iStartPositionScreenX + GetTileWidth() / 2 - POWERUP_RADIUS,
			iStartPositionScreenY + GetTileHeight() / 2 - POWERUP_RADIUS,
			iStartPositionScreenX + GetTileWidth() / 2 + POWERUP_RADIUS,
			iStartPositionScreenY + GetTileHeight() / 2 + POWERUP_RADIUS,
			0xffffff,
			pSurface
		);
		break;
	case 7: // 'P'
		pEngine->DrawRectangle( 
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			0x999999,
			pSurface
		);
		pEngine->DrawOval(
			iStartPositionScreenX + GetTileWidth() / 2 - POWERUP_OUTER_RADIUS,
			iStartPositionScreenY + GetTileHeight() / 2 - POWERUP_OUTER_RADIUS,
			iStartPositionScreenX + GetTileWidth() / 2 + POWERUP_OUTER_RADIUS,
			iStartPositionScreenY + GetTileHeight() / 2 + POWERUP_OUTER_RADIUS,
			0xffffff,
			pSurface
		);
		pEngine->DrawOval(
			iStartPositionScreenX + GetTileWidth() / 2 - POWERUP_RADIUS,
			iStartPositionScreenY + GetTileHeight() / 2 - POWERUP_RADIUS,
			iStartPositionScreenX + GetTileWidth() / 2 + POWERUP_RADIUS,
			iStartPositionScreenY + GetTileHeight() / 2 + POWERUP_RADIUS,
			0x000000,
			pSurface
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
	int width = GetTileWidth();
	int height = GetTileHeight();
	double actorVelocityY = pActor->GetVelocityY();
	int actorX = pActor->GetX() + pActor->GetVelocityX();
	int actorY = pActor->GetY() + actorVelocityY;
	int actorRadius = pActor->GetRadius();

	return
		actorY + 1 >= iMapY * height &&
		actorY - 1 <= ((iMapY + 1) * height - 1) &&
		actorX >= iMapX * width &&
		actorX <= ((iMapX + 1) * width - 1);
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
	int width = GetTileWidth();
	int height = GetTileHeight();
	double actorVelocityX = pActor->GetVelocityX();
	int actorX = pActor->GetX() + actorVelocityX;
	int actorY = pActor->GetY() + pActor->GetVelocityY();

	return
		actorX + 1 >= iMapX * width &&
		actorX - 1 <= ((iMapX + 1) * width - 1) &&
		actorY >= iMapY * height &&
		actorY <= ((iMapY + 1) * height - 1);
}

int GameTileManager::GetPowerupRadius()
{
	return POWERUP_OUTER_RADIUS;
}