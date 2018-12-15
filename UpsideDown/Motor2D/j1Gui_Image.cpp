#include "j1Gui_Image.h"

Gui_Image::Gui_Image(Element_type type, iPoint position, SDL_Rect rect, bool Visible, bool In_Game, bool Dragable, SDL_Texture* tex, Gui_Elements* Parent) : Gui_Elements(type, position, rect, Visible, In_Game, Parent, tex)
{
	
	texture = tex;
	dragable = Dragable;

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

	return true;
}

bool Gui_Image::PostUpdate()
{
	if(visible && !in_game)
		App->render->Blit(texture, GlobalPos.x, GlobalPos.y, &Rect, SDL_FLIP_NONE, 0);

	return true;
}

bool Gui_Image::CleanUp()
{

	return true;
}