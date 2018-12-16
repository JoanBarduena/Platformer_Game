#include "j1Gui_Elements.h"
#include "j1Textures.h"

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

void Gui_Elements::ValuetoString(int value, p2SString text)
{
	std::string s = std::to_string(value);

	std::string t("%%");
	
	if(parent == App->scene->Settings_Menu)
		s += t;

	text = s.c_str();

	App->tex->UnLoad(texture);

	texture = App->font->Print(text.GetString(), color, font);

	int width = 0, height = 0;
	App->font->CalcSize(text.GetString(), width, height, font);
	Rect.w = width;
	Rect.h = height;
}

void Gui_Elements::SetHour(int milisec)
{
	int sec = milisec / 1000.0f; //from milisecond to sec
	int min = sec / 60.0f; //from seconds to minutes 
	int hour = min / 60.0f; //from minutes to hours

	p2SString time;
 
	int sec_print = sec; 

	if (min > 0)
		sec_print -= min * 60;

	if (hour >= 10 && min >= 10 && sec_print >= 10) {
		time.create("%i : %i : %i", hour, min, sec_print);
	}
	else if (hour < 10 && min >= 10 && sec_print >= 10) {
		time.create("0%i : %i : %i", hour, min, sec_print);
	}
	else if (hour >= 10 && min < 10 && sec_print >= 10) {
		time.create("%i : 0%i : %i", hour, min, sec_print);
	}
	else if (hour >= 10 && min >= 10 && sec_print < 10) {
		time.create("%i : %i : 0%i", hour, min, sec_print);
	}
	else if (hour >= 10 && min < 10 && sec_print < 10) {
		time.create("%i : 0%i : 0%i", hour, min, sec_print);
	}
	else if (hour < 10 && min >= 10 && sec_print < 10) {
		time.create("0%i : %i : 0%i", hour, min, sec_print);
	}
	else if (hour < 10 && min < 10 && sec_print >= 10) {
		time.create("0%i : 0%i : %i", hour, min, sec_print);
	}
	else {
		time.create("0%i : 0%i : 0%i", hour, min, sec_print);
	}

	text.create(time.GetString());

	App->tex->UnLoad(texture);

	texture = App->font->Print(text.GetString(), color, font);

	int width = 0, height = 0;
	App->font->CalcSize(this->text.GetString(), width, height, font);
	Rect.w = width;
	Rect.h = height;
}
