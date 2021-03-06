#include "header.h"
#include "BaseEngine.h"
#include "GameMain.h"
#include "JPGImage.h"
#include "TileManager.h"
#include "DisplayableObject.h"

#include "Player.h"
#include "Infected.h"
#include "TutorialInfected.h"
#include <time.h>
#include "GameTileManager.h"
#include "FontManager.h"
#include <string>
#include <iostream>
#include <fstream>
using namespace std;

// -- Constructor --

GameMain::GameMain()
: BaseEngine(6), 
_currentLevel(1),
_levelsLoaded(false),
_points(0),
_killsThisLevel(0),
_numInfected(9999)
{
	// Create a new instance of the tile manager
	_pGameTileManager = new GameTileManager();

	// Start on the title page
	_state = INTRO;

	// Load in all the custom fonts to be used in the game
	_titleFont = m_oFontManager.GetFont("fonts/Acidic.ttf", 120);
	_normalFont = m_oFontManager.GetFont("fonts/SUPERPOI_S.ttf", 24);
	_statsFont = m_oFontManager.GetFont("fonts/strenuous.ttf", 24);
	_highScoresFont = m_oFontManager.GetFont("fonts/strenuous.ttf", 30);
}

// -- Destructor --

GameMain::~GameMain()
{
	// Deallocate the _levels array which stores the tile layout for each level.
	for(int i = 0; i < NUM_LEVELS; i++)
	{
		for(int j = 0; j < NUM_TILE_ROWS; j++)
			delete [] _levels[i][j];
		delete [] _levels[i];
	}
	delete [] _levels;

	// Deallocate the tile manager reference
	delete _pGameTileManager;

	// deallocate the ararys of x,y starting positions for the player
	delete [] _playerStartX;
	delete [] _playerStartY;

	// If they are still around, delete all of the displayable objects
	if (m_ppDisplayableObjects != NULL)
	{
		_actors.clear();
		delete [] m_ppDisplayableObjects;
		m_ppDisplayableObjects = NULL;
	}
}

// -- Overridden methods from BaseEngine --

int GameMain::InitialiseObjects()
{
	// Seed the random numbers generated for placing infected randomly
	srand(time(NULL));

	_playerStartX = new int[NUM_LEVELS];
	_playerStartY = new int[NUM_LEVELS];

	_levels = new char **[NUM_LEVELS];
	LoadLevel("levels/intro1.txt", 0);
	LoadLevel("levels/intro2.txt", 1);
	LoadLevel("levels/intro3.txt", 2);
	LoadLevel("levels/intro4.txt", 3);
	LoadLevel("levels/intro5.txt", 4);
	LoadLevel("levels/level1.txt", 5);
	LoadLevel("levels/level2.txt", 6);
	LoadLevel("levels/level3.txt", 7);
	_levelsLoaded = true;

	return 0;
}

void GameMain::SetupBackgroundBuffer()
{
	char **data;
	switch (_state)
	{
	case INTRO:
		FillBackground(0x444444);
		break;
	case PLAYING:
		if (! _levelsLoaded)
			break;

		data = _levels[_currentLevel - 1];

		// Specify how many tiles wide and high
		_pGameTileManager->SetSize(NUM_TILE_COLUMNS, NUM_TILE_ROWS); 
		// Set up the tiles
		for ( int x = 0 ; x < NUM_TILE_COLUMNS ; x++ )
			for ( int y = 0 ; y < NUM_TILE_ROWS ; y++ )
				_pGameTileManager->SetValue(x, y, GetNumber(data[y][x]));

		// Specify the screen x,y of top left corner
		_pGameTileManager->SetBaseTilesPositionOnScreen(0, 0);

		_pGameTileManager->DrawAllTiles(this, this->GetBackground(), 0, 0, NUM_TILE_COLUMNS-1, NUM_TILE_ROWS-1);
		break;
	case COMPLETED:
	case GAME_OVER:
	case HIGH_SCORES:
		FillBackground(0x444444);
		FillBackground(0x444444);
		break;
	}
}


