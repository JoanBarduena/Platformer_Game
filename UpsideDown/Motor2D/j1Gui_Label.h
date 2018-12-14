#ifndef __j1GUI_LABEL_H__
#define __j1GUI_LABEL_H__

#include "j1Gui_Elements.h"

class Gui_Label :public Gui_Elements
{
public:
	Gui_Label(Element_type type, iPoint position, SDL_Rect rect, bool visible, const char* Text, SDL_Color Color, TTF_Font* Font, Gui_Elements* Parent);

	~Gui_Label();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();

public:

	const char* text;
	SDL_Color color;
	TTF_Font* font;

};


#endif //__j1GUI_LABEL_H__