#if defined(_MSC_VER)
#ifdef _MEMORYLEAKDEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "SDL.h"
#include <SDL_ttf.h>
#else
#include "SDL/SDL.h"
#include <SDL_ttf.h>
#endif
