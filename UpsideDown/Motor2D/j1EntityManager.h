#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Entity.h"


class j1Entity; 
class j1Player;
class j1Bat; 

enum EntityType
{
	PLAYER,
	BAT,
	SMASHER,
	UNKNOWN
};

struct Info_Enemy
{
	EntityType type = EntityType::UNKNOWN;
	fPoint position;
};


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

	void OnCollision(Collider* c1, Collider* c2);

	j1Entity* CreateEntity(EntityType type, int x, int y);

	void DestroyEntity(j1Entity* entity);
	void AddEnemy(int x, int y, EntityType type);
	void AddPlayer();
	void Spawn(const Info_Enemy& info);

	bool reset_pos = false;

public:

	p2List<j1Entity*> entities;

	j1Player* player = nullptr;

private:

	Info_Enemy		Entity_Array[20];

	float accumulated_time;
	float UpdateCycle = 0.1f;

	bool do_logic = false;
	
};

#endif // __ENTITYMANAGER_H__

