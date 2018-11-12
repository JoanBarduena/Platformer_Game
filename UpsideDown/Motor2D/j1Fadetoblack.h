#ifndef __j1FADETOBLACK_H__
#define __j1FADETOBLACK_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1Fadetoblack : public j1Module
{
public:
	j1Fadetoblack();

	// Destructor
	~j1Fadetoblack();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update(float dt);

	bool FadeToBlack(j1Module* module_off, j1Module* module_on, float time = 2.0f);
	bool IsFading() const;

	j1Module* ModuleOn;
	j1Module* ModuleOff;

private:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black

	} current_step = fade_step::none;

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
};

#endif //__j1FADETOBLACK_H__