void GameMain::KeyUp(int iKeyCode)
{
	switch (iKeyCode)
	{
	case SDLK_s:
		if (_state == INTRO)
			StartLevel(NUM_TRAINING_LEVELS + 1);
		else if (_state == PLAYING)
			EndLevel();
		break;
	case SDLK_h:
		if (_state == INTRO)
			ChangeState(HIGH_SCORES);
	}
}

void GameMain::KeyDown(int iKeyCode)
{
	switch ( iKeyCode )
	{
	case SDLK_ESCAPE: // End program when escape is pressed
		if (_state == INTRO)
			SetExitWithCode(0);
		else
			ChangeState(INTRO);
		break;
	case SDLK_SPACE:
		break;
	}
}

void GameMain::MouseUp(int iButton, int iX, int iY)
{
	switch (_state)
	{
	case INTRO:
		StartLevel(1);
		break;
	case GAME_OVER:
	case COMPLETED:
		_points = 0;
		StartLevel(NUM_TRAINING_LEVELS + 1);
		break;
	}
}

void GameMain::GameAction()
{
	// If too early to act then do nothing
	if (!TimeToAct())
		return;

	// Don't act for another 10 ticks
	SetTimeToAct(1);

	UpdateAllObjects(GetTime());
}

void GameMain::DrawStrings()
{
	char* buf;
	char* message;

	switch (_state)
	{
	case INTRO:
		DrawScreenString(170, 200, "SWARM", 0x001100, _titleFont);
		DrawScreenString(265, 320, "by marcus whybrow", 0x000000, _statsFont);
		DrawScreenString(100, 400, "(Click) Start tutorial and then game.", 0x000000, _statsFont);
		DrawScreenString(100, 430, "(S) Skip Tutorial and play game", 0x000000, _statsFont);
		DrawScreenString(100, 460, "(H) View the highscores list.", 0x000000, _statsFont);
		break;
	case PLAYING:
		CopyBackgroundPixels(0, 0, GetScreenWidth(), 40);
		char score[128];
		sprintf(score, "POINTS: %d", _points);
		char level[128];
		sprintf(level, "LEVEL: %d", _currentLevel);

		DrawScreenString(200, 10, score, 0xffffff, _statsFont);
		DrawScreenString(50, 10, level, 0xffffff, _statsFont);
		SetNextUpdateRect(0, 0, GetScreenWidth(), 40);

		switch (_currentLevel)
		{
		case 1:
			DrawScreenString(75, 460, "Different colours run away from you. Catch", 0x000000, _statsFont);
			DrawScreenString(75, 480, "them to receive +10 points.", 0x000000, _statsFont);
			DrawScreenString(75, 500, "Make contact to progress to the next level.", 0x000000, _statsFont);
			break;
		case 2:
			DrawScreenString(75, 460, "Similar colours chase you, giving a penalty", 0x000000, _statsFont);
			DrawScreenString(75, 480, "of -10 points. Press space to change colour", 0x000000, _statsFont);
			DrawScreenString(75, 500, "and make contact to earn 10 points.", 0x000000, _statsFont);
			break;
		case 3:
			DrawScreenString(75, 460, "Kill colours different to you, by collision", 0x000000, _statsFont);
			DrawScreenString(75, 480, "or by luring them into a hole.", 0x000000, _statsFont);
			break;
		case 4:
			DrawScreenString(75, 470, "The black powerup with a white border is like a", 0x000000, _statsFont);
			DrawScreenString(75, 490, "black hole. When active all colours give points.", 0x000000, _statsFont);
			DrawScreenString(75, 510, "This powerup lasts 10 seconds.", 0x000000, _statsFont);
			break;
		case 5:
			DrawScreenString(75, 520, "The white powerup with a black border gives", 0x000000, _statsFont);
			DrawScreenString(75, 540, "super speed. All enemies give points and run", 0x000000, _statsFont);
			DrawScreenString(75, 560, "away. This powerup lasts 5 seconds.", 0x000000, _statsFont);
			break;
		}
		break;
	case COMPLETED:
		buf = new char[20];
		sprintf(buf, "%d", _points);
		DrawScreenString(50, 470, "You completed every level!", 0x000000, _statsFont);
		DrawScreenString(50, 500, "Total score: ", 0x000000, _statsFont);
		DrawScreenString(240, 500, buf, 0xffffff, _statsFont);
		delete [] buf;
		SetupHighScores();
		DrawScreenString(500, 50, "(Esc) back to Menu", 0x000000, _statsFont);
		DrawScreenString(500, 80, "(Click) Start again", 0x000000, _statsFont);
		break;
	case GAME_OVER:
		buf = new char[20];
		sprintf(buf, "%d", _points);
		DrawScreenString(50, 470, "You died, better luck next time.", 0x000000, _statsFont);
		DrawScreenString(50, 500, "Total score: ", 0x000000, _statsFont);
		DrawScreenString(240, 500, buf, 0xffffff, _statsFont);
		delete [] buf;
		SetupHighScores();
		DrawScreenString(500, 50, "(Esc) back to Menu", 0x000000, _statsFont);
		DrawScreenString(500, 80, "(Click) Start again", 0x000000, _statsFont);
		break;
	case HIGH_SCORES:
		DrawScreenString(500, 50, "(Esc) back to Menu", 0x000000, _statsFont);
		SetupHighScores();
		break;
	}
}


