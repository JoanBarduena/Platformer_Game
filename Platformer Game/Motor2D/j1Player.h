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
	void CameraOnPlayer();
	void SetIdleAnimation();

	//Save and Load
	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

public:
	SDL_Texture * graphics = nullptr;

	iPoint position;
	iPoint speed;
	int player_speed;
	int maxSpeed_y;

	//Animations--------------------------
	Animation* current_animation = nullptr;
	Animation idle;
	Animation idle_turned; 
	Animation running;
	Animation run_turned; 
	Animation jumping;
	Animation jump_turned; 
	Animation falling;
	Animation falling_turned; 

	//HitBox
	Collider* playerHitbox; 

	//Sounds FX ------
	uint jump; 
	uint run; 
	float run_time = 0;
	//---------------

	//camera limits values
	int limit_left, limit_right, limit_up, limit_down;

	bool flip;
	bool touching_above;//if player is touching from above
	bool touching_bottom;
	bool touching_right; //if player is touching from right
	bool touching_left; //if player is touching from left
	bool is_jumping; //if player is jumping (speed.y < 0)
	bool is_falling; //if player is falling (speed.y > 0)
	
	bool invert_gravity;

	int player_width, player_height;
	int jump_force;
	int cooldown;

	p2SString folder;

	
};

#endif // __j1PLAYER_H__