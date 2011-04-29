#include "header.h"
#include "BaseEngine.h"
#include "GameMain.h"
#include "JPGImage.h"
#include "TileManager.h"
#include "DisplayableObject.h"

#include "Player.h"
#include "Infected.h"
#include <time.h>
#include "GameTileManager.h"
#include "FontManager.h"

GameMain::GameMain()
: BaseEngine(6), 
_frictionCoefficient(1)
{
	_pGameTileManager = new GameTileManager();
	_state = INTRO;
	_titleFont = m_oFontManager.GetFont("Acidic.ttf", 120);
	_normalFont = m_oFontManager.GetFont("SUPERPOI_S.ttf", 24);
}


/*
Do any setup of back buffer prior to locking the screen buffer
Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
*/
void GameMain::SetupBackgroundBuffer()
{
	switch (_state)
	{
	case INTRO:
		FillBackground(0x444444);
		break;
	case PLAYING:
		char* data[] = {
			"+--------------+",
			"|      x       |",
			"|         x----+",
			"|              |",
			"|     x        |",
			"|     |        |",
			"|  x  +--      |",
			"|     |        |",
			"|     +--+x    |",
			"|              |",
			"|   +------+   |",
			"+---+------+---+" };

		// Specify how many tiles wide and high
		_pGameTileManager->SetSize( 16, 12 ); 
		// Set up the tiles
		for ( int x = 0 ; x < 16 ; x++ )
			for ( int y = 0 ; y < 12 ; y++ )
				_pGameTileManager->SetValue(x, y, GetNumber(data[y][x]));

		// Specify the screen x,y of top left corner
		_pGameTileManager->SetBaseTilesPositionOnScreen(0, 0);

		_pGameTileManager->DrawAllTiles(this, this->GetBackground(), 0, 0, 15, 11);
		break;
	}
}

int GameMain::GetNumber(char c)
{
	switch(c)
	{
	case '|': return 0;
	case '-': return 1;
	case '+': return 2;
	case ' ': return 3;
	case 'x': return 4;
	default: return 3;
	}
}


/*
In here you need to create any movable objects that you wish to use.
Sub-classes need to implement this function.
*/
int GameMain::InitialiseObjects()
{
	// Seed the random numbers generated for placing infected randomly
	srand(time(NULL));

	return 0;
}


/* Draw text labels */
void GameMain::DrawStrings()
{
	switch (_state)
	{
	case INTRO:
		DrawScreenString(170, 200, "SWARM", 0x000000, _titleFont);
		DrawScreenString(260, 320, "by marcus whybrow", 0x000000, NULL);
		DrawScreenString(275, 420, "CLICK TO START", 0x000000, NULL);
		break;
	case PLAYING:
		CopyBackgroundPixels( 0, 0, GetScreenWidth(), 30 );
		char buf[128];
		sprintf( buf, "Changing text %d %d\n", rand(), rand() );
		DrawScreenString( 150, 10, buf, 0xffffff, NULL );
		SetNextUpdateRect( 0/*X*/, 0/*Y*/, GetScreenWidth(), 30/*Height*/ );
		break;
	}
}


/* Overridden GameAction to ensure that objects use the modified time */
void GameMain::GameAction()
{
	// If too early to act then do nothing
	if (!TimeToAct())
		return;

	// Don't act for another 10 ticks
	SetTimeToAct(1);

	UpdateAllObjects(GetTime());
}

// Override to add a node at specified point
void GameMain::MouseDown( int iButton, int iX, int iY )
{

}

// Override to add a node at specified point
void GameMain::MouseUp(int iButton, int iX, int iY)
{
	switch (_state)
	{
	case INTRO:
		StartLevel(0);
		break;
	}
}

/*
Handle any key presses here.
Note that the objects themselves (e.g. player) may also check whether a key is pressed
*/
void GameMain::KeyDown(int iKeyCode)
{
	switch ( iKeyCode )
	{
	case SDLK_ESCAPE: // End program when escape is pressed
		SetExitWithCode(0);
		break;
	case SDLK_SPACE:
		break;
	}
}

void GameMain::RemoveActor(Actor *pActor)
{
	pActor->SetVisible(false);
}

vector<Actor*>* GameMain::GetActors()
{
	return &_actors;
}

double GameMain::GetFrictionCoefficient()
{
	return _frictionCoefficient;
}

void GameMain::ChangeState(State newState)
{
	_state = newState;

	SetupBackgroundBuffer();
	Redraw(true);
}

void GameMain::StartLevel(int levelNumber)
{
	ChangeState(PLAYING);
	switch (levelNumber)
	{
	case 0:
		// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
		DrawableObjectsChanged();

		// Destroy any existing objects
		DestroyOldObjects();

		int numInfected = 300;
		int numDisplayableObjects = numInfected + 1;

		Player *pPlayer = new Player(this, numInfected);

		for (int i = 0; i < numInfected; i++)
			_actors.push_back(new Infected(this, i, pPlayer));
		_actors.push_back(pPlayer);

		// Create an array one element larger than the number of objects that you want.
		m_ppDisplayableObjects = new DisplayableObject*[_actors.size() + 1];
		
		int i = 0;
		for (vector<Actor*>::iterator it = _actors.begin(); it != _actors.end(); it++)
			m_ppDisplayableObjects[i++] = *it;
		
		m_ppDisplayableObjects[_actors.size()] = NULL;
		break;
	}
}

GameTileManager* GameMain::GetGameTileManager()
{
	return _pGameTileManager;
}