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
	bool UpdateFrame(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	j1Entity* CreateEntity(EntityType type, int x = 0, int y = 0);
	void DestroyEntity(j1Entity* entity);

private:
	bool UpdateAll(float dt, bool check_logic);

public:

	p2List<j1Entity*> entities;
	j1Player* player;

private:
	float accumulated_time;
	bool do_logic = false;
};
#endif // !__ENTITY_MANAGER_H__

