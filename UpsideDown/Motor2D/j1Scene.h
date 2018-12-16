#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "Animation.h"
#include "j1Gui.h"

struct SDL_Texture;

struct level
{
	level(int num, char* mapPath)
	{
		this->mapPath.create(mapPath);
		lvl = num;
	}

	p2SString mapPath;
	int lvl;
};

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//Load Level
	void Level_Load(uint number = 0);
	void RespawnEnemies();
	void RespawnCoinsHearts(); 
	void Create_UI_Elements();

public:

	p2List<level*> levels_list;
	p2List_item<level*>* level_to_load;
	uint actual_level;
	SDL_Texture * debug_tex;

	iPoint Mouse_Pos;
	iPoint Prev_Mouse_Pos;

	bool start_pos = true;
	bool pause = false;
	bool debug_UI = false;

	bool loading_tutorial = false;
	bool loading_lvl1 = false;
	bool loading_lvl2 = false;
	bool loading_menu = false;
	bool saved_game = false;
	bool loading_saved_game = false;

	bool from_menu = false; 

	Gui_Elements* Settings_Menu = nullptr;

private:

	float dt_scene;

	bool adding_entities = false;
	bool menu_active = true; 
	bool start_elems = true;
	
	Animation player_running;
	Animation *current;

	Animation* current_keyA; 
	Animation keyA, keyA_pressed; 

	Animation* current_keyD;
	Animation keyD, keyD_pressed;

	Animation* current_keyP;
	Animation keyP, keyP_pressed;

	Animation* current_keyL;
	Animation keyL, keyL_pressed;

	Animation* current_keySpace;
	Animation keySpace, keySpace_pressed;

	SDL_Texture *keyboard; 
	SDL_Texture *graphics;
	SDL_Texture *title;

	Gui_Elements* Skip = nullptr;
	Gui_Elements* Skip_Text = nullptr;

	Gui_Elements* Menu = nullptr;

	Gui_Elements* Coins = nullptr; 

	Gui_Elements* Timer = nullptr; 

	Gui_Elements* Slider_FX = nullptr;
	Gui_Elements* Slider_FX_Value = nullptr;
	Gui_Elements* Button_FX = nullptr;
	int FX_Slider_pos = 95;

	Gui_Elements* Slider_Music = nullptr;
	Gui_Elements* Slider_Music_Value = nullptr;
	Gui_Elements* Button_Music = nullptr;
	int Music_Slider_pos = 35;

	Gui_Elements* Main_Menu = nullptr;

	Gui_Elements* Credits_Menu = nullptr;
	
};

#endif // __j1SCENE_H__