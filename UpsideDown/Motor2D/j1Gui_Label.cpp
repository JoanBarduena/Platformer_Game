#include "j1Gui_Label.h"
#include "j1Fonts.h"

Gui_Label::Gui_Label(Element_type type, iPoint position, char* Text, SDL_Color Color, TTF_Font* Font) : Gui_Elements(type, position)
{
	pos.x = position.x;
	pos.y = position.y;
	text = Text;
	color = Color;
	font = Font;

	texture = App->font->Print(text, color, font);

}

Gui_Label::~Gui_Label() {}

bool Gui_Label::Awake() {

	return true;
}

bool Gui_Label::Start()
{


	return true;
}

bool Gui_Label::PreUpdate()
{


	return true;
}

bool Gui_Label::PostUpdate() {


	App->render->Blit(texture, pos.x, pos.y, Rect, SDL_FLIP_NONE);

	return true;
}

bool Gui_Label::CleanUp() {

	return true;
}