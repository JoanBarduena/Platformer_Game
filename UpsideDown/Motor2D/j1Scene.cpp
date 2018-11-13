#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Fadetoblack.h"
#include "j1PathFinding.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
		
	level* lvl1 = new level(1, "map_p1.tmx");
	level* lvl2 = new level(2, "map_p2.tmx");

	levels_list.add(lvl1);
	levels_list.add(lvl2);

	actual_level = levels_list.start;
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	return ret; 
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load(levels_list.start->data->mapPath.GetString());

	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
	{
		App->pathfinding->SetMap(w, h, data);
		LOG("Create walkability");
	}
		

	RELEASE_ARRAY(data);

	debug_tex = App->tex->Load("maps/pathfinding_debug.png");

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	// debug pathfing ------------------
	static iPoint origin;
	static bool origin_selected = false;

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);

	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN)
	{
		if (origin_selected == true)
		{
			App->pathfinding->CreatePath(origin, p);
			origin_selected = false;
		}
		else
		{
			origin = p;
			origin_selected = true;
		}
	}

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();

	if(App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 3;

	if(App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 3;

	if(App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 3;

	if(App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 3;
	
	//F1 Starts form the very first level 
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		App->player->level_change = 0;
		Level_Load(1); 
	}

	//F2 Starts from the beginning of the current level
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		if (actual_level->data->lvl == 2)
		{
			App->player->level_change = 0;
			Level_Load(2);
			//App->fade->FadeToBlack(App->scene, App->scene);
		}
		else
		{
			App->player->level_change = 0;
			Level_Load(1);
			//App->fade->FadeToBlack(App->scene, App->scene);
		}
	}


	//F3 Starts the second level  
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		App->player->level_change = 0;
		Level_Load(2);
		//App->fade->FadeToBlack(App->scene, App->scene);
	}

	//App->render->Blit(img, 0, 0);
	App->map->Draw();

	int x, y;
	App->input->GetMousePosition(x, y);
	iPoint map_coordinates = App->map->WorldToMap(x - App->render->camera.x, y - App->render->camera.y);
	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d Tile:%d,%d",
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count(),
		map_coordinates.x, map_coordinates.y);

	App->win->SetTitle(title.GetString());

	// Debug pathfinding ------------------------------
	//int x, y;
	App->input->GetMousePosition(x, y);
	iPoint p = App->render->ScreenToWorld(x, y);
	p = App->map->WorldToMap(p.x, p.y);
	p = App->map->MapToWorld(p.x, p.y);

	App->render->Blit(debug_tex, p.x, p.y);

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();

	for (uint i = 0; i < path->Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
		App->render->Blit(debug_tex, pos.x, pos.y);
		LOG("EEEEEEEYYYYYYY");
	}

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	return true;
}

void j1Scene::Level_Load(uint number)
{
	p2List_item<level*>* lvl = levels_list.start;
	for (int i = 1; i < number; i++)
	{
		lvl = lvl->next;
	}
	actual_level = lvl;

	if (actual_level != nullptr)
	{
		////Starting the level & player
		App->map->Load(actual_level->data->mapPath.GetString());
		App->player->playerHitbox = nullptr;
		App->player->Start();
	}
	else
	{
		LOG("Actual Level (%u) is nullptr", number);
	}
}