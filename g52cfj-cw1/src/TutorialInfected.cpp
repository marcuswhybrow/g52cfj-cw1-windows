#include "TutorialInfected.h"

TutorialInfected::TutorialInfected(GameMain *pEngine, Player *pPlayer)
: Infected(pEngine, pPlayer) {}

void TutorialInfected::HasBeenRemoved()
{
	// Start the next level when this Infected
	// is removed from play
	_pGameMain->EndLevel();
}