#include "header.h"

#include "templates.h"

// Needs one of the following #includes, to include the class definition
#include "GameMain.h"

#define BASE_SCREEN_WIDTH 800
#define BASE_SCREEN_HEIGHT 600



int main(int argc, char *argv[])
{
#if defined(_MSC_VER)
#ifdef _MEMORYLEAKDEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	//_CrtSetBreakAlloc(51);
#endif
#endif

	int iResult;

	{
		// Needs just one of the two following lines:
		GameMain oMain;

		iResult = oMain.Initialise("Swarm by Marcus Whybrow", BASE_SCREEN_WIDTH, BASE_SCREEN_HEIGHT, "Cornerstone Regular.ttf", 24);
		iResult = oMain.MainLoop();
		oMain.Deinitialise();
	}

#if defined(_MSC_VER)
#ifdef _MEMORYLEAKDEBUG
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
	_CrtDumpMemoryLeaks();
#endif
#endif

	return iResult;
}
