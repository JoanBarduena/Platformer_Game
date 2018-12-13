#include "j1Gui_Label.h"
#include "j1Fonts.h"

Gui_Label::Gui_Label(Element_type type, iPoint position, SDL_Rect rect, const char* Text, SDL_Color Color, TTF_Font* Font, Gui_Elements* Parent) : Gui_Elements(type, position, rect, Parent)
{
	pos.x = position.x;
	pos.y = position.y;
	text = Text;
	color = Color;
	font = Font;
	parent = Parent;

	Rect = rect;

	texture = App->font->Print(text, color, font);


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
	}

	return true;
}

bool Gui_Label::PostUpdate() {


	App->render->Blit(texture, GlobalPos.x, GlobalPos.y, &Rect, SDL_FLIP_NONE, 0);

	return true;
}

bool Gui_Label::CleanUp() {

	return true;
}