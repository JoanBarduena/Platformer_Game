#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Audio.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Fadetoblack.h"
#include "j1PathFinding.h"
#include "j1EntityManager.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
		
	level* menu = new level(0, "map_menu.tmx"); 
	level* lvl1 = new level(1, "map_p1.tmx");
	level* lvl2 = new level(2, "map_p2.tmx");

	levels_list.add(menu); 
	levels_list.add(lvl1);
	levels_list.add(lvl2);

	level_to_load = levels_list.start;
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
	
	actual_level = 0;

	debug_tex = App->tex->Load("maps/pathfinding_debug.png");

	App->audio->PlayMusic("audio/music/Galway.ogg");

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene PreUpdate", Profiler::Color::Blue);

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
	BROFILER_CATEGORY("Scene Update", Profiler::Color::Green);

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		start_pos = false;
		App->LoadGame();
	}
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		start_pos = false; 
		App->SaveGame();
	}
		
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		App->render->camera.y += 3;

	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		App->render->camera.y -= 3;

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		App->render->camera.x += 3;

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x -= 3;

		
	
	//F1 Starts form the very first level 
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		start_pos = true;
		Level_Load(1); 
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN)
	{
		//counter for animation speed
		App->caped_frames = !App->caped_frames;
	}

	//F2 Starts from the beginning of the current level
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		start_pos = true;
		if (level_to_load->data->lvl == 2)
		{
			Level_Load(2);
		}
		else
		{
			Level_Load(1); 
		}
	}
	//F3 Starts the second level  
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		start_pos = true;
		Level_Load(2);
	}

	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		Level_Load(0);
	}

	App->audio->SetMusicVolume();
	App->map->Draw();

	const p2DynArray<iPoint>* path = App->pathfinding->GetLastPath();
	if (App->collision->debug)
	{
		for (uint i = 0; i < path->Count(); ++i)
		{
			iPoint pos = App->map->MapToWorld(path->At(i)->x, path->At(i)->y);
			App->render->Blit(debug_tex, pos.x, pos.y);
		}
	}
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Collision PostUpdate", Profiler::Color::Red);

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
	for (int i = 0; i < number; i++)
	{
		lvl = lvl->next;
	}
	level_to_load = lvl;

	//App->map->Load(level_to_load->data->mapPath.GetString());


	if (actual_level > 0)
	{
		if (start_pos)
			App->entityManager->player->initial_pos = true;
		else
			App->entityManager->player->initial_pos = false;
	}

	if (actual_level == 0 && level_to_load->data->lvl > 0)
	{
		App->map->Load(level_to_load->data->mapPath.GetString());
		RespawnEntities();
		App->entityManager->AddPlayer();
		App->entityManager->Start();
		App->entityManager->player->Start();
		actual_level = level_to_load->data->lvl;
	}
	else if (actual_level == level_to_load->data->lvl)
	{
		App->entityManager->player->CleanUp();
		App->entityManager->DestroyEnemies();
		App->map->Load(level_to_load->data->mapPath.GetString());
		RespawnEntities();
		App->entityManager->Start();
	}
	else if ((actual_level == 1 && level_to_load->data->lvl == 2) || (actual_level == 2 && level_to_load->data->lvl == 1))
	{
		App->entityManager->player->CleanUp();
		App->entityManager->DestroyEnemies();
		App->map->Load(level_to_load->data->mapPath.GetString());
		RespawnEntities();
		App->entityManager->Start();
		actual_level = level_to_load->data->lvl;
	}
	else if ((actual_level > 0) && (level_to_load->data->lvl == 0))
	{
		App->entityManager->CleanUp();
		App->entityManager->DestroyEnemies();
		App->entityManager->DestroyPlayer();
		App->map->Load(level_to_load->data->mapPath.GetString());
		actual_level = level_to_load->data->lvl;
		App->audio->PlayMusic("audio/music/Galway.ogg");
	}

	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
	{
		App->pathfinding->SetMap(w, h, data);
	}
	RELEASE_ARRAY(data);

}

void j1Scene::RespawnEntities()
{
	
	App->entityManager->CreateEntity(2700, 700, SMASHER);
	App->entityManager->CreateEntity(6020, 700, SMASHER);
	App->entityManager->CreateEntity(5200, 600, SMASHER);
	App->entityManager->CreateEntity(500, 500, BAT);
	App->entityManager->CreateEntity(2600, 500, BAT);
	App->entityManager->CreateEntity(6000, 500, BAT);

	App->entityManager->CreateEntity(800, 950, COIN);
	App->entityManager->CreateEntity(900, 950, COIN);
}