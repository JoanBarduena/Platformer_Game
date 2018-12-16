#include "j1Gui_Button.h"
#include "j1Audio.h"


Gui_Button::Gui_Button(Element_type type, iPoint position, SDL_Rect rect, SDL_Rect rect_hovering, SDL_Rect rect_clicking, bool visible, bool In_Game, SDL_Texture* tex, Function function, Gui_Elements* Parent) : Gui_Elements(type, position, rect, visible, In_Game, Parent, tex)
{
	
	texture = tex;
	funct = function;

	hovering_rect = rect_hovering;
	clicking_rect = rect_clicking;

	hovering = false;
	clicking_left = false;
	do_action = false;

}

Gui_Button::~Gui_Button() {}

bool Gui_Button::Awake()
{
	return true;
}

bool Gui_Button::Start()
{

	

	return true;
}

bool Gui_Button::PreUpdate()
{

	if (parent != nullptr)
	{
		GlobalPos.x = parent->GlobalPos.x + pos.x;
		GlobalPos.y = parent->GlobalPos.y + pos.y;
		
		visible = parent->visible;

	}

	debug_rect.x = GlobalPos.x;
	debug_rect.y = GlobalPos.y;

	if (visible)
	{
		if (!in_game)
		{
			if (App->scene->Mouse_Pos.x > GlobalPos.x && App->scene->Mouse_Pos.x < GlobalPos.x + Rect.w && App->scene->Mouse_Pos.y > GlobalPos.y && App->scene->Mouse_Pos.y < GlobalPos.y + Rect.h)
			{
				hovering = true;
				if (hover_fx)
				{
					App->audio->PlayFx(App->gui->FX_Hover);
					hover_fx = false;
				}
					
			}
			else
			{
				hovering = false;
				hover_fx = true;
			}

			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && hovering)
			{
				clicking_left = true;
			}
			if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && hovering && clicking_left)
			{
				clicking_left = false;
				do_action = true;
				App->audio->PlayFx(App->gui->FX_Click);

			}
			else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && !hovering)
			{
				clicking_left = false;
			}
		}
		else
		{
			if (App->scene->pause == false)
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
				if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && hovering && clicking_left)
				{
					clicking_left = false;
					do_action = true;

				}
				else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && !hovering)
				{
					clicking_left = false;
				}
			}
		}
		

		
	}


	

	return true;
}

bool Gui_Button::PostUpdate()
{
	if (visible && !in_game)
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
		if (App->scene->debug_UI)
		{
			Show_Debug_Rect();
		}
	}
	
	

	return true;
}

bool Gui_Button::CleanUp()
{

	return true;
}