#include "j1App.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Smasher.h"

j1Smasher::j1Smasher(int x, int y, EntityType type) : j1Entity(x, y, EntityType::BAT)
{
	current_animation = NULL;

	idle.LoadAnimations("smasher", "run");
	position.x = x;
	position.y = y;
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
		collider = App->collision->AddCollider({ (int)position.x, (int)position.y, 31, 28 }, COLLIDER_ENEMY, App->entityManager);
	}

	speed.x = speed.y = 3;
	current_animation = &idle;
	return true;
}

bool j1Smasher::Update(float dt, bool do_logic)
{
	dt_smasher = dt;
	collider->SetPos(position.x, position.y);

	if (do_logic == true)
	{
		if ((App->entityManager->player->position.x - position.x) <= Smasher_range && (App->entityManager->player->position.x - position.x) >= -Smasher_range)
		{
			iPoint Player_center = { App->map->WorldToMap((int)App->entityManager->player->position.x + App->entityManager->player->player_width / 2, (int)App->entityManager->player->position.y + App->entityManager->player->player_height / 2) };
			Smasher_center = { App->map->WorldToMap((int)position.x + 31 / 2, (int)position.y + 28 / 2) };
			Smasher_dest = Player_center;

			int ret = App->pathfinding->CreatePath(Smasher_center, Smasher_dest);
			path = App->pathfinding->GetLastPath();
			Move(*path, dt);

		}
	}

	return true;
}

bool j1Smasher::PostUpdate()
{
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
	App->render->Blit(graphics, (int)position.x, (int)position.y, &rect, SDL_FLIP_NONE);
}

void j1Smasher::OnCollision(Collider* c1, Collider* c2)
{

}

void j1Smasher::Move(const p2DynArray<iPoint>& path, float dt)
{
	smasher_direction = App->pathfinding->SetDirection(path);

	if (smasher_direction == Direction::NORTH)
	{
		position.y -= speed.y * dt;
	}
	else if (smasher_direction == Direction::EAST)
	{
		position.x += speed.x * dt;
	}
	else if (smasher_direction == Direction::SOUTH)
	{
		position.y += speed.y * dt;
	}
	else if (smasher_direction == Direction::WEST)
	{
		position.x -= speed.x * dt;
	}
	else if (smasher_direction == Direction::SOUTH_EAST)
	{
		position.x += speed.x * dt;
		position.y += speed.y * dt;
	}
	else if (smasher_direction == Direction::SOUTH_WEST)
	{
		position.x -= speed.x * dt;
		position.y += speed.y * dt;
	}
	else if (smasher_direction == Direction::NORTH_EAST)
	{
		position.x += speed.x * dt;
		position.y -= speed.y * dt;

	}
	else if (smasher_direction == Direction::NORTH_WEST)
	{
		position.x -= speed.x * dt;
		position.y -= speed.y * dt;
	}


}

bool j1Smasher::Load(pugi::xml_node&)
{
	return true;
}
bool j1Smasher::Save(pugi::xml_node&) const
{
	return true;
}


