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

bool j1EntityManager::PreUpdate()
{
	for (uint i = 0; i < 20; ++i)
	{
		if (Entity_Array[i].type != EntityType::UNKNOWN)
		{
			Spawn(Entity_Array[i]);
			Entity_Array[i].type = EntityType::UNKNOWN;
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

j1Entity* j1EntityManager::CreateEntity(EntityType type, int x, int y)
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
	case EntityType::PLAYER:
		ret = new j1Player(x, y, type);
		break;
	}
	if (ret != nullptr)
		entities.add(ret);
	return ret; 
}

void j1EntityManager::DestroyEnemies()
{
	for (int i = 0; i < 20; i++)
	{
		Entity_Array[i].type = EntityType::UNKNOWN;
	}

	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next) {
		if (iterator->data->type != EntityType::PLAYER)
		{
			iterator->data->CleanUp();
			int num = entities.find(iterator->data);
			RELEASE(entities.At(num)->data);
			entities.del(entities.At(num));
		}
	}
}

void j1EntityManager::AddEnemy(int x, int y, EntityType type)
{
	for (uint i = 0; i < 20; ++i)
	{
		if (Entity_Array[i].type == EntityType::UNKNOWN)
		{
			Entity_Array[i].type = type;
			Entity_Array[i].position.x = x;
			Entity_Array[i].position.y = y;
			break;
		}
	}
}

void j1EntityManager::AddPlayer()
{
	player = (j1Player*)CreateEntity(PLAYER, 0, 700);
}

void j1EntityManager::Spawn(const Info_Enemy& info)
{
	for (uint i = 0; i < 20; ++i)
	{
		if (Entity_Array[i].type != EntityType::UNKNOWN)
		{
			j1Entity* entity;
			if (Entity_Array[i].type == BAT)
				entity = new j1Bat(info.position.x, info.position.y, info.type);
			else if (Entity_Array[i].type == SMASHER)
				entity = new j1Smasher(info.position.x, info.position.y, info.type);
				

			entities.add(entity);
			entity->Start();
			break;
		}
	}
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	if (player != nullptr)
	{
		player->Load(data);
	}

	for (pugi::xml_node bat = data.child("bat").child("position"); bat; bat = bat.next_sibling()) {
		iPoint batpos = { bat.attribute("x").as_int(), bat.attribute("y").as_int() };
		AddEnemy(batpos.x, batpos.y, BAT);
	}
	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	player->Save(data.append_child("player"));

	pugi::xml_node bat = data.append_child("bat");
	pugi::xml_node smasher = data.append_child("smasher");

	p2List_item<j1Entity*>* iterator;

	for (iterator = entities.start; iterator; iterator = iterator->next)
	{
		if (iterator->data->type == BAT)
			iterator->data->Save(bat); 
		if (iterator->data->type == SMASHER)
			iterator->data->Save(smasher); 
	}

	p2List_item<j1Entity*>* iterator2;

	for (iterator2 = entities.start; iterator2; iterator2 = iterator2->next)
	{
		if (iterator2->data->type == BAT) {
			pugi::xml_node position = bat.append_child("position");
			position.append_attribute("x") = iterator2->data->position.x;
			position.append_attribute("y") = iterator2->data->position.y;
		}
		if (iterator2->data->type == SMASHER) {
			pugi::xml_node position = smasher.append_child("position");
			position.append_attribute("x") = iterator2->data->position.x;
			position.append_attribute("y") = iterator2->data->position.y;
		}
	}

	return true;
}