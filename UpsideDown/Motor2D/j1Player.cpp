#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Collision.h"
#include "Animation.h"
#include "j1Fadetoblack.h"

#include<stdio.h>

j1Player::j1Player(int x, int y, EntityType Type) : j1Entity(x,y,Type)
{
	//Loading Animations
	idle.LoadAnimations("player", "idle");
	running.LoadAnimations("player", "running");
	jumping.LoadAnimations("player", "jumping");
	idle_turned.LoadAnimations("player", "idle_turned");
	run_turned.LoadAnimations("player", "run_turned");
	jump_turned.LoadAnimations("player", "jump_turned");
	falling.LoadAnimations("player", "falling");
	falling_turned.LoadAnimations("player", "falling_turned");

	god_mode_anim.LoadAnimations("player", "god_mode_anim");
	god_mode_turned.LoadAnimations("player", "god_mode_turned");

	death.LoadAnimations("player", "death"); 

	healing.LoadAnimations("player", "healing"); 

	type = Type;
}

j1Player::~j1Player()
{}

void j1Player::LoadValues()
{
	pugi::xml_document config_file;
	config_file.load_file("config.xml");
	pugi::xml_node config;
	config = config_file.child("config");
	pugi::xml_node player;
	player = config.child("player");

	//PLayer initial position
	Player.position.x = player.child("position").attribute("x").as_int();
	Player.position.y = player.child("position").attribute("y").as_int();

	//camera limits
	limit_up = player.child("limits").attribute("up").as_int();
	Player.limit_down = player.child("limits").attribute("down").as_int();
	Player.limit_left = player.child("limits").attribute("left").as_int();
	Player.limit_right = player.child("limits").attribute("right").as_int();

	//Player physic values
	Player.player_speed = player.child("speed_x").attribute("value").as_int();
	Player.maxSpeed_y = player.child("maxSpeed_y").attribute("value").as_int();
	Player.jump_force = player.child("jump_force").attribute("value").as_int();

	//Player dimensions
	Player.player_width = player.child("player_size").attribute("width").as_int();
	Player.player_height = player.child("player_size").attribute("height").as_int();

	//Camera initial position
	Player.camera_position.x = player.child("camera").attribute("x").as_int();
	Player.camera_position.y = player.child("camera").attribute("y").as_int();

	//Player limits 
	Player.player_limit_up = player.child("player_limit").attribute("up").as_int();
	Player.player_limit_down = player.child("player_limit").attribute("down").as_int();
	Player.player_limit_left = player.child("player_limit").attribute("left").as_int();
	Player.player_limit_right = player.child("player_limit").attribute("right").as_int();

	//Player deadzone limit 
	Player.player_dead_limit_down = player.child("player_dead_limit").attribute("down").as_int();
	Player.player_dead_limit_up = player.child("player_dead_limit").attribute("up").as_int();

	//GodMode Hitbox value
	Player.godmode_hitbox = player.child("godmode_hitbox").attribute("value").as_int();

	//Initial player lifes
	if (player_starts)
	{
		Player.player_lifes = player.child("lifes").attribute("value").as_int();
	}
		
}

bool j1Player::Start()
{
	LoadValues();

	if (player_starts)
	{
		limit_up = Player.limit_up;
		limit_down = Player.limit_down;
		limit_right = Player.limit_right;
		limit_left = Player.limit_left;

		player_speed = Player.player_speed;
		maxSpeed_y = Player.maxSpeed_y;
		jump_force = Player.jump_force;

		player_width = Player.player_width;
		player_height = Player.player_height;

		dead_limit_down = Player.player_dead_limit_down;
		dead_limit_up = Player.player_dead_limit_up;

		lifes = Player.player_lifes; 

		//Loading Sounds FX
		jump_fx = App->audio->LoadFx("audio/fx/Jump.wav");
		run_fx = App->audio->LoadFx("audio/fx/Run.wav");
		invert_gravity_fx = App->audio->LoadFx("audio/fx/invert_gravity.wav");
		death_fx = App->audio->LoadFx("audio/fx/death.wav"); 

		player_starts = false;
	}

	//-----------------------------------------------
	if (graphics == nullptr)
	{
		graphics = App->tex->Load("textures/adventurer.png");
	}
	
	App->render->camera.x = Player.camera_position.x;
	App->render->camera.y = Player.camera_position.y;

	if (initial_pos)
	{
		position.x = Player.position.x;
		position.y = Player.position.y;
		
		invert_gravity = false; 
		initial_pos = false;
	}
	//Player HitBox
	if (collider == nullptr)
	{
		collider = App->collision->AddCollider({ (int)position.x, (int)position.y, player_width+15, player_height+5 }, COLLIDER_PLAYER, App->entityManager);
	}
	can_move = true;
	current_animation = &idle;
	current_healing = &healing; 
	death.Reset(); 

	return true;
}

