#ifndef __j1GUI_IMAGE_H__
#define __j1GUI_IMAGE_H__

#include "j1Gui_Elements.h"

class Gui_Image :public Gui_Elements
{
public:
	Gui_Image(Element_type type, iPoint position, SDL_Rect rect, Gui_Elements* Parent, SDL_Texture* tex);

	~Gui_Image();

	bool Awake();

	bool Start();

	bool PreUpdate();

	bool PostUpdate();

	bool CleanUp();
};


#endif //__j1GUI_IMAGE_H__