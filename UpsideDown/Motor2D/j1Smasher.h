#ifndef __j1SMASHER_H__
#define __j1SMASHER_H__

#include "j1Entity.h"
#include "j1PathFinding.h"
#include "j1Render.h"
#include "PugiXml/src/pugixml.hpp"

#define GRAVITY 1

class j1Smasher : public j1Entity
{
public:

	j1Smasher(int x, int y, EntityType type);

	virtual ~j1Smasher();

	bool Start();
	bool Update(float dt, bool do_logic);
	bool PostUpdate();
	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	void LoadValues();
	void Move(const p2DynArray<iPoint>& path, float dt);

	void Draw();

public:

	const p2DynArray<iPoint>* path;
	bool created_path = false;
	Direction smasher_direction;
	iPoint Smasher_center;
	iPoint Smasher_dest;

	Animation idle, running;

	int Smasher_range = 400;
	int max_speed_y = 10;

	float dt_smasher;
	bool flip;

	bool touching_above, touching_bottom, touching_right, touching_left;
	

};

#endif // __j1SMASHER_H__
