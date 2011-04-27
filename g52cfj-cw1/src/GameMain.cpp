#include "header.h"
#include "BaseEngine.h"
#include "GameMain.h"
#include "JPGImage.h"
#include "TileManager.h"
#include "DisplayableObject.h"

#include "Player.h"
#include "Infected.h"
#include <time.h>

/*
Do any setup of back buffer prior to locking the screen buffer
Basically do the drawing of the background in here and it'll be copied to the screen for you as needed
*/
void GameMain::SetupBackgroundBuffer()
{
	FillBackground( 0x000000 );

	//for ( int iX = 0 ; iX < GetScreenWidth() ; iX++ )
	//	for ( int iY = 0 ; iY < this->GetScreenHeight() ; iY++ )
	//		switch( rand()%100 )
	//		{
	//		case 0: SetBackgroundPixel( iX, iY, 0xFF0000 ); break;
	//		case 1: SetBackgroundPixel( iX, iY, 0x00FF00 ); break;
	//		case 2: SetBackgroundPixel( iX, iY, 0x0000FF ); break;
	//		case 3: SetBackgroundPixel( iX, iY, 0xFFFF00 ); break;
	//		case 4: SetBackgroundPixel( iX, iY, 0x00FFFF ); break;
	//		case 5: SetBackgroundPixel( iX, iY, 0xFF00FF ); break;
	//		}
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

	int numInfected = 0;
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