#ifndef __j1BAT_H__
#define __j1BAT_H__

#include "j1Entity.h"
#include "j1PathFinding.h"
#include "j1Render.h"
#include "PugiXml/src/pugixml.hpp"

class j1Bat : public j1Entity
{
public:

	j1Bat(int x, int y, EntityType type);

	virtual ~j1Bat();

	bool Start();
	bool Update(float dt, bool do_logic);
	bool PostUpdate();
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void LoadValues();
	void Move(p2DynArray<iPoint>& path, float dt);

	void Draw();

public:

	p2DynArray<iPoint>* path;

	Animation idle, flying;

	float dt_bat;

};

#endif // __j1BAT_H__