bool j1Player::CleanUp()
{
	App->tex->UnLoad(graphics); 
	graphics = nullptr;

	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider = nullptr;
	}
	return true;
}

bool j1Player::Update(float dt, bool do_logic)
{
	BROFILER_CATEGORY("Player Update", Profiler::Color::Green);

	touching_above = false;
	touching_right = false;
	touching_left = false;
	touching_bottom = false;
	camera_goes_right = false;
	camera_goes_left = false;

	win1 = false;
	win2 = false;
	wintutorial = false; 

	dt_player = dt;

	//Normal Game Mode
	if (god_mode == false)
	{
		if(App->scene->actual_level != 0)
			GameMode();

		maxSpeed_y = Player.maxSpeed_y;

		//When player speed.y != 0  setting if the player is FALLING or JUMPING
		if (invert_gravity == false)
		{
			if (speed.y > 0) //falling
			{
				is_falling = true;
				is_jumping = false;
			}
			if (speed.y < 0) //jumping
			{
				is_jumping = true;
				is_falling = false;
				
			}
		}
		else if (invert_gravity == true)
		{
			if (speed.y > 0) //jumping
			{
				is_falling = false;
				is_jumping = true;
				
			}
			if (speed.y < 0) //falling
			{
				is_jumping = false;
				is_falling = true;
			}
		}

		// if the player is falling set falling animation
		if (is_falling == true && current_animation != &jumping && current_animation != &jump_turned )
		{
			if (invert_gravity == false)
				current_animation = &falling;
			else if (invert_gravity == true && flip == false)
				current_animation = &falling;
			else if (invert_gravity == true && flip == true)
				current_animation = &falling_turned;
		}	
	}
	else //if not Normal Mode --> God mode
	{
		if(App->scene->actual_level != 0)
			GodMode();
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN && App->scene->pause == false)
	{
		god_mode = !god_mode;
	}
	
	//Flip texture in case speed.x is negative
	Flip();

	//Set playerhitbox position
	collider->SetPos(position.x + (player_width/2), position.y);
	
	return true;
}

