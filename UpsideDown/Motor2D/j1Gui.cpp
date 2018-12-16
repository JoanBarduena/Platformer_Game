#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Gui.h"
#include "j1Gui_Elements.h"
#include "j1Gui_Image.h"
#include "j1Gui_Label.h"
#include "j1GUi_Button.h"
#include "j1Gui_Slider.h"
#include "p2List.h"

j1Gui::j1Gui() : j1Module()
{
	name.create("gui");
}

// Destructor
j1Gui::~j1Gui()
{}

// Called before render is available
bool j1Gui::Awake(pugi::xml_node& conf)
{
	LOG("Loading GUI atlas");
	bool ret = true;

	atlas_file_name = conf.child("atlas").attribute("file").as_string("");

	return ret;
}

// Called before the first frame
bool j1Gui::Start()
{
	atlas = App->tex->Load(atlas_file_name.GetString());
	FX_Click = App->audio->LoadFx("audio/fx/ui/click1.wav");
	FX_Hover = App->audio->LoadFx("audio/fx/ui/rollover2.wav");

	bool ret = true;


	return ret;
}

// Update all guis
bool j1Gui::PreUpdate()
{
	BROFILER_CATEGORY("Gui PreUpdate", Profiler::Color::Blue);
	bool ret = true;

	for (p2List_item<Gui_Elements*>* iterator = List_elem.start; iterator != nullptr; iterator = iterator->next)
	{
		ret = iterator->data->PreUpdate();
		if (ret == false)
			break;
	}

	return ret;
}

// Called after all Updates
bool j1Gui::PostUpdate()
{
	BROFILER_CATEGORY("Gui PostUpdate", Profiler::Color::Red);

	bool ret = true;

	if (App->scene->pause)
		App->render->DrawQuad({ 0,0,(int)App->win->width, (int)App->win->height }, 0, 0, 0, 150, true, false);

	for (p2List_item<Gui_Elements*>* iterator = List_elem.start; iterator != nullptr; iterator = iterator->next)
	{
		ret = iterator->data->PostUpdate();
		if (ret == false)
			break;
	}

	
	return ret;
}

// Called before quitting
bool j1Gui::CleanUp()
{
	LOG("Freeing GUI");

	bool ret = true;

	for (p2List_item<Gui_Elements*>* iterator = List_elem.start; iterator != nullptr; iterator = iterator->next)
	{
		ret = iterator->data->CleanUp();
		if (ret == false)
			break;
	}

	return ret;
}

// const getter for atlas
SDL_Texture* j1Gui::GetAtlas() const
{
	return atlas;
}


Gui_Elements* j1Gui::Create_Button(Element_type element, iPoint position, SDL_Rect rect, SDL_Rect rect_hovering, SDL_Rect rect_clicking, bool visible, bool In_Game, SDL_Texture* tex, Function function, Gui_Elements* Parent, Button_type button)
{
	Gui_Elements* elem = nullptr;
	elem = new Gui_Button(element, position, rect, rect_hovering, rect_clicking, visible, In_Game, tex, function, Parent);

	if (elem != nullptr)
		List_elem.add(elem);

	return elem;
}

Gui_Elements* j1Gui::Create_Image(Element_type element, iPoint position, SDL_Rect rect, bool visible, bool In_Game, bool dragable, SDL_Texture* tex, Gui_Elements* Parent)
{
	Gui_Elements* elem = nullptr;
	elem = new Gui_Image(element, position, rect, visible, In_Game, dragable, tex, Parent);

	if (elem != nullptr)
		List_elem.add(elem);

	return elem;
}

Gui_Elements* j1Gui::Create_Label(Element_type element, iPoint position, bool visible, bool In_Game, p2SString Text, SDL_Color Color, _TTF_Font* Font, Gui_Elements* Parent)
{
	Gui_Elements* elem = nullptr;
	elem = new Gui_Label(element, position, visible, In_Game, Text, Color, Font, Parent);

	if (elem != nullptr )
		List_elem.add(elem);

	return elem;
}

Gui_Elements* j1Gui::Create_Slider(Element_type type, iPoint position, SDL_Rect rect, bool visible, SDL_Texture* tex, Gui_Elements* Parent)
{
	Gui_Elements* elem = nullptr;
	elem = new Gui_Slider(type, position, rect, visible, tex, Parent);

	if (elem != nullptr )
		List_elem.add(elem);

	return elem;
}

void j1Gui::Delete_UI_Elements()
{
	for (p2List_item<Gui_Elements*>* iterator = List_elem.start; iterator != nullptr; iterator = iterator->next) 
	{
			iterator->data->CleanUp();
			List_elem.del(iterator);
			RELEASE(iterator->data);
	
		LOG("deleting UI elements");
		
	}
}

void j1Gui::Delete_This_Element(Gui_Elements* element)
{
	for (p2List_item<Gui_Elements*>* iterator = List_elem.start; iterator; iterator = iterator->next)
	{
		if (iterator->data == element)
		{
			iterator->data->CleanUp();
			List_elem.del(iterator);
			RELEASE(iterator->data);
		}
	}
}

// class Gui ---------------------------------------------------
