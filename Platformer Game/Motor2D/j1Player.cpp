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

#include<stdio.h>

j1Player::j1Player() : j1Module()
{
	name.create("player");

	//Idle Animation
	idle.PushBack({ 70, 11, 50, 67 });
	idle.PushBack({ 184, 11, 50, 67 });
	idle.PushBack({ 292, 13, 50, 67 });
	idle.PushBack({ 398, 13, 50, 67 });
	
	//Running Animation
	running.PushBack({ 188, 97, 53, 66 });
	running.PushBack({ 294, 97, 53, 66 });
	running.PushBack({ 406, 97, 53, 66 });
	running.PushBack({ 518, 97, 53, 66 });
	running.PushBack({ 624, 97, 53, 66 });
	running.PushBack({ 733, 97, 53, 66 });
	
	//Jumping Animation
	jumping.PushBack({ 72, 172, 60, 71 });
	jumping.PushBack({ 181, 172, 60, 71 });
	jumping.PushBack({ 296, 172, 60, 71 });
	jumping.PushBack({ 399, 172, 60, 71 });
	jumping.PushBack({ 517, 172, 60, 71 });
	jumping.PushBack({ 619, 172, 60, 71 });
	jumping.PushBack({ 740, 172, 60, 71 });
	jumping.PushBack({ 65, 260, 60, 71 });
	jumping.PushBack({ 191, 244, 60, 71 });
	jumping.PushBack({ 299, 244, 60, 71 });
	jumping.loop = false;
	jumping.speed = 0.2;

	//Idle Animation Turned
	idle_turned.PushBack({ 1192, 118, 50, 69 });
	idle_turned.PushBack({ 1082, 118, 50, 69 });
	idle_turned.PushBack({ 970, 118, 50, 69 });
	idle_turned.PushBack({ 862, 118, 50, 69 });

	//Running Animation Turned
	run_turned.PushBack({ 1409, 213, 53, 66 });
	run_turned.PushBack({ 1302, 213, 53, 66 });
	run_turned.PushBack({ 1192, 213, 53, 66 });
	run_turned.PushBack({ 1073, 213, 53, 66 });
	run_turned.PushBack({ 972, 213, 53, 66 });
	run_turned.PushBack({ 863, 213, 53, 66 });

	//Jumping Animation Turned 
	jump_turned.PushBack({ 1528, 304, 60, 71 });
	jump_turned.PushBack({ 1418, 304, 60, 71 });
	jump_turned.PushBack({ 1307, 308, 60, 71 });
	jump_turned.PushBack({ 1199, 322, 60, 71 });
	jump_turned.PushBack({ 1094, 322, 60, 71 });
	jump_turned.PushBack({ 973, 324, 60, 71 });
	jump_turned.PushBack({ 863, 316, 60, 71 });
	jump_turned.PushBack({ 1078, 410, 60, 71 });
	jump_turned.PushBack({ 973, 405, 60, 71 });
	jump_turned.PushBack({ 863, 405, 60, 71 });
	jump_turned.loop = false;
	jump_turned.speed = 0.2;

	//Falling Animation
	falling.PushBack({ 191, 244, 60, 71 });
	falling.PushBack({ 299, 244, 60, 71 });
	falling.loop = true;

	//Falling Animation Turned 
	falling_turned.PushBack({ 973, 405, 60, 71 });
	falling_turned.PushBack({ 863, 405, 60, 71 });
	falling_turned.loop = true;
	
}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config)
{
	//PLayer initial position
	position.x = config.child("position").attribute("x").as_int();
	position.y = config.child("position").attribute("y").as_int();

	//camera limits
	limit_up = config.child("limits").attribute("up").as_int();
	limit_down = config.child("limits").attribute("down").as_int();
	limit_left = config.child("limits").attribute("left").as_int();
	limit_right = config.child("limits").attribute("right").as_int();

	//Player physic values
	player_speed = config.child("speed_x").attribute("value").as_int();
	maxSpeed_y = config.child("maxSpeed_y").attribute("value").as_int();
	jump_force = config.child("jump_force").attribute("value").as_int();

	//Player dimensions
	player_width = config.child("player_size").attribute("width").as_int();
	player_height = config.child("player_size").attribute("height").as_int();

	return true;
}

bool j1Player::Start()
{
	if (graphics == nullptr)
		graphics = App->tex->Load("textures/adventurer_v2.png");

	//standard animation
	current_animation = &idle;
	//Player HitBox
	playerHitbox = App->collision->AddCollider({ position.x, position.y, player_width, player_height }, COLLIDER_PLAYER, this);

	//Loading Sounds FX
	if (jump == 0)
		jump = App->audio->LoadFx("audio/fx/Jump.wav");
	if (run == 0)
		run = App->audio->LoadFx("audio/fx/Run.wav"); 

	cooldown = 100;

	return true;
}

bool j1Player::CleanUp()
{
	App->tex->UnLoad(graphics);

	return true;
}