bool j1Player::PostUpdate()
{
	BROFILER_CATEGORY("Player PostUpdate", Profiler::Color::Red);

	//TIMER 
	if ((App->scene->actual_level == 1 || App->scene->actual_level == 2) && !App->scene->pause)
	{
		if (App->scene->loading_saved_game)
			timer = game_time.Read() + saved_timer;

		else
			timer = game_time.Read(); 

		paused_timer = timer;
	}
	else
	{
		timer = paused_timer;

		game_time.Start();
		game_time.DefineStartTime(-paused_timer);
	}

	if (flip == false)
	{
		if (invert_gravity == true)
		{
			App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(dt_player), SDL_FLIP_VERTICAL);
			if (is_healing)
			{
				if (healing.Finished() == false)
					App->render->Blit(graphics, position.x, position.y, &current_healing->GetCurrentFrame(dt_player), SDL_FLIP_VERTICAL);
				else
				{
					is_healing = false;
					healing.Reset();
				}
			}
		}
		else
		{
			App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(dt_player), SDL_FLIP_NONE);
			if (is_healing)
			{
				if (healing.Finished() == false)
					App->render->Blit(graphics, position.x, position.y, &current_healing->GetCurrentFrame(dt_player), SDL_FLIP_NONE);
				else
				{
					is_healing = false;
					healing.Reset(); 
				}
					
			}
		}
	}
	else 
	{
		if (invert_gravity == false)
		{
			App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(dt_player), SDL_FLIP_HORIZONTAL);
			if (is_healing)
			{
				if (healing.Finished() == false)
					App->render->Blit(graphics, position.x, position.y, &current_healing->GetCurrentFrame(dt_player), SDL_FLIP_HORIZONTAL);
				else
				{
					is_healing = false;
					healing.Reset();
				}
			}
		}
		else
		{
			App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(dt_player), SDL_FLIP_NONE);
			if (is_healing)
			{
				if (healing.Finished() == false)
					App->render->Blit(graphics, position.x, position.y, &current_healing->GetCurrentFrame(dt_player), SDL_FLIP_NONE);
				else
				{
					is_healing = false;
					healing.Reset();
				}
			}
		}
		
	}


	Check_Collision();
	
	//applaying movement to the player
	if (can_move)
	{
		if (App->scene->actual_level != 0)
		{
			position.x += speed.x*dt_player;
			position.y += speed.y*dt_player;
		}
		else if (App->scene->actual_level == 0)
		{
			position.x == position.x;
			position.y == position.y;
		}
		
		
	}

	CameraOnPlayer();

	//Camera limits
	if (App->render->camera.x * (-1) < limit_left)
	{
		App->render->camera.x = -limit_left;
	}

	if (App->render->camera.y < -limit_down)
	{
	App->render->camera.y = -limit_down;
	}

	if (App->render->camera.y > -limit_up)
	{
		App->render->camera.y = -limit_up;
	}

	if (App->render->camera.x * (-1) > limit_right)
	{
		App->render->camera.x = -limit_right; 
	}

	//Player limits	
	if (position.x < Player.player_limit_left)
		position.x = Player.player_limit_left;

	if (position.x > Player.player_limit_right)
		position.x = Player.player_limit_right;

	if (god_mode == false)
	{
		if (speed.x != 0 && touching_above && SDL_GetTicks() > run_time)
		{
			App->audio->PlayFx(run_fx, 0);
			run_time = SDL_GetTicks() + (1 / player_speed) + 450;
		}
	}
	
	//Parallax if the camera moves
	for (image = App->map->data.image_layers.start; image; image = image->next)
	{
		if (image->data->speed > 0)
		{
			if (camera_goes_left && position.x > 450)
			{
				image->data->position.x += image->data->speed*(dt_player);
			}
			else if (camera_goes_right && position.x < 5900)
			{
				image->data->position.x -= image->data->speed*(dt_player);
			}
		}
	}
	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_FLOOR) //Standard Floor
	{
		if (((c2->rect.y) > (c1->rect.y + (c1->rect.h - 30)))) //if player touches ground from above 
		{
			if (invert_gravity == false)
				touching_above = true;
			else
				touching_bottom = true;

		}
		else if ((c2->rect.x) > (c1->rect.x + c1->rect.w - 15)) //if player touches wall from right
		{
			touching_right = true;
		}
		else if ((c2->rect.x + (c2->rect.w)) < (c1->rect.x + 15)) //if player touches wall from left
		{
			touching_left = true;
		}
		else if ((c2->rect.y + (c2->rect.h)) < (c1->rect.y + 30)) //if player touches ground from bottom
		{
			if (invert_gravity == false)
				touching_bottom = true;
			else
				touching_above = true;
		}
	}
	else if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_FLOOR_JUMPABLE) //Floor that you can pass through
	{
		if (((c2->rect.y) > (c1->rect.y + (c1->rect.h - 19)))) //if player touches ground from above 
		{
			if (invert_gravity == false)
				touching_above = true;
		}
		else if ((c2->rect.y + (c2->rect.h)) < (c1->rect.y + 15)) //if player touches ground from bottom
		{
			if (invert_gravity == true)
				touching_above = true;
			else if (invert_gravity == true && is_falling == false)
				touching_bottom = true;
		}
	}
	else if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_ENEMY) //if player touches an enemy. 
	{
		image = App->map->data.image_layers.start;
		if (c2->rect.x > c1->rect.x || c2->rect.x < c1->rect.x || c2->rect.y > c1->rect.y || c2->rect.y < c1->rect.y)
		{
			if (god_mode == false)
			{ 
				current_animation = &death; 
				if (can_move)
				{
					App->audio->PlayFx(death_fx);
					lifes--; 
				}		
				can_move = false;
				image->data->position.x = 0;
				
				if (App->scene->level_to_load->data->lvl == 1 && lifes > 0)
				{
					App->fade->FadeToBlack(App->scene, App->scene, 1.3f);
					App->scene->start_pos = true;
					App->scene->loading_lvl1 = true; 
				}
				else if (App->scene->level_to_load->data->lvl == 2 && lifes > 0)
				{
					App->fade->FadeToBlack(App->scene, App->scene, 1.3f);
					App->scene->start_pos = true;
					App->scene->loading_lvl2 = true;
				}
				else if(lifes <= 0)
				{
					App->fade->FadeToBlack(App->scene, App->scene, 1.3f);
					App->scene->loading_menu = true;
				}
			}
		}
	}
	else if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WIN1)
	{
		if (App->scene->actual_level == 3)
			wintutorial = true;
		else
		{
			win1 = true;
		}
			
		can_move = false;
		
	}
	else if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WIN2)
	{
		win2 = true;
		can_move = false;
	}
}

//To flip Player texture
void j1Player::Flip()
{
	if (speed.x > 0)
		flip = false;
	else if (speed.x < 0) // if speed.x of the player is negative we flip 180� the player texture
		flip = true;
}

