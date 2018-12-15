#ifndef __j1COIN_H__
#define __j1COIN_H__

#include "j1Entity.h"
#include "j1PathFinding.h"
#include "j1Render.h"
#include "PugiXml/src/pugixml.hpp"

class j1Coin : public j1Entity
{
public:

	j1Coin(int x, int y, EntityType type);

	virtual ~j1Coin();

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
	float dt_coin;

	uint coin_fx; 

};

#endif // __j1COIN_H__