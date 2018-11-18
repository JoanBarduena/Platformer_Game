#include "j1App.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Smasher.h"

j1Smasher::j1Smasher(int x, int y, EntityType type) : j1Entity(x, y, EntityType::SMASHER)
{
	current_animation = NULL;

	idle.LoadAnimations("smasher", "run");
	position.x = x;
	position.y = y;

	start_pos.x = x;
	start_pos.y = y;
}

void  j1Smasher::LoadValues()
{

}

j1Smasher::~j1Smasher() {}

bool j1Smasher::Start()
{
	if (graphics == nullptr)
	{
		graphics = App->tex->Load("textures/skull_monster.png");
	}
	if (collider == nullptr)
	{
		collider = App->collision->AddCollider({ (int)position.x, (int)position.y, 53, 45 }, COLLIDER_ENEMY, App->entityManager);
	}

	position.x = start_pos.x;
	position.y = start_pos.y;
	flip = false;
	speed.x = speed.y = 3;
	current_animation = &idle;
	return true;
}

bool j1Smasher::Update(float dt, bool do_logic)
{
	BROFILER_CATEGORY("Smasher Update", Profiler::Color::Green);

	touching_above = false;
	touching_right = false;
	touching_left = false;
	touching_bottom = false;

	dt_smasher = dt;
	if (do_logic == true)
	{
		if ((App->entityManager->player->position.x - position.x) <= Smasher_range && (App->entityManager->player->position.x - position.x) >= -Smasher_range)
		{
			iPoint Player_center = { App->map->WorldToMap((int)App->entityManager->player->position.x + App->entityManager->player->player_width+1, (int)App->entityManager->player->position.y + App->entityManager->player->player_height / 2) };
			Smasher_center = { App->map->WorldToMap((int)position.x + 53 / 2, (int)position.y + 45 / 2) };
			Smasher_dest = Player_center;

			int ret = App->pathfinding->CreatePath(Smasher_center, Smasher_dest);
			path = App->pathfinding->GetLastPath();
			Move(*path, dt);

		}
	}
	position.y += speed.y*dt_smasher;
	collider->SetPos(position.x, position.y);
	return true;
}

bool j1Smasher::PostUpdate()
{
	BROFILER_CATEGORY("Smasher PostUpdate", Profiler::Color::Red);

	if (touching_above == false)
	{
		speed.y += GRAVITY * dt_smasher; //Aplying "gravity"
		if (speed.y > max_speed_y)
			speed.y = max_speed_y;
		//LOG("not touching above");
	}
	else if (touching_above == true)
	{
		speed.y = 0;
	}

	
	Draw();
	return true;
}

bool j1Smasher::CleanUp()
{
	App->tex->UnLoad(graphics);
	graphics = nullptr;
	if (collider != nullptr)
		collider = nullptr;
	return true;
}

void j1Smasher::Draw()
{
	SDL_Rect rect = current_animation->GetCurrentFrame(dt_smasher);
	if(flip)
		App->render->Blit(graphics, (int)position.x, (int)position.y, &rect, SDL_FLIP_HORIZONTAL);
	else
		App->render->Blit(graphics, (int)position.x, (int)position.y, &rect, SDL_FLIP_NONE);

}

void j1Smasher::OnCollision(Collider* c1, Collider* c2)
{
	if (c1->type == COLLIDER_ENEMY && c2->type == COLLIDER_FLOOR) //Standard Floor
	{
		if (((c2->rect.y) > (c1->rect.y + (c1->rect.h - 50)))) //if touches ground from above 
		{
			touching_above = true;
		}
		else if ((c2->rect.x) > (c1->rect.x + c1->rect.w - 15)) //if touches wall from right
		{
			touching_right = true;
		}
		else if ((c2->rect.x + (c2->rect.w)) < (c1->rect.x + 15)) //if touches wall from left
		{
			touching_left = true;
		}
		else if ((c2->rect.y + (c2->rect.h)) < (c1->rect.y + 50)) //if touches ground from bottom
		{
			touching_bottom = true;
		}
	}
}

void j1Smasher::Move(const p2DynArray<iPoint>& path, float dt)
{
	smasher_direction = App->pathfinding->SetDirection(path,"smasher");

	
	if (smasher_direction == Direction::EAST)
	{
		position.x += speed.x * dt;
		flip = false;
	}
	else if (smasher_direction == Direction::WEST)
	{
		position.x -= speed.x * dt;
		flip = true;

	}
	
}

bool j1Smasher::Load(pugi::xml_node& data)
{
	return true;
}
bool j1Smasher::Save(pugi::xml_node& data) const
{
	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;


	return true;
}


