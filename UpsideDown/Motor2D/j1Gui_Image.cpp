#include "j1Gui_Image.h"

Gui_Image::Gui_Image(Element_type type, iPoint position, SDL_Rect rect, bool Visible, bool In_Game, bool Dragable, SDL_Texture* tex, Gui_Elements* Parent) : Gui_Elements(type, position, rect, Visible, In_Game, Parent, tex)
{
	
	texture = tex;
	dragable = Dragable;

	if (parent != nullptr)
	{
		if (parent->type == Element_type::SLIDER)
		{
			hovering_rect = {723, rect.y, rect.w, rect.h};
			clicking_rect = {677, rect.y-2, rect.w, rect.h};
			parent->Value_percentage = (((GlobalPos.x - parent->GlobalPos.x) * 100) / parent->Rect.w - rect.w) + rect.w;
		}
	}

}

Gui_Image::~Gui_Image() {}

bool Gui_Image::Awake()
{

	return true;
}

bool Gui_Image::Start()
{

	return true;
}

bool Gui_Image::PreUpdate()
{
	if (visible)
	{
		if (App->scene->Mouse_Pos.x > GlobalPos.x && App->scene->Mouse_Pos.x < GlobalPos.x + Rect.w && App->scene->Mouse_Pos.y > GlobalPos.y && App->scene->Mouse_Pos.y < GlobalPos.y + Rect.h)
		{
			hovering = true;
		}
		else
		{
			hovering = false;
		}

		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && hovering)
		{
			clicking_left = true;
		}
		if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && clicking_left)
		{
			clicking_left = false;
		}

	}
	
	if (clicking_left)
	{

		if (dragable)
		{
			if (parent != nullptr)
			{
				if (parent->type == Element_type::SLIDER)
				{
					pos.x = App->scene->Mouse_Pos.x - (Rect.w / 2) - parent->GlobalPos.x;
				}
				else
				{
					pos.x = App->scene->Mouse_Pos.x - (Rect.w/2);
					pos.y = App->scene->Mouse_Pos.y - (Rect.h/2);
				}
				
			}
			else
			{
				pos.x = App->scene->Mouse_Pos.x - (App->scene->Prev_Mouse_Pos.x - GlobalPos.x);
				pos.y = App->scene->Mouse_Pos.y - (App->scene->Prev_Mouse_Pos.y - GlobalPos.y);
			}

		}


	}

	if (parent != nullptr)
	{
		GlobalPos.x = parent->GlobalPos.x + pos.x;
		GlobalPos.y = parent->GlobalPos.y + pos.y;

		if (parent->type == Element_type::SLIDER)
		{
			if (GlobalPos.x < parent->GlobalPos.x)
			{
				GlobalPos.x = parent->GlobalPos.x;
			}
			else if (GlobalPos.x > parent->GlobalPos.x + parent->Rect.w)
			{
				GlobalPos.x = parent->GlobalPos.x + parent->Rect.w;
			}
				
			parent->Value_percentage = (((GlobalPos.x - parent->GlobalPos.x) * 100)/ parent->Rect.w - Rect.w) + Rect.w;
		}

		visible = parent->visible;
	}
	else
	{
		GlobalPos.x = pos.x;
		GlobalPos.y = pos.y;
	}

	debug_rect.x = GlobalPos.x;
	debug_rect.y = GlobalPos.y;

	return true;
}

bool Gui_Image::PostUpdate()
{
	if (visible && !in_game)
	{
		if (parent != nullptr)
		{
			if (parent->type == Element_type::SLIDER)
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
		
	return true;
}

bool Gui_Image::CleanUp()
{

	return true;
}