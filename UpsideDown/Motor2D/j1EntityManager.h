#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Entity.h"


class j1Entity; 
class j1Player; 
enum class EntityType;

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	virtual ~j1EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	j1Entity* CreateEntity(EntityType type, int x, int y);
	void DestroyEntity(j1Entity* entity);

	void Spawn(const Info_Enemy& info);

public:

	p2List<j1Entity*> entities;
	Info_Enemy		Array_Info[20];

private:

	float accumulated_time;
	float UpdateCycle = 0.1f;

	bool do_logic = false;
	
};

#endif // !__ENTITYMANAGER_H__

