#include "j1Gui_Image.h"

Gui_Image::Gui_Image(Element_type type, iPoint position, SDL_Rect rect, bool Visible, bool Dragable, SDL_Texture* tex, Gui_Elements* Parent) : Gui_Elements(type, position, rect, Visible, Parent, tex)
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
				pos.x = App->scene->Mouse_Pos.x - (App->scene->Prev_Mouse_Pos.x - parent->GlobalPos.x);
				pos.y = App->scene->Mouse_Pos.y - (App->scene->Prev_Mouse_Pos.y - parent->GlobalPos.y);

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
	if(visible)
		App->render->Blit(texture, GlobalPos.x, GlobalPos.y, &Rect, SDL_FLIP_NONE, 0);

	return true;
}

bool Gui_Image::CleanUp()
{

	return true;
}