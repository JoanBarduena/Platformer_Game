#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"

struct SDL_Texture;

struct level
{
	level(int num, char* mapPath, char* music)
	{
		this->mapPath.create(mapPath);
		level_music.create(music); 
		lvl = num;
	}

	p2SString mapPath;
	p2SString level_music; 
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

public:

	p2List<level*> levels_list;
	p2List_item<level*>* actual_level;
	SDL_Texture * debug_tex;

private:
	
	bool player_added = false;
	bool not_started = false; 
};

#endif // __j1SCENE_H__