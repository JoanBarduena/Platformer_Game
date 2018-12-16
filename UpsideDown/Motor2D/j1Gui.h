#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "p2Defs.h"
#include "p2Log.h"
#include "j1Module.h"
#include "j1App.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Fonts.h"

#include "SDL/include/SDL.h"

#define CURSOR_WIDTH 2

// TODO 1: Create your structure of classes
enum class Element_type
{
	BUTTON,
	LABEL,
	IMAGE,
	SLIDER,
	NONE
};

enum class Button_type
{
	STANDARD,
	SLIDER,
	NONE
};

enum class Function
{
	PLAY,
	EXIT,
	GITHUB,
	GITHUB_JOSEP,
	GITHUB_JOAN,
	SKIP,
	RESUME,
	LOAD,
	SAVE,
	SETTINGS,
	MENU,
	APPLY,
	CREDITS,
	NONE
};

class Gui_Elements;
// ---------------------------------------------------
class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	SDL_Texture* GetAtlas() const;

	Gui_Elements* Create_Button(Element_type element, iPoint position, SDL_Rect rect, SDL_Rect rect_hovering, SDL_Rect rect_clicking, bool visible, bool In_Game, SDL_Texture* tex = nullptr, Function function = Function::NONE, Gui_Elements* Parent = nullptr, Button_type button = Button_type::NONE);
	Gui_Elements* Create_Image(Element_type element, iPoint position, SDL_Rect rect, bool visible, bool In_Game, bool dragable, SDL_Texture* tex = nullptr, Gui_Elements* Parent = nullptr);
	Gui_Elements* Create_Label(Element_type element, iPoint position, bool visible, bool In_Game, p2SString Text, SDL_Color Color, _TTF_Font* Font, Gui_Elements* Parent = nullptr);
	Gui_Elements* Create_Slider(Element_type type, iPoint position, SDL_Rect rect, bool visible, SDL_Texture* tex = nullptr, Gui_Elements* Parent = nullptr);

	void Delete_UI_Elements();
	void Delete_This_Element(Gui_Elements* element);

private:

	SDL_Texture * atlas;
	p2SString atlas_file_name;

public:
	p2List<Gui_Elements*> List_elem;
	uint FX_Click, FX_Hover;


};
#endif // __j1GUI_H__