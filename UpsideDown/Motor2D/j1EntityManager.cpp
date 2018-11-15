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
	name.create("entity");
}

j1EntityManager::~j1EntityManager() {}

bool j1EntityManager::Start()
{
	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->Start();
	}

	return true;
}

bool j1EntityManager::PreUpdate()
{
	BROFILER_CATEGORY("EntityManagerPreUpdate", Profiler::Color::Orange)

		for (uint i = 0; i < MAX_ENEMIES; ++i)
		{
			if (queue[i].type != ENTITY_TYPE::UNKNOWN)
			{
				SpawnEnemy(queue[i]);
				queue[i].type = ENTITY_TYPE::UNKNOWN;
			}
		}

	return true;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntityManagerUpdate", Profiler::Color::LightSeaGreen)

		for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
		{
			iterator->data->Update(dt);
		}

	return true;
}

bool j1EntityManager::PostUpdate()
{
	BROFILER_CATEGORY("EntityManagerPostUpdate", Profiler::Color::Yellow)

		for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
		{
			iterator->data->PostUpdate();
		}

	return true;
}

bool j1EntityManager::CleanUp()
{
	LOG("Freeing all enemies");

	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != nullptr; iterator = iterator->next)
	{
		iterator->data->CleanUp();
		int index = entities.find(iterator->data);
		RELEASE(entities.At(index)->data);
		entities.del(entities.At(index));
	}

	player = nullptr;

	return true;
}

j1Entity* j1EntityManager::CreateEntity(ENTITY_TYPE type, int x, int y)
{
	j1Entity* ret = nullptr;
	switch (type)
	{
	case PLAYER:
		ret = new j1Player(x, y, type);
		if (ret != nullptr) entities.add(ret); break;
	}
	return ret;
}

void j1EntityManager::AddEnemy(int x, int y, ENTITY_TYPE type)
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == ENTITY_TYPE::UNKNOWN)
		{
			queue[i].type = type;
			queue[i].position.x = x;
			queue[i].position.y = y;
			break;
		}
	}
}

void j1EntityManager::SpawnEnemy(const EnemyInfo& info)
{

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENTITY_TYPE::UNKNOWN)
		{
			j1Entity* entity;
			
			entities.add(entity);
			entity->Start();
		}
	}
}

void j1EntityManager::CreatePlayer()
{
	player = (j1Player*)CreateEntity(PLAYER);
}

void j1EntityManager::OnCollision(Collider* c1, Collider* c2)
{
	for (p2List_item<j1Entity*>* iterator = entities.start; iterator != NULL; iterator = iterator->next)
	{
		if (iterator->data->collider == c1)
		{
			iterator->data->OnCollision(c1, c2);
			break;
		}
	}
}

bool j1EntityManager::Load(pugi::xml_node& data)
{

}

bool j1EntityManager::Save(pugi::xml_node& data) const
{

}