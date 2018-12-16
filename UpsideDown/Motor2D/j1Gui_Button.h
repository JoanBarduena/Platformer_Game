#ifndef __j1GUI_BUTTON_H__
#define __j1GUI_BUTTON_H__

#include "j1Gui_Elements.h"

class Gui_Button :public Gui_Elements
{
public:
	Gui_Button(Element_type type, iPoint position, SDL_Rect rect, SDL_Rect rect_hovering, SDL_Rect rect_clicking, bool visible, bool In_Game, SDL_Texture* tex, Function function, Gui_Elements* Parent);

	~Gui_Button();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();

public:

	Button_type Type = Button_type::NONE;

	bool hover_fx = true;

};


#endif //__j1GUI_BUTTON_H__