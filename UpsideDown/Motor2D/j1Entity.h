#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "j1EntityManager.h"
#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "p2Log.h"
#include "j1Audio.h"

struct SDL_Texture;
struct Collider; 
struct Animation;

enum EntityType;

class j1Entity
{
public:

	j1Entity(int x, int y, EntityType type);
	virtual ~j1Entity() {};

	virtual bool Start() { return true; };
	virtual bool Update(float dt, bool do_logic) { return true; };
	virtual bool PostUpdate() { return true;};
	virtual bool CleanUp() { return true; };

	virtual void OnCollision(Collider* c1, Collider* c2) {}

	virtual bool Load(pugi::xml_node&) { return true; }
	virtual bool Save(pugi::xml_node&) const { return true; }
	virtual void LoadValues() {}

	virtual void Draw() {};

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
