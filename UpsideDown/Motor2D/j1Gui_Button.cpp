#include "j1Gui_Button.h"


Gui_Button::Gui_Button(Element_type type, iPoint position, SDL_Rect rect, SDL_Texture* tex, Function function, Gui_Elements* Parent) : Gui_Elements(type, position, rect, Parent, tex)
{
	pos.x = position.x;
	pos.y = position.y;
	texture = tex;
	funct = function;
	parent = Parent;

	Rect = rect;

	hovering_rect = { 650, 260, 190, 49 };
	clicking_rect = { 860, 264, 190, 45 };


	if (parent != nullptr)
	{
		parent->childrens.add(this);

		GlobalPos.x = parent->GlobalPos.x + pos.x;
		GlobalPos.y = parent->GlobalPos.y + pos.y;
	}
	else
	{
		GlobalPos.x = pos.x;
		GlobalPos.y = pos.y;

	}

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

	}


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
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && hovering)
	{
		clicking_left = false;
		do_action = true;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP && !hovering)
	{
		clicking_left = false;
	}

	return true;
}

bool Gui_Button::PostUpdate()
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

	return true;
}

bool Gui_Button::CleanUp()
{

	return true;
}