#ifndef __MODULE_ENTITY_H
#define __MODULE_ENTITY_H__

#include "j1EntityManager.h"
#include "j1Module.h"
#include "Animation.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1Timer.h"
#include "PugiXml/src/pugixml.hpp"

//struct SDL_Texture;
//struct Collider; 
//struct Animation;
//
//enum class EntityType
//{
//	PLAYER,
//	BAT,
//	SMASHER,
//	UNKNOWN
//};
//
//class j1Entity : public j1Module
//{
//public:
//
//	j1Entity(int x, int y, EntityType type);
//
//	virtual ~j1Entity();
//
//	bool UpdateFrame(float dt);
//
//	bool Update();
//
//
//	bool Calculate_Path();
//
//	void Check_Entity_Collision(Collider* c1, Collider* c2);
//
//	void LoadLogic(const char* animationPath);
//
//private:
//
//	void setAnimation();
//
//public:
//		
//	EntityType		type; 
//
//	iPoint			position; 
//	iPoint			speed; 
//
//	SDL_Texture*	graphics = nullptr; 
//
//	Collider*		collider = nullptr; 
//
//	Animation*		current_animation = nullptr;
//	Animation		idle, run; 
//	
//	//Fx Sounds 
//	uint			run_fx; 
//
//};
#endif // __MODULE_ENTITY_H__
