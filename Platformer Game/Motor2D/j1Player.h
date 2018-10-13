#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"

struct SDL_Texture;

struct player_config
{
	iPoint position;
	int limit_up, limit_down, limit_left, limit_right;
	int maxSpeed_y, player_speed, jump_force;
	int player_width, player_height;
	iPoint camera_position;
	int player_limit_left, player_limit_right;

	//Idle animation
	int idle_width, idle_height;
	iPoint idle1, idle2, idle3, idle4;

	//Jump animation
	int jump_width, jump_height;
	float jump_speed;
	iPoint jump1, jump2, jump3, jump4, jump5, jump6, jump7, jump8, jump9, jump10;

	//Run animation
	int run_width, run_height;
	iPoint run1, run2, run3, run4, run5, run6;

	//Idle animation turned 
	int idle_turned_width, idle_turned_height;
	iPoint idle_turned1, idle_turned2, idle_turned3, idle_turned4;

	//Run animation turned 
	int run_turned_width, run_turned_height;
	iPoint run_turned1, run_turned2, run_turned3, run_turned4, run_turned5, run_turned6;

	//Jump animation turned
	int jump_turned_width, jump_turned_height;
	float jump_turned_speed;
	iPoint jump_turned1, jump_turned2, jump_turned3, jump_turned4, jump_turned5, jump_turned6, jump_turned7, jump_turned8, jump_turned9, jump_turned10;

	//Falling animation
	int falling_width, falling_height;
	iPoint falling1, falling2;

	//Falling animation turned 
	int falling_turned_width, falling_turned_height;
	iPoint falling_turned1, falling_turned2;

	//Gravity cooldown value 
	int cooldown; 
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

	//Save and Load
	bool Save(pugi::xml_node&) const;
	bool Load(pugi::xml_node&);

	void LoadAnimations(); 

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

	player_config Player;


	//camera limits values
	int limit_left, limit_right, limit_up, limit_down;
	int player_limit_right, player_limit_left; 

	bool flip;
	bool touching_above;//if player is touching from above
	bool touching_bottom;
	bool touching_right; //if player is touching from right
	bool touching_left; //if player is touching from left
	bool is_jumping; //if player is jumping (speed.y < 0)
	bool is_falling; //if player is falling (speed.y > 0)

	bool win1;
	bool win2;
	
	bool invert_gravity;

	bool god_mode = false;

	bool camera_goes_left;
	bool camera_goes_right;

	int player_width, player_height;
	int jump_force;
	int cooldown;
	int counter = 0; 

	p2SString folder;

	
};

#endif // __j1PLAYER_H__