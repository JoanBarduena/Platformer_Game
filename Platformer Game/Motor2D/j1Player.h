#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;

class j1Player : public j1Module
{
public:
	j1Player();
	~j1Player();

	bool Awake(pugi::xml_node& config);
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:
	SDL_Texture * graphics = nullptr;

	iPoint position;
	float speed;

	//Animations--------------------------
	Animation* current_animation = nullptr;
	Animation idle;
	Animation running;

	bool flip;

};

#endif // __j1PLAYER_H__