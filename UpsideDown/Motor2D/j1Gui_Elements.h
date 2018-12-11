#ifndef __j1GUI_ELEMENTS_H__
#define __j1GUI_ELEMENTS_H__

#include "j1Gui.h"
#include "SDL/include/SDL.h"
#include "SDL_TTF\include\SDL_ttf.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Input.h"

class Gui_Elements
{
public:

	Gui_Elements(Element_type type, iPoint position, SDL_Texture* tex = nullptr, Function function = Function::NONE);

	virtual ~Gui_Elements();

	virtual bool Awake() { return true; }

	virtual bool Start() { return true; }

	virtual bool PreUpdate() { return true; }

	virtual bool PostUpdate() { return true; }

	virtual bool CleanUp() { return true; }


public:
	Function funct;
	Element_type type;
	iPoint pos;
	SDL_Texture* texture = nullptr;

	bool hovering, clicking_left, clicking_right, do_action;
	iPoint Mouse_Pos;

};

#endif //__j1GUI_ELEMENTS_H__