// -- Public methods --

void GameMain::RemoveActor(Actor *pActor)
{
	pActor->SetVisible(false);
	pActor->HasBeenRemoved();
	if (++_killsThisLevel >= _numInfected)
		EndLevel();
}

void GameMain::AddPoints()
{
	_points += 10;
}

void GameMain::Penalise()
{
	_points -= 5;
}

void GameMain::EndLevel()
{
	if (_currentLevel < NUM_LEVELS)
		StartLevel(_currentLevel + 1);
	else
		ChangeState(COMPLETED);
}

void GameMain::EndGame()
{
	ChangeState(GAME_OVER);
}

// -- Getters --

GameTileManager* GameMain::GetGameTileManager() { return _pGameTileManager; }
list<Actor*>* GameMain::GetActors() { return &_actors; }

// -- Private methods --

int GameMain::GetNumber(char c)
{
	switch(c)
	{
	case '|': return 0;
	case '-': return 1;
	case '+': return 2;
	case ' ': return 3;
	case 'x': return 4;
	case 'C':
	case 'S': return 5;
	case 'Z': return 6;
	case 'P': return 7;
	default: return 3;
	}
}

void GameMain::StartLevel(int levelNumber)
{
	_currentLevel = levelNumber;
	_killsThisLevel = 0;

	if (levelNumber == NUM_TRAINING_LEVELS + 1)
		_points = 0;

	ChangeState(PLAYING);

	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	DrawableObjectsChanged();

	if (m_ppDisplayableObjects != NULL)
		delete [] m_ppDisplayableObjects;

	m_iDrawableObjectsChanged = 1;

	// Calls destructors on all actors
	_actors.clear();

	_pPlayer = new Player(this);
	_pPlayer->SetPosition(_playerStartX[levelNumber-1], _playerStartY[levelNumber-1]);
	_pPlayer->SetPreviousTime(GetTime());
	_actors.push_back(_pPlayer);

	switch (levelNumber)
	{
	case 1:
		_actors.push_front(new TutorialInfected(this, _pPlayer));
		_actors.front()->SetColour(0x00ff00);
		_actors.front()->SetPosition(150, 150);
		_actors.front()->SetPreviousTime(GetTime());
		break;
	case 2:
		_actors.push_front(new TutorialInfected(this, _pPlayer));
		_actors.front()->SetColour(0xff0000);
		_actors.front()->SetPosition(150, 150);
		_actors.front()->SetPreviousTime(GetTime());

		_actors.push_front(new TutorialInfected(this, _pPlayer));
		_actors.front()->SetColour(0xff0000);
		_actors.front()->SetPosition(160, 140);
		_actors.front()->SetPreviousTime(GetTime());
		break;
	case 3:
		_actors.push_front(new TutorialInfected(this, _pPlayer));
		_actors.front()->SetColour(0x00ff00);
		_actors.front()->SetPosition(150, 100);
		_actors.front()->SetPreviousTime(GetTime());

		_actors.push_front(new TutorialInfected(this, _pPlayer));
		_actors.front()->SetColour(0x0000ff);
		_actors.front()->SetPosition(160, 90);
		_actors.front()->SetPreviousTime(GetTime());
		break;
	case 4:
	case 5:
		_numInfected = 20;
		for (int i = 0; i < _numInfected; i++)
		{
			_actors.push_front(new Infected(this, _pPlayer));
			_actors.front()->SetPreviousTime(GetTime());
		}
		break;
	case 6:
	case 7:
	case 8:
		_numInfected = 100;
		for (int i = 0; i < _numInfected; i++)
		{
			_actors.push_front(new Infected(this, _pPlayer));
			_actors.front()->SetPreviousTime(GetTime());
		}
		break;
	}

	// Create an array one element larger than the number of objects that you want.
	m_ppDisplayableObjects = new DisplayableObject*[_actors.size() + 1];
	
	int i = 0;
	for (list<Actor*>::iterator it = _actors.begin(); it != _actors.end(); it++)
		m_ppDisplayableObjects[i++] = *it;
	
	m_ppDisplayableObjects[_actors.size()] = NULL;
}

