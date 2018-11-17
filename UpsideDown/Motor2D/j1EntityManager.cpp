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
	/*for (iterator = entities.start; iterator != NULL && ret == true; iterator = iterator->next)
	{
		ret = iterator->data->Start();
	}
	*/
	return ret;
}

bool j1EntityManager::PreUpdate()
{
	//for (uint i = 0; i < Max_Enemies; ++i)
	//{
	//	if (queue[i].type != ENTITY_TYPES::UNKNOWN)
	//	{
	//		SpawnEnemy(queue[i]);
	//		queue[i].type = ENTITY_TYPES::UNKNOWN;
	//	}
	//}

	//return true;

	return true;
}

bool j1EntityManager::UpdateFrame(float dt)
{
	/*accumulated_time += dt;

	float update_ms_cycle = 0.0f;

	if (accumulated_time >= update_ms_cycle)
		do_logic = true;

	UpdateAll(dt, do_logic);

	if (do_logic == true) {
		accumulated_time = 0.0f;
		do_logic = false;
	}*/
	return true;

}

bool j1EntityManager::UpdateAll(float dt, bool do_logic)
{
	bool ret = true;

	/*p2List_item<j1Entity*>* item;
	for (item = entities.start; item != NULL && ret == true; item = item->next)
	{
		if (do_logic == true) {
			ret = item->data->UpdateFrame(dt);
		}

		if (ret == true)
			ret = item->data->Update();
	}*/

	return ret;
}

bool j1EntityManager::PostUpdate()
{
	bool ret = true;
		
	/*for (p2List_item<j1Entity*>* iterator = entities.start; iterator != NULL && ret == true; iterator = iterator->next)
	{
		ret = iterator->data->PostUpdate();
	}*/

	return true;
}

bool j1EntityManager::CleanUp()
{
	LOG("Freeing all enemies");
	bool ret = true;
	/*for (p2List_item<j1Entity*>* iterator = entities.start; iterator != NULL && ret == true; iterator = iterator->next)
	{
		ret = iterator->data->CleanUp();
	}*/

	return ret;
}

j1Entity* j1EntityManager::CreateEntity(EntityType type, int x, int y)
{
	//static_assert((int)EntityType::UNKNOWN == 3, "code needs update");

	j1Entity* ret = nullptr;
	//switch (type)
	//{
	////Player is not an entity yet
	///*case EntityType::PLAYER:
	//	ret = new j1Player();
	//	break;*/
	//}
	return ret; 
}

void j1EntityManager::DestroyEntity(j1Entity* entity)
{
	/*LOG("entity destroyed");
	RELEASE(entity);*/
}

bool j1EntityManager::Load(pugi::xml_node& data)
{
	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	return true;
}