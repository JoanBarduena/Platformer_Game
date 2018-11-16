#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;

struct player_config
{
	iPoint		 position;
	iPoint		 camera_position;

	int			 limit_up, limit_down, limit_left, limit_right;
	int			 maxSpeed_y, player_speed, jump_force;
	int			 player_width, player_height;
	int			 player_limit_left, player_limit_right, player_limit_up, player_limit_down;
	int			 cooldown;
	int			 godmode_hitbox; 
};

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
	void GameMode(); 
	void GodMode(); 

	//Save and Load
	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

public:

	SDL_Texture* graphics = nullptr;

	iPoint		 position;
	iPoint		 speed;
	int			 player_speed;
	int			 maxSpeed_y;

	//HitBox
	Collider*	 playerHitbox = nullptr; 

	Animation*   current_animation = nullptr;
	Animation	 idle, running, jumping, idle_turned, run_turned, jump_turned, falling, falling_turned;
	Animation	 god_mode_anim, god_mode_turned; 

	//Sounds FX 
	uint		 jump; 
	uint		 run; 
	float		 run_time = 0;

	player_config Player;

	//camera limits values
	int limit_left, limit_right, limit_up, limit_down;
	int player_limit_right, player_limit_left; 

	bool flip;				//if player is going left (-speed.x)
	bool touching_above;	//if player is touching from above
	bool touching_bottom;	//if player is touching from bottom
	bool touching_right;	//if player is touching from right
	bool touching_left;		//if player is touching from left
	bool is_jumping;		//if player is jumping (speed.y < 0)
	bool is_falling;		//if player is falling (speed.y > 0)		

	bool win1;
	bool win2;
	
	bool invert_gravity;

	bool god_mode = false;

	bool camera_goes_left;
	bool camera_goes_right;

	int player_width, player_height;
	int jump_force;
	int cooldown;
	int level_change = 0;
	int counter = 0; 

	p2SString folder;
};

#endif // __j1PLAYER_H__