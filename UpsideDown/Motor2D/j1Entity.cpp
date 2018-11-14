#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Audio.h"
#include "p2Log.h"
#include "j1EntityManager.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1PathFinding.h"
#include "j1Player.h"

j1Entity::j1Entity(EntityType type):type(type)
{	
	
}

j1Entity::~j1Entity()
{
}

bool j1Entity::Update(float dt)
{
	return true;
}

bool j1Entity::Calculate_Path()
{
	return true; 
}

void j1Entity::Check_Entity_Collision(Collider* c1, Collider* c2)
{
	
}

void j1Entity::setAnimation()
{

}

void j1Entity::LoadLogic(const char* animationPath)
{
	
}