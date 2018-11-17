#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "j1EntityManager.h"
#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "PugiXml/src/pugixml.hpp"

struct SDL_Texture;
struct Collider; 
struct Animation;

enum EntityType
{
	PLAYER,
	BAT,
	SMASHER,
	UNKNOWN
};

class j1Entity
{
public:

	j1Entity(int x, int y, EntityType type);
	virtual ~j1Entity();

	virtual bool Start();
	virtual bool Update(float dt, bool do_logic);
	virtual bool PostUpdate();
	virtual bool CleanUp();

	virtual void OnCollision(Collider* c1, Collider* c2);

	virtual bool Load(pugi::xml_node&);
	virtual bool Save(pugi::xml_node&) const;

	virtual void Draw();

public:
		
	fPoint			speed;
	fPoint			start_pos;
	fPoint			position;

	EntityType		type;

	SDL_Texture*	graphics = nullptr;
	Collider*		collider = nullptr;


	Animation*		current_animation = nullptr;

	//Fx Sounds 
	uint			run_fx;

};
#endif // __MODULE_ENTITY_H__
