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

#include<stdio.h>

j1Player::j1Player() : j1Module()
{
	name.create("player");

	idle.PushBack({ 70, 11, 50, 67 });
	idle.PushBack({ 184, 11, 50, 67 });
	idle.PushBack({ 292, 13, 50, 67 });
	idle.PushBack({ 398, 13, 50, 67 });

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
	speed = 2;

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
		position.x -= speed;

	}
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x += speed;
	}
	

	

	return true;
}

bool j1Player::PostUpdate()
{
	App->render->Blit(graphics, position.x, position.y, &current_animation->GetCurrentFrame());
	return true;
}