bool j1Player::Update(float dt)
{

	touching_above = false;
	touching_right = false;
	touching_left = false;
	touching_bottom = false;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
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
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
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

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && is_falling == false && is_jumping == false)
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
		else
		{
			current_animation = &jumping;
			speed.y = -jump_force; 
		}
		App->audio->PlayFx(run, 0);
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && cooldown == 100)
	{
		invert_gravity = !invert_gravity;
		cooldown = 0;
	}
	

	
	//flip texture in case speed.x is negative
	Flip();

	//Set playerhitbox position
	playerHitbox->SetPos(position.x + (player_width/2), position.y);

	//----------------------------------------------------------------------------------
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
			App->audio->PlayFx(jump, 0);
		}
	}
	else if (invert_gravity == true)
	{
		if (speed.y > 0) //jumping
		{
			is_falling = false;
			is_jumping = true;
			App->audio->PlayFx(jump, 0);
		}
		if (speed.y < 0) //falling
		{
			is_jumping = false;
			is_falling = true;
			
		}
	}
	
	//----------------------------------------------------------------------------------

	// if the player is falling set falling animation
	if(is_falling == true && current_animation != &jumping && current_animation != &jump_turned)
	{ 
		if (invert_gravity == false)
			current_animation = &falling;
		else if (invert_gravity == true && flip == false)
			current_animation = &falling;
		else if (invert_gravity == true && flip == true)
			current_animation = &falling_turned;
	}

	return true;
}

bool j1Player::PostUpdate()
{
	if (flip == false)
	{
		if (invert_gravity == true)
		{
			App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_VERTICAL);
		}
		else
		{
			App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_NONE);
		}
	}
	else if (flip == true)
	{
		if (invert_gravity == false)
		{
			App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);
		}
		else
		{
			App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_NONE);
		}
		
	}
		
	if(cooldown < 100)
		cooldown++;

	Check_Collision();
	
	//applaying movement to the player
	position.x += speed.x;
	position.y += speed.y;

	CameraOnPlayer();
	//Camera limits
	if (App->render->camera.x*-1 < limit_left)
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
		

	//Player limits	
	if (position.x < 6)
		position.x = 6;

	if (speed.x != 0 && touching_above && SDL_GetTicks() > run_time)
	{
		App->audio->PlayFx(run, 0);
		run_time = SDL_GetTicks() + (1 / player_speed) + 450;
	}

	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_FLOOR) //Standard Floor
	{
		
		if (((c2->rect.y) > (c1->rect.y + (c1->rect.h - 15)))) //if player touches ground from above 
		{
			if(invert_gravity == false)
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
		else if ((c2->rect.y + (c2->rect.h) ) < (c1->rect.y + 15)) //if player touches ground from bottom
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
			if(invert_gravity == false)
				touching_above = true;
		}
		else if ((c2->rect.y + (c2->rect.h)) < (c1->rect.y + 15)) //if player touches ground from bottom
		{
			if (invert_gravity == true)
				touching_above = true;
		}
	}
}

//To flip Player texture
void j1Player::Flip()
{
	if (speed.x > 0)
		flip = false;
	else if (speed.x < 0) // if speed.x of the player is negative we flip 180º the player texture
		flip = true;
}

void j1Player::Check_Collision()
{
	//checking if player is touching ground from above
	if (touching_above == false)
	{
		if (invert_gravity == false)
		{
			speed.y += 1; //Aplying "gravity"
			if (speed.y > maxSpeed_y)
				speed.y = maxSpeed_y;
		}
		else
		{
			speed.y -= 1; //Aplying "gravity inverted"
			if (speed.y < -maxSpeed_y)
				speed.y = -maxSpeed_y;
		}
		
	}
	else if (touching_above == true && is_falling == true)
	{
		speed.y = 0;
		is_falling = false;
		SetIdleAnimation(); //set animation to idle when player lands
		jumping.Reset(); //jumping frame reset to frame number 1
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
}

void j1Player::CameraOnPlayer()
{
	uint window_w, window_h;
	App->win->GetWindowSize(window_w, window_h);


	// for X AXIS 
	if (position.x >(App->render->camera.x * -1) + ((5 * window_w) / 8))
	{
		App->render->camera.x -= player_speed;
	}
	else if (position.x < (App->render->camera.x*-1) + ((3 * window_w) / 8))
	{
		App->render->camera.x += player_speed;
	}

	// for Y AXIS
	if (position.y + 67 > (App->render->camera.y*-1) + ((5 * window_h) / 8))
	{
		App->render->camera.y -= 6;
	}
	else if (position.y < (App->render->camera.y*-1) + ((window_h) / 8))
	{
		App->render->camera.y += 6;
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
	pugi::xml_node player = data.append_child("player");

	player.append_attribute("x") = position.x;
	player.append_attribute("y") = position.y;

	return true;
}

//Load player position
bool j1Player::Load(pugi::xml_node& data)
{
	position.x = data.child("player").attribute("x").as_int();
	position.y = data.child("player").attribute("y").as_int();

	return true; 
}