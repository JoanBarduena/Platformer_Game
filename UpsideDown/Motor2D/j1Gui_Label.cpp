#include "j1Gui_Label.h"
#include "j1Fonts.h"

Gui_Label::Gui_Label(Element_type type, iPoint position, SDL_Rect rect, bool visible, bool In_Game, const char* Text, SDL_Color Color, TTF_Font* Font, Gui_Elements* Parent) : Gui_Elements(type, position, rect, visible, In_Game, Parent)
{
	
	text = Text;
	color = Color;
	font = Font;

	texture = App->font->Print(text, color, font);

}

Gui_Label::~Gui_Label() {}

bool Gui_Label::Awake()
{
	return true;
}

bool Gui_Label::Start()
{
	return true;
}

bool Gui_Label::PreUpdate()
{
	if (parent != nullptr)
	{
		GlobalPos.x = parent->GlobalPos.x + pos.x;
		GlobalPos.y = parent->GlobalPos.y + pos.y;
		visible = parent->visible;
		
		if (parent->type == Element_type::BUTTON)
		{
			if (parent->clicking_left)
				GlobalPos.y += 4;
		}

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

bool Gui_Label::PostUpdate() {

	if (visible && !in_game)
	{
		App->render->Blit(texture, GlobalPos.x, GlobalPos.y, &Rect, SDL_FLIP_NONE, 0);
		if (App->scene->debug_UI)
		{
			Show_Debug_Rect();
		}
	}
		
		

	return true;
}

bool Gui_Label::CleanUp() {

	return true;
}