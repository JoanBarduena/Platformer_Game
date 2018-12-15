#ifndef __j1HEART_H__
#define __j1HEART_H__

#include "j1Entity.h"
#include "j1PathFinding.h"
#include "j1Render.h"
#include "PugiXml/src/pugixml.hpp"

class j1Heart : public j1Entity
{
public:

	j1Heart(int x, int y, EntityType type);

	virtual ~j1Heart();

	bool Start();
	bool Update(float dt, bool do_logic);
	bool PostUpdate();
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void LoadValues();

	void Draw();

public:

	Animation idle;
	float dt_heart;

	uint heart_fx;

};

#endif // __j1HEART_H__