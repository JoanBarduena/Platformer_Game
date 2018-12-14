#include "j1Gui_Elements.h"

Gui_Elements::Gui_Elements(Element_type type, iPoint position, SDL_Rect rect, bool Visible, Gui_Elements* Parent, SDL_Texture* tex) 
{
	pos.x = position.x;
	pos.y = position.y;
	parent = Parent;
	Rect = rect;
	

	if (parent != nullptr)
	{
		parent->childrens.add(this);
		visible = parent->visible;

		GlobalPos.x = parent->GlobalPos.x + pos.x;
		GlobalPos.y = parent->GlobalPos.y + pos.y;
	}
	else
	{
		visible = Visible;

		GlobalPos.x = pos.x;
		GlobalPos.y = pos.y;
	}
}

Gui_Elements::~Gui_Elements() {}
