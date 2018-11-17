#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1PathFinding.h"
#include "j1Player.h"

j1Entity::j1Entity(int x, int y, EntityType type){}

j1Entity::~j1Entity(){}

bool j1Entity::Start()
{
	return true;
}

bool j1Entity::Update(float dt, bool do_logic)
{
	return true;
}

bool j1Entity::PostUpdate()
{
	return true;
}

bool j1Entity::CleanUp() 
{
	return true;
}

bool j1Entity::Load(pugi::xml_node&)
{
	return true;
}

bool j1Entity::Save(pugi::xml_node&) const
{
	return true;
}

void j1Entity::OnCollision(Collider* c1, Collider* c2) {}

void j1Entity::Draw() {}
