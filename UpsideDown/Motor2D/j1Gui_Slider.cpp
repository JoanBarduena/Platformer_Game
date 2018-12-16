#include "j1Gui_Slider.h"

Gui_Slider::Gui_Slider(Element_type type, iPoint position, SDL_Rect rect, bool visible, SDL_Texture* tex, Gui_Elements* Parent) : Gui_Elements(type, position, rect, visible, false, Parent, tex)
{
	texture = tex;
	in_game = false;
}

Gui_Slider::~Gui_Slider()
{

}

bool Gui_Slider::Awake()
{
	return true;
}

bool Gui_Slider::Start()
{
	return true;
}

bool Gui_Slider::PreUpdate()
{
	if (parent != nullptr)
	{
		GlobalPos.x = parent->GlobalPos.x + pos.x;
		GlobalPos.y = parent->GlobalPos.y + pos.y;

		visible = parent->visible;

	}
	
	

	return true;
}

bool Gui_Slider::PostUpdate()
{

	SliderValue = (float)Value_percentage / 100;

	if(visible)
		App->render->Blit(texture, GlobalPos.x, GlobalPos.y, &Rect, SDL_FLIP_NONE, 0);

	return true;
}

bool Gui_Slider::CleanUp() 
{
	return true;
}
