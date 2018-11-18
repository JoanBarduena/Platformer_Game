#include "j1App.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Bat.h"
#include "j1Scene.h"

j1Bat::j1Bat(int x, int y, EntityType type) : j1Entity(x, y, EntityType::BAT)
{
	current_animation = NULL;

	idle.LoadAnimations("bat", "flying");
	position.x = x;
	position.y = y;
}

void  j1Bat::LoadValues()
{

}

j1Bat::~j1Bat(){}

bool j1Bat::Start()
{
	if(graphics == nullptr)
	{
		graphics = App->tex->Load("textures/bat_sprite.png");
	}
	if (collider == nullptr)
	{
		collider = App->collision->AddCollider({ (int)position.x, (int)position.y, 31, 28 }, COLLIDER_ENEMY, App->entityManager);
	}
	
	speed.x = speed.y = 5;
	current_animation = &idle;
	return true;
}

bool j1Bat::Update(float dt, bool do_logic)
{
	dt_bat = dt;
	collider->SetPos(position.x, position.y);

	if (do_logic == true)
	{
		if ((App->entityManager->player->position.x - position.x) <= Bat_range && (App->entityManager->player->position.x - position.x) >= -Bat_range)
		{
			iPoint Player_center = { App->map->WorldToMap((int)App->entityManager->player->position.x + App->entityManager->player->player_width/2, (int)App->entityManager->player->position.y + App->entityManager->player->player_height/2)};
			Bat_center = { App->map->WorldToMap((int)position.x + 31 / 2, (int)position.y + 28 / 2 )};
			Bat_dest = Player_center;

			/*LOG("BAT %uc", App->pathfinding->IsWalkable(Bat_center));
			LOG("bat pos %i,%i", Bat_center.x, Bat_center.y);
			LOG("PLA %uc", App->pathfinding->IsWalkable(Bat_dest));
			LOG("PLA pos %i,%i", Bat_dest.x, Bat_dest.y);*/

			int ret = App->pathfinding->CreatePath(Bat_center, Bat_dest);
			path = App->pathfinding->GetLastPath();
			Move(*path, dt);

		}
	}
	
	return true;
}

bool j1Bat::PostUpdate()
{
	Draw();
	return true;
}

bool j1Bat::CleanUp()
{
	App->tex->UnLoad(graphics);
	graphics = nullptr;
	if (collider != nullptr)
		collider = nullptr;
	return true;
}

void j1Bat::Draw()
{
	SDL_Rect rect = current_animation->GetCurrentFrame(dt_bat);
	App->render->Blit(graphics, (int)position.x, (int)position.y, &rect, SDL_FLIP_NONE);
}

void j1Bat::OnCollision(Collider* c1, Collider* c2)
{

}

void j1Bat::Move(const p2DynArray<iPoint>& path, float dt)
{
	bat_direction = App->pathfinding->SetDirection(path);
	
	if (bat_direction == Direction::NORTH)
	{
		position.y -= speed.y * dt;
	}
	else if (bat_direction == Direction::EAST)
	{
		position.x += speed.x * dt;
	}
	else if (bat_direction == Direction::SOUTH)
	{
		position.y += speed.y * dt;
	}
	else if (bat_direction == Direction::WEST)
	{
		position.x -= speed.x * dt;
	}
	else if (bat_direction == Direction::SOUTH_EAST)
	{
		position.x += speed.x * dt;
		position.y += speed.y * dt;
	}
	else if (bat_direction == Direction::SOUTH_WEST)
	{
		position.x -= speed.x * dt;
		position.y += speed.y * dt;
	}
	else if (bat_direction == Direction::NORTH_EAST)
	{
		position.x += speed.x * dt;
		position.y -= speed.y * dt;
		
	}
	else if (bat_direction == Direction::NORTH_WEST)
	{
		position.x -= speed.x * dt;
		position.y -= speed.y * dt;
	}


}

bool j1Bat::Load(pugi::xml_node&)
{
	return true;
}
bool j1Bat::Save(pugi::xml_node&) const
{
	return true;
}