void j1Player::Check_Collision()
{
	//checking if player is touching ground from above
	if (touching_above == false)
	{
		if (invert_gravity == false)
		{
			speed.y +=1*dt_player; //Aplying "gravity"
			if (speed.y > maxSpeed_y)
				speed.y = maxSpeed_y;
		}
		else
		{
			speed.y -= 1*dt_player; //Aplying "gravity inverted"
			if (speed.y < -maxSpeed_y)
				speed.y = -maxSpeed_y;
		}
		
	}
	else if (touching_above == true && is_falling == true && god_mode == false)
	{
		speed.y = 0;
		is_falling = false;
		SetIdleAnimation(); //set animation to idle when player lands
		jumping.Reset(); //jumping frame reset to frame number 1

		if (!can_invert)
			can_invert = true;

	}
	else if (touching_above == true && is_falling == true && god_mode == true)
	{
		speed.y = 0;
		is_falling = false; 
	}
	
	if (touching_bottom == true)
	{
		if (invert_gravity == false)
		{
			speed.y = 1;
		}
		else
		{
			speed.y = -1;
		}

		is_falling == true;
	}

	//checking if player is touching ground from right
	if (touching_right == true)
	{
		if (speed.x > 0)
			speed.x = 0;
	}
	//checking if player is touching ground from left
	if (touching_left == true)
	{
		if (speed.x < 0)
			speed.x = 0;
	}

	if (win1 == true)
	{
		App->fade->FadeToBlack(App->scene, App->scene, 1.3f);
		if (apply_score)
		{
			player_score += 1000;
			apply_score = !apply_score;
		}
			
		initial_pos = true;
		App->scene->start_pos = true;
		App->scene->loading_lvl2 = true; 
	}
	if (win2 == true)
	{
		App->fade->FadeToBlack(App->scene, App->scene, 1.3f);
		initial_pos = true;
		App->scene->start_pos = true;
		App->scene-> loading_menu = true; 
	}
	if (wintutorial == true)
	{
		App->fade->FadeToBlack(App->scene, App->scene, 1.3f);
		initial_pos = true;
		App->scene->start_pos = true;
		App->scene->loading_lvl1 = true; 
	}
}

void j1Player::CameraOnPlayer()
{
	uint window_w, window_h;
	App->win->GetWindowSize(window_w, window_h);

	if (App->scene->actual_level != 0)
	{
		// for X AXIS 
		if (position.x > ((float)App->render->camera.x * -1) + ((5 * window_w) / 8))
		{
			App->render->camera.x -= ceilf(player_speed*dt_player);
			camera_goes_right = true;
		}
		else if (position.x < (App->render->camera.x*-1) + ((3 * window_w) / 8))
		{
			App->render->camera.x += (int)player_speed*dt_player;
			camera_goes_left = true;
		}

		// for Y AXIS
		if (position.y + 67 > (App->render->camera.y*-1) + ((5 * window_h) / 8))
		{
			App->render->camera.y -= 6 * dt_player;
		}
		else if (position.y < (App->render->camera.y*-1) + ((window_h) / 8))
		{
			App->render->camera.y += 6 * dt_player;
		}
	}		
}

void j1Player::SetIdleAnimation()
{
		if (invert_gravity == false)
			current_animation = &idle;
		else if (invert_gravity == true && flip == true)
			current_animation = &idle_turned;
		else if (invert_gravity == true && flip == false)
			current_animation = &idle;
}

//Save player position
bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node pos = data.append_child("position"); 
	pugi::xml_node gravity = data.append_child("invert_gravity");
	pugi::xml_node level = data.append_child("current_level"); 
	pugi::xml_node god = data.append_child("god_mode"); 
	pugi::xml_node life_node = data.append_child("lifes");
	pugi::xml_node timer_node = data.append_child("timer"); 
	pugi::xml_node coins_node = data.append_child("coins");
	pugi::xml_node score_node = data.append_child("score"); 

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;
	gravity.append_attribute("value") = invert_gravity;
	level.append_attribute("value") = App->scene->level_to_load->data->lvl;
	god.append_attribute("value") = god_mode; 
	life_node.append_attribute("value") = lifes; 
	timer_node.append_attribute("value") = timer; 
	coins_node.append_attribute("value") = collected_coins; 
	score_node.append_attribute("value") = player_score; 

	LOG("SAVING PLAYER");

	return true;
}

