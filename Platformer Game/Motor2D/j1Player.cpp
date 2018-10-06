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
	position.y = 925;
	speed.x = 0;
	speed.y = 0;
	flip = false;

	current_animation = &idle;

	return true;
}

bool j1Player::CleanUp()
{
	App->tex->UnLoad(graphics);
	return true;
}

bool j1Player::Update(float dt)
{
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

	//move player
	position.x += speed.x;

	

	return true;
}

bool j1Player::PostUpdate()
{
	if (flip == false)
		App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_NONE);
	else 
		App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame(), SDL_FLIP_HORIZONTAL);

	return true;
}

void j1Player::Flip()
{
	if (speed.x > 0)
		flip = false;
	else if (speed.x < 0)
		flip = true;
}