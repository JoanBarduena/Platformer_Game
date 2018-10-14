#ifndef __J1FADETOBLACK_H__
#define __J1FADETOBLACK_H__

#include "j1Module.h"
#include "SDL\include\SDL_rect.h"

class j1Fadetoblack : public j1Module
{
public:
	j1Fadetoblack();
	~j1Fadetoblack();

	bool Start();
	bool Update();
	bool Fadetoblack(j1Module* module_off, j1Module* module_on, float time = 1.0f);
	bool IsFading() const;

public:

	enum fade_step
	{
		none,
		fade_to_black,
		fade_from_black
	} current_step = fade_step::none;

	uint window_w, window_h; 

	Uint32 start_time = 0;
	Uint32 total_time = 0;
	SDL_Rect screen;
	j1Module* to_enable = nullptr;
	j1Module* to_disable = nullptr;
};

#endif //__MODULEFADETOBLACK_H__