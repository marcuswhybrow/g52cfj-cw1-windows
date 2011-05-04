#include "header.h"

#include "GameTileManager.h"
#include "Actor.h"

#define POWERUP_RADIUS 7
#define POWERUP_OUTER_RADIUS 9

// -- Constructor --

GameTileManager::GameTileManager()
{
	// This reference to an image data instance is used
	// to load images from file and draw them to the screen
	_pImageData = new ImageData();
}

// -- Destructor --

GameTileManager::~GameTileManager()
{
	delete _pImageData;
}

// -- Public methods --

bool GameTileManager::CheckCollisionHorizontal(int iMapX, int iMapY, Actor *pActor)
{
	// If the actor is outside the bounds of the tile
	// then it cannot be intersecting with it
	if (pActor->GetVelocityX() > 0 && pActor->GetX() > iMapX * GetTileWidth() + GetTileWidth() / 2)
		return false;
	if (pActor->GetVelocityX() < 0 && pActor->GetX() <= iMapX *  GetTileWidth() +  GetTileWidth() / 2)
		return false;
	
	// If it is inside the bounds of the tile,
	// then a specific algorithm must be used for
	// each type of tile due to their differing
	// collidable shapes
	switch (GetValue(iMapX, iMapY))
	{
	case 0: // '|' A vertical wall
		return CollisionHorizontalWallVertical(iMapX, iMapY, pActor);
	case 1: // '-' A horizontal wall
		return CollisionHorizontalWallHorizontal(iMapX, iMapY, pActor);
	case 2: // '+' An intersective wall
		return CollisionHorizontalWallCorner(iMapX, iMapY, pActor);
	case 3: // ' ' An empty space
		// You can never collide with an empty sapce
		return false;
	case 4: // 'x' A hole
		// If the actor is colliding with this hole,
		// instruct it to begin falling into the hole
		if (CollisionHorizontalHole(iMapX, iMapY, pActor))
			pActor->SetInHole(iMapX, iMapY);
		return false;
	}
	return false;
}

bool GameTileManager::CheckCollisionVertical(int iMapX, int iMapY, Actor *pActor)
{
	// If the actor is outside the bounds of the tile
	// then it cannot be intersecting with it
	if (pActor->GetVelocityY() > 0 && pActor->GetY() > iMapY * GetTileHeight() + GetTileHeight() / 2)
		return false;
	if (pActor->GetVelocityY() < 0 && pActor->GetY() <= iMapY *  GetTileHeight() +  GetTileHeight() / 2)
		return false;

	// If it is inside the bounds of the tile,
	// then a specific algorithm must be used for
	// each type of tile due to their differing
	// collidable shapes
	switch (GetValue(iMapX, iMapY))
	{
	case 0: // '|' A vertical wall
		return CollisionVerticalWallVertical(iMapX, iMapY, pActor);
	case 1: // '-' A horizontal wall
		return CollisionVerticalWallHorizontal(iMapX, iMapY, pActor);
	case 2: // '+' An intersective wall
		return CollisionVerticalWallCorner(iMapX, iMapY, pActor);
	case 3: // ' ' An empty space
		return false;
	case 4: // 'x' A hole
		// If the actor is colliding with this hole,
		// instruct it to begin falling into the hole
		if (CollisionVerticalHole(iMapX, iMapY, pActor))
			pActor->SetInHole(iMapX, iMapY);
		return false;
	}
	return false;
}

// -- Overriden methods inherited from TileManager

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
	case 0: // '|' A vertial wall
		_pImageData->LoadImage("images/wall-vertical.png");
		_pImageData->RenderImage(
			pEngine->GetBackground(),
			0, 0,
			iStartPositionScreenX, iStartPositionScreenY,
			_pImageData->GetWidth(), _pImageData->GetHeight()
		);
		break;
	case 1: // '-' A horizontal wall
		_pImageData->LoadImage("images/wall-horizontal.png");
		_pImageData->RenderImage(
			pEngine->GetBackground(),
			0, 0,
			iStartPositionScreenX, iStartPositionScreenY,
			_pImageData->GetWidth(), _pImageData->GetHeight()
		);
		break;
	case 2: // '+' An intersective wall
		_pImageData->LoadImage("images/wall-corner.png");
		_pImageData->RenderImage(
			pEngine->GetBackground(),
			0, 0,
			iStartPositionScreenX, iStartPositionScreenY,
			_pImageData->GetWidth(), _pImageData->GetHeight()
		);
		break;
	case 3: // ' ' An empty space
	case 5: // 'S' The starting position of the player or 'C' A tile clear of spawing enemies
		pEngine->DrawRectangle( 
			iStartPositionScreenX,
			iStartPositionScreenY, 
			iStartPositionScreenX + GetTileWidth() - 1,
			iStartPositionScreenY + GetTileHeight() - 1,
			0x999999,
			pSurface
		);
		break;
	case 4: // 'x' A hole
		_pImageData->LoadImage("images/hole-dark.png");
		_pImageData->RenderImage(
			pEngine->GetBackground(),
			0, 0,
			iStartPositionScreenX, iStartPositionScreenY,
			_pImageData->GetWidth(), _pImageData->GetHeight()
		);
		break;
	case 6: // 'Z' A Speed powerup (S was taken alread, so Z stands for Zoom)
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
	case 7: // 'P' A Power powserup
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

// -- Getters --

int GameTileManager::GetWidth() { return m_iMapWidth; }
int GameTileManager::GetHeight() { return m_iMapHeight; }
int GameTileManager::GetTileWidth() { return 50; }
int GameTileManager::GetTileHeight() { return 50; }
int GameTileManager::GetPowerupRadius() { return POWERUP_OUTER_RADIUS; }

// -- Private methods --

bool GameTileManager::CollisionVerticalWallVertical(int iMapX, int iMapY, Actor *pActor)
{
	// Collision detection in the x axis for a vertical wall
	// which is just rectangular
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
	// Collision detection in the x axis for a horizontal wall
	// which is just rectangular
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
	// Collision detection in the x axis for an intersective wall
	// which is the exact shape of given by overlapping the vertical
	// and horizontal wall collision areas.
	return CollisionVerticalWallHorizontal(iMapX, iMapY, pActor) || 
		CollisionVerticalWallVertical(iMapX, iMapY, pActor);
}
bool GameTileManager::CollisionVerticalHole(int iMapX, int iMapY, Actor *pActor)
{
	// Collision detection in the x axis for a hole,
	// which a square the size of the entire tile.
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
	// Collision detection in the y axis for a vertical wall
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
	// Collision detection in the y axis for a horizontal wall
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
	// Collision detection in the y axis for an intersective wall
	// which is has the same collision shape as the overlap of 
	// both the vertical and horizontal walls.
	return CollisionHorizontalWallHorizontal(iMapX, iMapY, pActor) || 
		CollisionHorizontalWallVertical(iMapX, iMapY, pActor);
}
bool GameTileManager::CollisionHorizontalHole(int iMapX, int iMapY, Actor *pActor)
{
	// Collision detection in the y axis for a hole,
	// which is just a square as large as the tile.
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