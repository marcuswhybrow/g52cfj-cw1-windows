#include "TutorialInfected.h"

TutorialInfected::TutorialInfected(GameMain *pEngine, int id, Player *pPlayer)
: Infected(pEngine, id, pPlayer)
{
	// do nothing
}

void TutorialInfected::HasBeenRemoved()
{
	_pGameMain->EndLevel();
}