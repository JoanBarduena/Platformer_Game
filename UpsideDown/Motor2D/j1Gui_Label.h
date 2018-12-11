#ifndef __j1GUI_LABEL_H__
#define __j1GUI_LABEL_H__

#include "j1Gui_Elements.h"

class Gui_Label :public Gui_Elements
{
public:
	Gui_Label(Element_type type, iPoint position, char* Text, SDL_Color Color, TTF_Font* Font);

	~Gui_Label();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();

public:

	SDL_Rect* Rect = nullptr;

	char* text;
	SDL_Color color;
	TTF_Font* font;

};


#endif //__j1GUI_LABEL_H__
