#ifndef __MODULE_ENTITY_H__
#define __MODULE_ENTITY_H__

#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "PugiXml/src/pugixml.hpp"

struct SDL_Texture;

enum class EntityType
{
	player,
	bat,
	smasher
};

class j1Entity : public j1Module
{
public:

	j1Entity(EntityType type);

	virtual ~j1Entity();

	bool Update(float dt);

	bool Calculate_Path();

	void Check_Entity_Collision(Collider* c1, Collider* c2);

	void LoadLogic(const char* animationPath);

private:

	void setAnimation();

public:
		
	EntityType		type; 

	iPoint			position; 
	iPoint			speed; 

	SDL_Texture*	graphics = nullptr; 

	Collider*		collider = nullptr; 

	Animation*		current_animation = nullptr;
	Animation		idle, run; 
	
	//Fx Sounds 
	uint			run; 

};
#endif // __MODULE_ENTITY_H__
