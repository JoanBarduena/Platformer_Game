#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "j1Map.h"
#include "j1Entity.h"

struct SDL_Texture;

struct player_config
{
	iPoint		 position;
	iPoint		 camera_position;

	int			 limit_up, limit_down, limit_left, limit_right;
	int			 maxSpeed_y, player_speed, jump_force;
	int			 player_width, player_height;
	int			 player_dead_limit_down, player_dead_limit_up;
	int			 player_limit_left, player_limit_right, player_limit_up, player_limit_down;
	int			 godmode_hitbox; 
};

class j1Player : public j1Entity
{
public:
	j1Player(int x, int y, EntityType type);
	~j1Player();

	
	bool Start();
	bool Update(float dt, bool do_logic);
	bool PostUpdate();
	bool CleanUp();

	void LoadValues();
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

	fPoint		 position;
	fPoint		 speed;
	int			 player_speed;
	int			 maxSpeed_y;
	int			dead_limit_up, dead_limit_down;

	float dt_player;

	//HitBox
	
	Animation	 idle, running, jumping, idle_turned, run_turned, jump_turned, falling, falling_turned;
	Animation	 god_mode_anim, god_mode_turned; 

	//Sounds FX 
	uint		 jump_fx, run_fx, invert_gravity_fx, death_fx; 

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
	bool can_move;
	
	bool invert_gravity;
	bool can_invert = true;
	bool god_mode = false;

	bool camera_goes_left;
	bool camera_goes_right;

	bool initial_pos = true;

	int player_width, player_height;
	int jump_force;
	int counter = 0;

	p2SString folder;
	p2List_item<ImageLayer*>* image = nullptr;
};

#endif // __j1PLAYER_H__