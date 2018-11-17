#include "j1App.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Bat.h"

j1Bat::j1Bat(int x, int y, EntityType type) : j1Entity(x, y, EntityType::BAT)
{
	
}

j1Bat::~j1Bat(){}

bool j1Bat::Start()
{
	graphics = App->tex->Load("textures/bat_sprite.png");
	if (graphics == NULL)
	{
		LOG("bat sprite not loaded");
		return false;
	}
	collider = App->collision->AddCollider({ (int)position.x, (int)position.y, 53, 45 }, COLLIDER_NONE, App->entityManager);

	return true;
}

bool j1Bat::Update(float dt, bool do_logic)
{

	collider->SetPos(position.x, position.y);

	return true;
}

bool j1Bat::CleanUp()
{
	return true;
}

void j1Bat::OnCollision(Collider* c1, Collider* c2)
{

}

bool j1Bat::Load(pugi::xml_node&)
{
	return true;
}
bool j1Bat::Save(pugi::xml_node&) const
{
	return true;
}

void  j1Bat::LoadValues()
{

}

void j1Bat::Move(p2DynArray<iPoint>& path, float dt)
{

}
