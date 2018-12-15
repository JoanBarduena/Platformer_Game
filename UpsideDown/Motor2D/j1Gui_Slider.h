#ifndef __j1GUI_SLIDER_H__
#define __j1GUI_SLIDER_H__

#include "j1Gui_Elements.h"
#include "j1GUi_Button.h"

class Gui_Slider :public Gui_Elements
{
public:
	Gui_Slider(Element_type type, iPoint position, SDL_Rect rect, bool visible, SDL_Texture* tex, Gui_Elements* Parent);

	~Gui_Slider();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();

public:
	float SliderValue = 0.0f;

};

#endif //__j1GUI_SLIDER_H__