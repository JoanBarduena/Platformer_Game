#include "j1EntityManager.h"
#include "p2Log.h"
#include "j1Render.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1Collision.h"
#include "j1Input.h"
#include "j1Player.h"
#include "j1Window.h"

j1EntityManager::j1EntityManager()
{
	name.create("entityManager");
}

j1EntityManager::~j1EntityManager()
{}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	
	return true;
}

bool j1EntityManager::Start()
{
	return true;
}

bool j1EntityManager::Update(float dt)
{

	return true;
}

bool j1EntityManager::PostUpdate(float dt)
{
	return true;
}

bool j1EntityManager::CleanUp()
{
	return true;
}


bool j1EntityManager::Load(pugi::xml_node& data)
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	return true;
}