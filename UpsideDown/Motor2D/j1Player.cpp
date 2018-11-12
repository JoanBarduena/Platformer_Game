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

j1Player::j1Player() : j1Module()
{
	idle.LoadAnimations("idle"); 
	running.LoadAnimations("running"); 
	jumping.LoadAnimations("jumping"); 
	idle_turned.LoadAnimations("idle_turned");
	run_turned.LoadAnimations("run_turned");
	jump_turned.LoadAnimations("jump_turned");
	falling.LoadAnimations("falling"); 
	falling_turned.LoadAnimations("falling_turned"); 

	name.create("player");	
}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config)
{
	//PLayer initial position
	Player.position.x = config.child("position").attribute("x").as_int();
	Player.position.y = config.child("position").attribute("y").as_int();

	//camera limits
	Player.limit_up = config.child("limits").attribute("up").as_int();
	Player.limit_down = config.child("limits").attribute("down").as_int();
	Player.limit_left = config.child("limits").attribute("left").as_int();
	Player.limit_right = config.child("limits").attribute("right").as_int();

	//Player physic values
	Player.player_speed = config.child("speed_x").attribute("value").as_int();
	Player.maxSpeed_y = config.child("maxSpeed_y").attribute("value").as_int();
	Player.jump_force = config.child("jump_force").attribute("value").as_int();

	//Player dimensions
	Player.player_width = config.child("player_size").attribute("width").as_int();
	Player.player_height = config.child("player_size").attribute("height").as_int();

	//Camera initial position
	Player.camera_position.x = config.child("camera").attribute("x").as_int();
	Player.camera_position.y = config.child("camera").attribute("y").as_int();

	//Player limits 
	Player.player_limit_left = config.child("player_limit").attribute("left").as_int();
	Player.player_limit_right = config.child("player_limit").attribute("right").as_int();

	//Cooldown value
	Player.cooldown = config.child("cooldown").attribute("value").as_int();

	return true;
}

bool j1Player::Start()
{
	if (counter <= 0)
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

		counter++;
		LOG("counter %d", counter); 
	}
	//Initial Values 
	position.x = Player.position.x;
	position.y = Player.position.y;

	App->render->camera.x = Player.camera_position.x; 
	App->render->camera.y = Player.camera_position.y;

	graphics = App->tex->Load("textures/adventurer.png");
	
	//Player HitBox
	if (playerHitbox == nullptr)
		playerHitbox = App->collision->AddCollider({ position.x, position.y, player_width, player_height }, COLLIDER_PLAYER, this);

	//Loading Sounds FX
	jump = App->audio->LoadFx("audio/fx/Jump.wav");
	run = App->audio->LoadFx("audio/fx/Run.wav"); 

	cooldown = Player.cooldown; 

	return true;
}

bool j1Player::CleanUp()
{
	App->tex->UnLoad(graphics);
	if (playerHitbox != nullptr)
	{
		playerHitbox->to_delete = true;
		playerHitbox = nullptr;
	}
	return true;
}

bool j1Player::Update(float dt)
{

	touching_above = false;
	touching_right = false;
	touching_left = false;
	touching_bottom = false;
	camera_goes_right = false;
	camera_goes_left = false;
	win1 = false;
	win2 = false;

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

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN && cooldown == 80)
	{
		invert_gravity = !invert_gravity;
		cooldown = 0;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		god_mode = !god_mode;
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
	if(is_falling == true && current_animation /*!= &jumping && current_animation != &jump_turned*/)
	{ 
		if (invert_gravity == false)
			current_animation = &falling;
		else if (invert_gravity == true && flip == false)
			current_animation = &falling;
		else if (invert_gravity == true && flip == true)
			current_animation = &falling_turned;
	}

	/*LOG("camera.y %d", App->render->camera.y); */

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
		
	if(cooldown < 80)
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

if (App->render->camera.x* -1 > limit_right)
{
	App->render->camera.x = -limit_right; 
}


//Player limits	
if (position.x < Player.player_limit_left)
	position.x = Player.player_limit_left;

if (position.x > Player.player_limit_right)
	position.x = Player.player_limit_right;


if (speed.x != 0 && touching_above && SDL_GetTicks() > run_time)
{
	App->audio->PlayFx(run, 0);
	run_time = SDL_GetTicks() + (1 / player_speed) + 450;
}

// Parallax if the camera moves
p2List_item<ImageLayer*>* image = nullptr;
for (image = App->map->data.image_layers.start; image; image = image->next)
{
	if (image->data->speed > 0)
	{
		if (camera_goes_left && App->render->camera.x > 200)
		{
			image->data->position.x += image->data->speed;
		}
		else if (camera_goes_right)
		{
			image->data->position.x -= image->data->speed;
		}

	}
}


return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_FLOOR) //Standard Floor
	{

		if (((c2->rect.y) > (c1->rect.y + (c1->rect.h - 15)))) //if player touches ground from above 
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
		else if ((c2->rect.y + (c2->rect.h)) < (c1->rect.y + 15)) //if player touches ground from bottom
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

	else if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_LIMIT)
	{
		if ((c2->rect.y) > (c1->rect.y + (c1->rect.h - 15)) && invert_gravity == false) //if player touches ground from above 
		{
			if (god_mode == false)
			{
				position.x = Player.position.x;
				position.y = Player.position.y;
				invert_gravity = false;
				App->render->camera.x = Player.camera_position.x;
				App->render->camera.y = Player.camera_position.y;
			}
			else
				touching_above = true;

		}
		else if ((c2->rect.y + c2->rect.h) < (c1->rect.y + 15) && invert_gravity == true)
		{
			if (god_mode == false)
			{
				position.x = Player.position.x;
				position.y = Player.position.y;
				invert_gravity = false;
				App->render->camera.x = Player.camera_position.x;
				App->render->camera.y = Player.camera_position.y;
			}
			else
				touching_above = true;
		}
	}
	else if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WIN1)
	{
		win1 = true;
		App->fade->FadeToBlack(App->scene, App->scene); 
	}
	else if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_WIN2)
	{
		win2 = true;
		App->fade->FadeToBlack(App->scene, App->scene);
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

	if (win1 == true)
	{
		App->scene->Level_Load(2);
	}
	if (win2 == true)
	{
		App->scene->Level_Load(1);
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
		camera_goes_right = true;
	}
	else if (position.x < (App->render->camera.x*-1) + ((3 * window_w) / 8))
	{
		App->render->camera.x += player_speed;
		camera_goes_left = true;
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
	pugi::xml_node position_ = data.append_child("position");
	pugi::xml_node state = data.append_child("state");

	position_.append_attribute("x") = position.x;
	position_.append_attribute("y") = position.y;
	state.append_attribute("invert_gravity") = invert_gravity; 

	return true;
}

//Load player position
bool j1Player::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();
	invert_gravity = data.child("state").attribute("invert_gravity").as_bool();

	return true; 
}