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

	//Falling Animation
	jumping.PushBack({ 191, 244, 60, 71 });
	falling.PushBack({ 299, 244, 60, 71 });
	falling.loop = true;
	
}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1Player::Start()
{
	if (graphics == nullptr)
		graphics = App->tex->Load("textures/adventurer_v2.png");

	//initial values
	position.x = 0;
	position.y = 0;
	speed.x = 0;
	speed.y = 0;
	flip = false;
	touching_y = false;
	touching_right = false;
	touching_left = false;
	is_jumping = false;

	//standard animation
	current_animation = &idle;

	playerHitbox = App->collision->AddCollider({ position.x, position.y, 50, 67 }, COLLIDER_PLAYER, this);

	return true;
}

bool j1Player::CleanUp()
{
	App->tex->UnLoad(graphics);
	return true;
}

bool j1Player::Update(float dt)
{

	touching_y = false;
	touching_right = false;
	touching_left = false;

	if(current_animation != &jumping)
		current_animation = &idle;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		speed.x = -5;
		if(current_animation == &idle)
			current_animation = &running;
		
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		speed.x = 5;
		if (current_animation == &idle)
			current_animation = &running;
		
	}
	else
	{
		speed.x = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && is_falling == false && is_jumping == false)
	{
		current_animation = &jumping;
		speed.y = -25; //jumping force
		
	}
	
	//flip texture in case speed.x is negative
	Flip();

	//Set playerhitbox position
	playerHitbox->SetPos(position.x, position.y);


	if (speed.y > 0)
	{
		is_falling = true;
		is_jumping = false;
	}
	if (speed.y < 0)
	{
		is_jumping = true;
		is_falling = false;
	}
	
	if(is_falling == true && current_animation != &jumping)
	{ 
		current_animation = &falling;
	}

	return true;
}

bool j1Player::PostUpdate()
{
	if (flip == false)
		App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_NONE);
	else 
		App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);

	Check_Collision();
	
	//move player
	position.x += speed.x;
	position.y += speed.y;

	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_FLOOR)
	{
		
		if (((c2->rect.y) > (c1->rect.y + (c1->rect.h - 10)))) //if player touches ground from above 
		{
			touching_y = true;
			
		}
		else if ((c2->rect.x) > (c1->rect.x + c1->rect.w - 10)) //if player touches wall from right
		{
			touching_right = true;
		}
		else if ((c2->rect.x + (c2->rect.w)) < (c1->rect.x + 10)) //if player touches wall from left
		{
			touching_left = true;
		}
	}
	
}

//To flip Player texture
void j1Player::Flip()
{
	if (speed.x > 0)
		flip = false;
	else if (speed.x < 0)
		flip = true;
}

void j1Player::Check_Collision()
{
	//checking if player is touching ground from above
	if (touching_y == false)
	{
		speed.y += 1;
		if (speed.y > 10)
			speed.y = 10;
	}
	else if (touching_y == true && is_falling == true)
	{
		speed.y = 0;
		is_falling = false;
		current_animation = &idle;
		jumping.Reset();
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