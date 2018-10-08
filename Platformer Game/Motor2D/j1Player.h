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

	void Flip();
	void OnCollision(Collider* c1, Collider* c2);
	void Check_Collision();

public:
	SDL_Texture * graphics = nullptr;

	iPoint position;
	iPoint speed;

	int maxSpeed_y;

	//Animations--------------------------
	Animation* current_animation = nullptr;
	Animation idle;
	Animation running;
	Animation jumping;
	Animation falling;

	//HitBox
	Collider* playerHitbox; 

	//Sounds FX
	uint jump; 
	uint run; 

	bool flip;

	bool touching_y; //if player is touching from above
	bool touching_right; //if player is touching from right
	bool touching_left; //if player is touching from left

	bool is_jumping; //if player is jumping (speed.y < 0)
	bool is_falling; //if player is falling (speed.y > 0)
};

#endif // __j1PLAYER_H__