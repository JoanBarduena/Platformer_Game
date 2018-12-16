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
	int			 player_lifes; 
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
	int			 dead_limit_up, dead_limit_down;

	float dt_player;

	//HitBox
	
	Animation	 idle, running, jumping, idle_turned, run_turned, jump_turned, falling, falling_turned;
	Animation	 god_mode_anim, god_mode_turned, death; 

	Animation*	current_healing; 
	Animation	healing; 

	//Sounds FX 
	uint		 jump_fx, run_fx, invert_gravity_fx, death_fx; 

	float		 run_time = 0;

	player_config Player;

	//camera limits values
	int limit_left, limit_right, limit_up, limit_down;
	int player_limit_right, player_limit_left; 

	int lifes; 

	bool flip = false;				//if player is going left (-speed.x)
	bool touching_above = false;	//if player is touching from above
	bool touching_bottom = false;	//if player is touching from bottom
	bool touching_right = false;	//if player is touching from right
	bool touching_left = false;		//if player is touching from left
	bool is_jumping = false;		//if player is jumping (speed.y < 0)
	bool is_falling = false;		//if player is falling (speed.y > 0)		

	bool win1 = false;
	bool win2 = false;
	bool wintutorial = false;
	bool can_move = false;
	
	bool invert_gravity = false;
	bool can_invert = true;
	bool god_mode = false;

	bool camera_goes_left = false;
	bool camera_goes_right = false;

	bool initial_pos = true;
	bool player_starts = true; 

	bool is_healing = false; 

	int player_width, player_height;
	int jump_force;
	int collected_coins = 0;

	//Timer values 
	j1Timer game_time;
	int timer = 0;
	int saved_timer = 0;
	int paused_timer = 0;

	p2SString folder;
	p2List_item<ImageLayer*>* image = nullptr;
};

#endif // __j1PLAYER_H__