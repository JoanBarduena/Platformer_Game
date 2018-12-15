#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Collision.h"
#include "j1EntityManager.h"
#include "j1Scene.h"
#include "j1Player.h"
#include "j1Bat.h"
#include "j1Smasher.h"
#include "j1Coin.h"

#include "Brofiler/Brofiler.h"

j1EntityManager::j1EntityManager()
{
	name.create("entityManager");
}

j1EntityManager::~j1EntityManager() {}

bool j1EntityManager::Awake(pugi::xml_node& config)
{
	LOG("Awaking Entity manager");

	return true;
}

bool j1EntityManager::Start()
{
	bool ret = true;
	p2List_item<j1Entity*>* iterator;
	for (iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->Start();
	}
	
	return ret;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntityManager Update", Profiler::Color::Green);

	bool ret = true;

	accumulated_time += dt;
	if (accumulated_time >= UpdateCycle)
		do_logic = true;

	p2List_item<j1Entity*>* iterator;

	for( iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->Update(dt, do_logic);
	}

	if (do_logic) {
		accumulated_time = 0.0f;
		do_logic = false;
	}

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("EntityManager PostUpdate", Profiler::Color::Red);

	p2List_item<j1Entity*>* iterator;
		
	for ( iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->PostUpdate();
	}

	return true;
}

bool j1EntityManager::CleanUp()
{
	bool ret = true;

	p2List_item<j1Entity*>* iterator;

	for ( iterator = entities.start; iterator != nullptr && ret == true; iterator = iterator->next)
	{
		iterator->data->CleanUp();
	}

	return ret;
}

void j1EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	p2List_item<j1Entity*>* iterator;

	for ( iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		if (iterator->data->collider == c1)
		{
			iterator->data->OnCollision(c1, c2);
			break;
		}
	}
}

void j1EntityManager::CreateEntity(int x, int y, EntityType type)
{
	j1Entity* ret = nullptr;
	switch (type)
	{
	case EntityType::BAT:
		ret = new j1Bat(x, y, type);
		break;
	case EntityType::SMASHER:
		ret = new j1Smasher(x, y, type);
		break;
	case EntityType::COIN:
		ret = new j1Coin(x, y, type);
		break;
	}
	if (ret != nullptr)
		entities.add(ret);
}

void j1EntityManager::AddPlayer()
{
	player = new j1Player(0, 700, PLAYER);
	entities.add(player);
}

void j1EntityManager::DestroyEnemies()
{
	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next) {
		if (iterator->data->type != EntityType::PLAYER)
		{
			iterator->data->CleanUp();
			entities.del(iterator);
			RELEASE(iterator->data);
			
			LOG("deleting enemy");
		}
	}
	
}

void j1EntityManager::DestroyThisEntity(j1Entity* entity)
{
	for (p2List_item<j1Entity*>* iterator = entities.start; iterator; iterator = iterator->next)
	{
		if (iterator->data == entity)
		{
			iterator->data->CleanUp();
			entities.del(iterator);
			RELEASE(iterator->data);
		}
	}
}

void j1EntityManager::DestroyPlayer()
{
	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next) {
		if (iterator->data->type == EntityType::PLAYER)
		{
			iterator->data->CleanUp();
			entities.del(iterator);
			RELEASE(iterator->data);
			player = nullptr;
			LOG("deleting PLAYER");
		}
	}

}

bool j1EntityManager::Load(pugi::xml_node& data)
{



	CleanUp(); 
	if (player != nullptr)
	{
		player->Load(data);
	}

	pugi::xml_node entities_node = data.child("entities").child("entity");

	for (p2List_item<j1Entity*>* entity = entities.start; entity!=nullptr; entity = entity->next)
	{
		entity->data->position.x = entities_node.attribute("position_x").as_int();
	
		if(entity->data->type == SMASHER)
			entity->data->position.y = entities_node.attribute("position_y").as_int() - 70;
		else 
			entity->data->position.y = entities_node.attribute("position_y").as_int();

		entities_node = entities_node.next_sibling("entity"); 
	}

	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	player->Save(data.append_child("player"));

	//uint bat_counter = 0;
	//uint smasher_counter = 0;
	//uint coin_counter = 0; 

	pugi::xml_node entities_node = data.append_child("entities");
	
	for (p2List_item<j1Entity*>* entity = entities.start; entity; entity = entity->next)
	{
		pugi::xml_node child = entities_node.append_child("entity");

		child.append_attribute("position_x") = entity->data->position.x;
		child.append_attribute("position_y") = entity->data->position.y;
	}
	return true;
}