void GameMain::ChangeState(State newState)
{
	if (_state == PLAYING && newState != PLAYING)
	{
		DrawableObjectsChanged();

		m_ppDisplayableObjects = NULL;
		m_iDrawableObjectsChanged = 1;

		// Calls destructors on all actors
		_actors.clear();
	}
	_state = newState;

	SetupBackgroundBuffer();
	Redraw(true);
}

void GameMain::SetupHighScores()
{
	DrawScreenString(50, 50, "High Scores:", 0x000000, _statsFont);

	std::ifstream file("highscores.txt");

	string line;
	int i = 0;
	int highScores[] = {0,0,0,0,0,0,0,0,0,0};
	// Read existing high scores
	while(getline(file, line) && i < 10)
		sscanf(line.c_str(), "%d\n", &highScores[i++]);

	file.close();

	int index = -1;
	if (_state == COMPLETED || _state == GAME_OVER)
	{
		// Insert points into highscores if necessary
		for (int j = 0; j < 10; j++)
		{
			if (_points >= highScores[j])
			{
				for (int k = 9; k > j; k--)
					highScores[k] = highScores[k-1];
				highScores[j] = _points;
				index = j;
				break;
			}
		}
	}

	char *scoreBuf;
	for (int m = 0; m < 10; m++)
	{
		scoreBuf = new char[128];
		sprintf(scoreBuf, "%d", highScores[m]);
		DrawScreenString(70, 100 + 30 * m, scoreBuf, (m == index) ? 0xffffff : 0x000000, _highScoresFont);
		delete [] scoreBuf;
	}

	if (_state == COMPLETED || _state == GAME_OVER)
	{
		std::ofstream fileout("highscores.txt");
		fileout.clear();

		// Write to file again
		char *buf;
		for (int l = 0; l < 10; l++)
		{
			buf = new char[256];
			sprintf(buf, "%d\n", highScores[l]);
			fileout << buf;
			delete [] buf;
		}
		delete [] buf;

		fileout.close();
	}
}

void GameMain::LoadLevel(char *pathToFile, int level)
{
	std::ifstream ifs(pathToFile);
	string line;

	_levels[level] = new char*[NUM_TILE_ROWS];
	int i = 0;
	char* cstring;
	int startIndex;

	_playerStartX[level] = 0;
	_playerStartY[level] = 0;

	while (getline(ifs, line) && i < NUM_TILE_ROWS)
	{
		_levels[level][i] = new char[NUM_TILE_COLUMNS];
		startIndex = strcspn(line.c_str(), "S");
		if(startIndex < NUM_TILE_COLUMNS)
		{
			_playerStartX[level] = startIndex * _pGameTileManager->GetTileWidth() + _pGameTileManager->GetTileWidth() / 2;
			_playerStartY[level] = i * _pGameTileManager->GetTileHeight() + _pGameTileManager->GetTileHeight() / 2;
		}
		strncpy(_levels[level][i++], line.c_str(), NUM_TILE_COLUMNS);
	}
}
