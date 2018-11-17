#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "j1Scene.h"
#include "j1Player.h"

#include "Brofiler/Brofiler.h"

j1EntityManager::j1EntityManager()
{
	name.create("entityManager");
}

j1EntityManager::~j1EntityManager() {}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	return true;
}

bool j1EntityManager::Start()
{
	bool ret = true;
	p2List_item<j1Entity*>* iterator;
	for (iterator = entities.start; iterator != NULL && ret == true; iterator = iterator->next)
	{
		ret = iterator->data->Start();
	}
	
	return ret;
}

bool j1EntityManager::PreUpdate()
{
	for (uint i = 0; i < 20; ++i)
	{
		if (Array_Info[i].type != EntityType::UNKNOWN)
		{
			Spawn(Array_Info[i]);
			LOG("Spawning Enemy");
			Array_Info[i].type = EntityType::UNKNOWN;
		}
	}


	return true;
}


bool j1EntityManager::Update(float dt)
{
	bool ret = true;

	accumulated_time += dt;
	if (accumulated_time >= UpdateCycle)
		do_logic = true;

	p2List_item<j1Entity*>* iterator;

	for( iterator = entities.start; iterator != NULL && ret == true; iterator = iterator->next)
	{
		ret = iterator->data->Update(dt, do_logic);
	}

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	bool ret = true;

	p2List_item<j1Entity*>* iterator;
		
	for ( iterator = entities.start; iterator != NULL && ret == true; iterator = iterator->next)
	{
		ret = iterator->data->PostUpdate();
	}

	return true;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;

	p2List_item<j1Entity*>* iterator;

	for ( iterator = entities.start; iterator != NULL && ret == true; iterator = iterator->next)
	{
		ret = iterator->data->CleanUp();
	}

	return ret;
}

j1Entity* j1EntityManager::CreateEntity(EntityType type, int x, int y)
{
	static_assert((int)EntityType::UNKNOWN == 3, "code needs update");

	j1Entity* ret = nullptr;
	switch (type)
	{
	case EntityType::PLAYER:

	case EntityType::BAT:

	case EntityType::SMASHER:
	}
	return ret; 
}

void j1EntityManager::DestroyEntity(j1Entity* entity)
{
	LOG("entity destroyed");
	RELEASE(entity);
}

void j1EntityManager::Spawn(const Info_Enemy& info)
{
	for (int i = 0; i < 20; ++i)
	{
		if (Array_Info[i].type != EntityType::UNKNOWN)
		{
			j1Entity* entity;
			if (Array_Info[i].type == EntityType::BAT)
			{
				//entity = new j1Bat(info.pos.x, info.pos.y, info.type)  --> constructor of entity
			}
				
		}
	}
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	return true;
}