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

GameMain::GameMain()
: BaseEngine(6), 
_frictionCoefficient(1)
{
	_pGameTileManager = new GameTileManager();
}


/*
Do any setup of back buffer prior to locking the screen buffer
Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
*/
void GameMain::SetupBackgroundBuffer()
{
	FillBackground(0x000000);

	char* data[] = {
		"+--------------+",
		"|      x       |",
		"|         x    |",
		"|              |",
		"|     x        |",
		"|     |        |",
		"|  x  +--      |",
		"|              |",
		"|     ---+x    |",
		"|              |",
		"|              |",
		"+--------------+" };

	// Specify how many tiles wide and high
	_pGameTileManager->SetSize( 16, 12 ); 
	// Set up the tiles
	for ( int x = 0 ; x < 16 ; x++ )
		for ( int y = 0 ; y < 12 ; y++ )
			_pGameTileManager->SetValue(x, y, GetNumber(data[y][x]));

	for ( int y = 0 ; y < 12 ; y++ )
	{
		for ( int x = 0 ; x < 16 ; x++ )
			printf("%d ", _pGameTileManager->GetValue(x,y) );
		printf("\n" );
	}

	// Specify the screen x,y of top left corner
	_pGameTileManager->SetBaseTilesPositionOnScreen(0, 0);

	_pGameTileManager->DrawAllTiles(this, this->GetBackground(), 0, 0, 15, 11);
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
	}
}


/*
In here you need to create any movable objects that you wish to use.
Sub-classes need to implement this function.
*/
int GameMain::InitialiseObjects()
{
	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	DrawableObjectsChanged();

	// Destroy any existing objects
	DestroyOldObjects();

	int numInfected = 300;
	int numDisplayableObjects = numInfected + 1;

	Player *pPlayer = new Player(this, numInfected);

	// Seed the random numbers generated for placing infected randomly
	srand(time(NULL));

	// You MUST set the array entry after the last one that you create to NULL, so that the system knows when to stop.
	// i.e. The LAST entry has to be NULL. The fact that it is NULL is used in order to work out where the end of the array is.
	for (int i = 0; i < numInfected; i++)
		_actors.push_back(new Infected(this, i, pPlayer));
	_actors.push_back(pPlayer);

	// Create an array one element larger than the number of objects that you want.
	m_ppDisplayableObjects = new DisplayableObject*[_actors.size() + 1];
	
	int i = 0;
	for (vector<Actor*>::iterator it = _actors.begin(); it != _actors.end(); it++)
		m_ppDisplayableObjects[i++] = *it;
	
	m_ppDisplayableObjects[_actors.size()] = NULL;

	return 0;
}


/* Draw text labels */
void GameMain::DrawStrings()
{
	CopyBackgroundPixels( 0, 0, GetScreenWidth(), 30 );
	char buf[128];
	sprintf( buf, "Changing text %d %d\n", rand(), rand() );
	DrawScreenString( 150, 10, buf, 0xffffff, NULL );
	SetNextUpdateRect( 0/*X*/, 0/*Y*/, GetScreenWidth(), 30/*Height*/ );
}


/* Overridden GameAction to ensure that objects use the modified time */
void GameMain::GameAction()
{
	// If too early to act then do nothing
	if ( !TimeToAct() )
		return;

	// Don't act for another 10 ticks
	SetTimeToAct( 1 );

	UpdateAllObjects( GetTime() );
}


// Override to add a node at specified point
void GameMain::MouseDown( int iButton, int iX, int iY )
{
	SetupBackgroundBuffer();
	Redraw(true); // Force total redraw
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
		SetExitWithCode( 0 );
		break;
	case SDLK_SPACE: // SPACE Pauses
		break;
	}
}

void GameMain::RemoveActor(Actor *pActor)
{
	//pActor->SetVisible(false);
}

vector<Actor*>* GameMain::GetActors()
{
	return &_actors;
}

double GameMain::GetFrictionCoefficient()
{
	return _frictionCoefficient;
}
