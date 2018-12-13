#ifndef __j1GUI_ELEMENTS_H__
#define __j1GUI_ELEMENTS_H__

#include "j1Gui.h"
#include "SDL/include/SDL.h"
#include "SDL_TTF\include\SDL_ttf.h"
#include "p2Point.h"
#include "j1Render.h"
#include "j1Input.h"
#include "j1Scene.h"

class Gui_Elements
{
public:

	Gui_Elements(Element_type type, iPoint position, SDL_Rect rect, Gui_Elements* Parent = nullptr, SDL_Texture* tex = nullptr);

	virtual ~Gui_Elements();

	virtual bool Awake() { return true; }

	virtual bool Start() { return true; }

	virtual bool PreUpdate() { return true; }

	virtual bool PostUpdate() { return true; }

	virtual bool CleanUp() { return true; }


public:

	Element_type type;

	Function funct;

	iPoint pos;
	iPoint GlobalPos;

	SDL_Texture* texture = nullptr;
	SDL_Rect Rect;

	bool hovering, clicking_left, do_action, dragable, moving;

	Gui_Elements* parent;
	p2List<Gui_Elements*> childrens;
};

#endif //__j1GUI_ELEMENTS_H__