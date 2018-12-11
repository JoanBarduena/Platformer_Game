#include "j1Gui_Button.h"


Gui_Button::Gui_Button(Element_type type, iPoint position, SDL_Rect rect, SDL_Texture* tex, Function function) : Gui_Elements(type, position, tex, function)
{
	pos.x = position.x;
	pos.y = position.y;
	texture = tex;
	funct = function;

	Rect = rect;

	hovering_rect = { 0,113,229,69 };
	clicking_rect = { 411,169,229,69 };

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
	App->input->GetMousePosition(Mouse_Pos.x, Mouse_Pos.y);

	if (Mouse_Pos.x > pos.x && Mouse_Pos.x < pos.x + Rect.w && Mouse_Pos.y > pos.y && Mouse_Pos.y < pos.y + Rect.h)
	{
		hovering = true;
	}
	else
	{
		hovering = false;
	}

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && hovering && !clicking_right)
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

	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN && hovering && !clicking_left)
	{
		clicking_right = true;
	}
	if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP && hovering)
	{
		clicking_right = false;
		do_action = true;
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_UP && !hovering)
	{
		clicking_right = false;
	}

	return true;
}

bool Gui_Button::PostUpdate()
{

	if (clicking_right == true || clicking_left == true)
	{
		App->render->Blit(texture, pos.x, pos.y, &clicking_rect, SDL_FLIP_NONE);
	}
	else if (hovering == true)
	{
		App->render->Blit(texture, pos.x, pos.y, &hovering_rect, SDL_FLIP_NONE);
	}
	else
	{
		App->render->Blit(texture, pos.x, pos.y, &Rect, SDL_FLIP_NONE);
	}

	return true;
}

bool Gui_Button::CleanUp()
{

	return true;
}