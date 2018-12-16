#include "j1Gui_Elements.h"

Gui_Elements::Gui_Elements(Element_type Type, iPoint position, SDL_Rect rect, bool Visible, bool In_Game, Gui_Elements* Parent, SDL_Texture* tex) 
{
	pos.x = position.x;
	pos.y = position.y;
	parent = Parent;
	Rect = rect;
	type = Type;
	

	if (parent != nullptr)
	{
		parent->childrens.add(this);
		visible = parent->visible;
		in_game = parent->in_game;

		GlobalPos.x = parent->GlobalPos.x + pos.x;
		GlobalPos.y = parent->GlobalPos.y + pos.y;

	}
	else
	{
		visible = Visible;
		in_game = In_Game;

		GlobalPos.x = pos.x;
		GlobalPos.y = pos.y;
	}

	debug_rect.x = GlobalPos.x;
	debug_rect.y = GlobalPos.y;
	debug_rect.w = rect.w;
	debug_rect.h = rect.h;
}

Gui_Elements::~Gui_Elements() {}

void Gui_Elements::Show_Debug_Rect()
{
	App->render->DrawQuad(debug_rect, 255, 0, 0, 255, false, false);
}

void Gui_Elements::Draw_InGame_Element()
{
	if (visible)
	{
		if (type == Element_type::BUTTON)
		{

			if (clicking_left == true)
			{
				App->render->Blit(texture, GlobalPos.x, GlobalPos.y, &clicking_rect, SDL_FLIP_NONE, 0);
			}
			else if (hovering == true)
			{
				App->render->Blit(texture, GlobalPos.x, GlobalPos.y, &hovering_rect, SDL_FLIP_NONE, 0);
			}
			else
			{
				App->render->Blit(texture, GlobalPos.x, GlobalPos.y, &Rect, SDL_FLIP_NONE, 0);
			}

		}
		else
		{
			App->render->Blit(texture, GlobalPos.x, GlobalPos.y, &Rect, SDL_FLIP_NONE, 0);
		}
		if (App->scene->debug_UI)
		{
			Show_Debug_Rect();
		}
	}
	
}
