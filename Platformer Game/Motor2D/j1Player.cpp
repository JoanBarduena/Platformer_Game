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

#include<stdio.h>

j1Player::j1Player() : j1Module()
{
	name.create("player");

	idle.PushBack({ 70, 11, 50, 67 });
	idle.PushBack({ 184, 11, 50, 67 });
	idle.PushBack({ 292, 13, 50, 67 });
	idle.PushBack({ 398, 13, 50, 67 });

	running.PushBack({ 188, 97, 53, 66 });
	running.PushBack({ 294, 97, 53, 66 });
	running.PushBack({ 406, 97, 53, 66 });
	running.PushBack({ 518, 97, 53, 66 });
	running.PushBack({ 624, 97, 53, 66 });
	running.PushBack({ 733, 97, 53, 66 });

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

	position.x = 0;
	position.y = 0;
	speed.x = 0;
	speed.y = 0;

	flip = false;
	touching = false;

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

	touching = false;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		speed.x = -4;
		current_animation = &running;
		
	}
	else if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		speed.x = 4;
		current_animation = &running;
		
	}
	else
	{
		speed.x = 0;
		current_animation = &idle;
		
	}
	
	//flip texture if speed
	Flip();

	//Set playerhitbox position
	playerHitbox->SetPos(position.x, position.y);
	

	return true;
}

bool j1Player::PostUpdate()
{
	if (flip == false)
		App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_NONE);
	else 
		App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);

	//checking if player is touching ground
	if (touching == false)
	{
		speed.y += 1;
		if (speed.y > 7)
			speed.y = 7;
	}
	else
	{
		speed.y = 0;
	}

	//move player
	position.x += speed.x;
	position.y += speed.y;

	return true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_PLAYER && c2->type == COLLIDER_FLOOR && touching == false)
	{
		//if player touches ground from above 
		if (((c2->rect.y) > (c1->rect.y + (c1->rect.h - 10)))) 
		{
			touching = true;
		}
			
	}
	
}

void j1Player::Flip()
{
	if (speed.x > 0)
		flip = false;
	else if (speed.x < 0)
		flip = true;
}