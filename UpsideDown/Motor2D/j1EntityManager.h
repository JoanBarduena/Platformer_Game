#ifndef __ENTITY_MANAGER_H__
#define __ENTITY_MANAGER_H__

#include "j1Module.h"
#include "p2Defs.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Entity.h"

#define MAX_ENEMIES 20

class j1Entity; 
class j1Player; 

struct EnemyInfo
{
	ENTITY_TYPE type = ENTITY_TYPE::UNKNOWN;
	iPoint position;
};

class j1EntityManager : public j1Module
{
public:

	j1EntityManager();
	~j1EntityManager();

	//bool Awake(pugi::xml_node&);
	bool Start();
	bool PreUpdate(); 
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	j1Entity* CreateEntity(ENTITY_TYPE type, int x = 0, int y = 0);

	void OnCollision(Collider* c1, Collider* c2);
	void CreatePlayer();
	void AddEnemy(int x, int y, ENTITY_TYPE type);

	p2List<j1Entity*>	entities;

	j1Player*			player = nullptr;

private:

	void SpawnEnemy(const EnemyInfo& info);
	EnemyInfo			queue[MAX_ENEMIES];
};
#endif // !__ENTITY_MANAGER_H__

