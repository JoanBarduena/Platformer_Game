#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "Animation.h"

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
	void RespawnEntities(); 

public:

	p2List<level*> levels_list;
	p2List_item<level*>* level_to_load;
	uint actual_level;
	SDL_Texture * debug_tex;

	bool start_pos = true;

private:
	
	float dt_scene;

	bool adding_entities = false;
	bool menu_active = true; 
	bool start_elems = true; 
	Animation player_running;
	Animation *current;

	Animation* current_keyA; 
	Animation keyA, keyA_pressed; 

	SDL_Texture *keyboard; 
	SDL_Texture *graphics;
	

	SDL_Rect rect = { 48, 49, 16, 15 };
};

#endif // __j1SCENE_H__