//Load player position
bool j1Player::Load(pugi::xml_node& data)
{
	position.x = data.child("player").child("position").attribute("x").as_int();
	position.y = data.child("player").child("position").attribute("y").as_int();
	invert_gravity = data.child("player").child("invert_gravity").attribute("value").as_bool();
	lifes = data.child("player").child("lifes").attribute("value").as_int();
	App->scene->Level_Load(data.child("player").child("current_level").attribute("value").as_int());
	god_mode = data.child("player").child("god_mode").attribute("value").as_bool();
	saved_timer = data.child("player").child("timer").attribute("value").as_int();
	collected_coins = data.child("player").child("coins").attribute("value").as_int(); 
	player_score = data.child("player").child("score").attribute("value").as_int(); 

	if (!App->scene->from_menu)
	{
		game_time.Start();
		game_time.DefineStartTime(-saved_timer);
	}

	return true; 
}

void j1Player::GameMode()
{
	collider->rect.h = player_height;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && can_move == true && App->scene->pause == false)
	{
		speed.x = -player_speed;
		if (current_animation != &jumping && &falling && &falling_turned && &jump_turned)
		{
			if (invert_gravity == false)
			{
				current_animation = &running;
			}
			else if (invert_gravity == true && flip == true)
			{
				current_animation = &run_turned;
			}
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && can_move==true && App->scene->pause == false)
	{
		speed.x = player_speed;
		if (current_animation != &jumping && &falling && &falling_turned && &jump_turned)
		{
			current_animation = &running;

		}

	}
	else // if not clicking anything
	{
		speed.x = 0;
		if (current_animation != &jumping && current_animation != &jump_turned)
		{
			SetIdleAnimation();
		}

	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && is_falling == false && is_jumping == false && can_move == true && App->scene->pause == false)
	{
		if (invert_gravity == true && flip == true)
		{
			current_animation = &jump_turned;
			speed.y = jump_force;
		}
		else if (invert_gravity == true && flip == false)
		{
			current_animation = &jumping;
			speed.y = jump_force;
		}
		else if (invert_gravity == false && flip == true)
		{
			current_animation = &jumping;
			speed.y = -jump_force;
		}
		else
		{
			current_animation = &jumping;
			speed.y = -jump_force;
		}
		App->audio->PlayFx(jump_fx, 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_L) == KEY_DOWN && can_invert == true && App->scene->pause == false)
	{
		invert_gravity = !invert_gravity;
		App->audio->PlayFx(invert_gravity_fx);
		can_invert = false;

	}

	image = App->map->data.image_layers.start;
	if (position.y < dead_limit_up || position.y > dead_limit_down)//Player dies if he falls
	{
		if (can_move)
		{
			App->audio->PlayFx(death_fx);
			if(App->scene->actual_level != 3)
				lifes--; 
		}
			
		can_move = false; 
		image->data->position.x = 0;
		
		if (App->scene->level_to_load->data->lvl == 1 && lifes > 0)
		{
			App->fade->FadeToBlack(App->scene, App->scene, 1.3f);
			App->scene->start_pos = true;
			App->scene->loading_lvl1 = true;
		}
		else if (App->scene->level_to_load->data->lvl == 2 && lifes > 0)
		{
			App->fade->FadeToBlack(App->scene, App->scene, 1.3f);
			App->scene->start_pos = true;
			App->scene->loading_lvl2 = true;
		}
		else if (App->scene->level_to_load->data->lvl == 3 && lifes > 0)
		{
			App->fade->FadeToBlack(App->scene, App->scene, 1.3f);
			App->scene->start_pos = true;
			App->scene->loading_tutorial = true;
		}
		else if (lifes <= 0)
		{
			App->fade->FadeToBlack(App->scene, App->scene, 1.3f); 
			App->scene->loading_menu = true;
		}
	}
}

void j1Player::GodMode() 
{
	maxSpeed_y = 0;
	collider->rect.h = Player.godmode_hitbox; 
	current_animation = &god_mode_anim;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && App->scene->pause == false)
	{
		speed.x = -player_speed;
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && App->scene->pause == false)
	{
		speed.x = player_speed;
	}
	else
	{
		speed.x = 0; 
	}

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT && App->scene->pause == false)
	{
		speed.y = -player_speed;
		maxSpeed_y = Player.maxSpeed_y;
		
	}
	else if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT && App->scene->pause == false)
	{
		is_falling = true; 
		speed.y = player_speed;
		maxSpeed_y = Player.maxSpeed_y;
	}
	//Player Limits 
	if (position.y < Player.player_limit_up)
		position.y = Player.player_limit_up;
	if (position.y > Player.player_limit_down)
		position.y = Player.player_limit_down;

	if (flip == true && invert_gravity == true)
		current_animation = &god_mode_turned; 

}

