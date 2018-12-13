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
#include "j1Fonts.h"
#include "j1Gui.h"
#include "j1Gui_Elements.h"


j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
		
	level* menu = new level(0, "map_menu.tmx"); 
	level* lvl1 = new level(1, "map_p1.tmx");
	level* lvl2 = new level(2, "map_p2.tmx");
	level* tutorial = new level(3, "map_tutorial.tmx"); 

	levels_list.add(menu); 
	levels_list.add(lvl1);
	levels_list.add(lvl2);
	levels_list.add(tutorial); 

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
	
	//Loading animations
	player_running.LoadAnimations("player", "running");
	keyA.LoadAnimations("keys", "keyA"); 
	keyA_pressed.LoadAnimations("keys", "keyA_pressed");
	keyD.LoadAnimations("keys", "keyD");
	keyD_pressed.LoadAnimations("keys", "keyD_pressed");
	keyP.LoadAnimations("keys", "keyP");
	keyP_pressed.LoadAnimations("keys", "keyP_pressed");
	keyL.LoadAnimations("keys", "keyL");
	keyL_pressed.LoadAnimations("keys", "keyL_pressed");
	keySpace.LoadAnimations("keys", "keySpace");
	keySpace_pressed.LoadAnimations("keys", "keySpace_pressed");


	actual_level = 0;
	App->render->camera.x = -50; 
	App->render->camera.y = -300; 

	current = &player_running;

	debug_tex = App->tex->Load("maps/pathfinding_debug.png");
	graphics = App->tex->Load("textures/adventurer.png");
	keyboard = App->tex->Load("textures/keyboard.png"); 

	//App->audio->PlayMusic("audio/music/Galway.ogg");

	Create_UI_Elements();

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	BROFILER_CATEGORY("Scene PreUpdate", Profiler::Color::Blue);

	Prev_Mouse_Pos = Mouse_Pos;

	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene Update", Profiler::Color::Green);

	dt_scene = dt;

	// Gui ------------------------------------------------------------------------------------------
	p2List_item<Gui_Elements*>* iterator;

	for (iterator = App->gui->List_elem.start; iterator != nullptr; iterator = iterator->next)
	{
		if (iterator->data->type == Element_type::BUTTON)
		{
			if (iterator->data->funct == Function::PLAY && iterator->data->do_action)
			{
				App->fade->FadeToBlack(this, this);
				loading_tutorial = true;
			}
			else if (iterator->data->funct == Function::EXIT && iterator->data->do_action)
			{
				return false;
			}
			iterator->data->do_action = false;
		}

	}
	// -------------------------------------------------------------------------------------------

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		start_pos = false;
		App->fade->FadeToBlack(this, this);
		loading_saved_game = true; 
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

	if (actual_level == 0)
	{
		App->render->camera.x -= 2;
		if (App->render->camera.x < -1350)
		{
			App->render->camera.x = -50;
		}
	}

	//F1 Starts form the very first level 
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		start_pos = true;
		App->fade->FadeToBlack(this, this);
		loading_lvl1 = true;
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
			App->fade->FadeToBlack(this, this);
			loading_lvl2 = true;
		}
		else
		{
			App->fade->FadeToBlack(this, this);
			loading_lvl1 = true; 
		}
	}
	//F3 Starts the second level  
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
	{
		start_pos = true;
		App->fade->FadeToBlack(this, this);
		loading_lvl2 = true; 
	}

	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN)
	{
		App->fade->FadeToBlack(this, this);
		loading_menu = true; 
	}

	//Load Tutorial 
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN)
	{
		if(actual_level == 0)
		{ 
			App->fade->FadeToBlack(this, this);
			loading_tutorial = true;
		}
	}

	//From fade to levels
	if (App->fade->IsFading() == false)
	{
		if (loading_tutorial == true)
		{
			Level_Load(3);
			loading_tutorial = false;
		}
		if (loading_menu == true)
		{
			Level_Load(0); 
			loading_menu = false; 
		}
		if (loading_lvl1 == true)
		{
			Level_Load(1);
			loading_lvl1 = false; 
		}
		if (loading_lvl2 == true)
		{
			Level_Load(2);
			loading_lvl2 = false; 
		}
		if (loading_saved_game == true)
		{
			App->LoadGame();
			loading_saved_game = false; 
		}
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

	App->input->GetMousePosition(Mouse_Pos.x, Mouse_Pos.y);
	
	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	BROFILER_CATEGORY("Collision PostUpdate", Profiler::Color::Red);

	bool ret = true;
	
	if (actual_level == 0)
	{
		App->render->Blit(graphics, 100, 630, &current->GetCurrentFrame(dt_scene), SDL_FLIP_NONE, 0);
	}

	if (actual_level == 3)
	{
		if (actual_level == 3 && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
			current_keyA = &keyA_pressed;
		else
			current_keyA = &keyA;

		if (actual_level == 3 && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
			current_keyD = &keyD_pressed;
		else
			current_keyD = &keyD;

		if (actual_level == 3 && App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT)
			current_keyL = &keyL_pressed;
		else
			current_keyL = &keyL;

		if (actual_level == 3 && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
			current_keySpace = &keySpace_pressed;
		else
			current_keySpace = &keySpace;

		App->render->Blit(keyboard, 300, 900, &current_keyA->GetCurrentFrame(dt_scene), SDL_FLIP_NONE);
		App->render->Blit(keyboard, 350, 900, &current_keyD->GetCurrentFrame(dt_scene), SDL_FLIP_NONE);
		App->render->Blit(keyboard, 2200, 900, &current_keyL->GetCurrentFrame(dt_scene), SDL_FLIP_NONE);
		App->render->Blit(keyboard, 3400, 650, &current_keyL->GetCurrentFrame(dt_scene), SDL_FLIP_NONE);
		App->render->Blit(keyboard, 1100, 900, &current_keySpace->GetCurrentFrame(dt_scene), SDL_FLIP_NONE);
	}

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
		App->gui->Delete_UI_Elements();

		App->map->Load(level_to_load->data->mapPath.GetString());

		if(level_to_load->data->lvl != 3)
			RespawnEntities();

		App->entityManager->AddPlayer();
		App->entityManager->Start();
		App->entityManager->player->Start();
		actual_level = level_to_load->data->lvl;
	}
	else if (actual_level == level_to_load->data->lvl)
	{
		App->entityManager->player->CleanUp();

		if (level_to_load->data->lvl != 3)
			App->entityManager->DestroyEnemies();

		App->map->Load(level_to_load->data->mapPath.GetString());

		if (level_to_load->data->lvl != 3)
			RespawnEntities();

		App->entityManager->Start();
	}
	else if ((actual_level == 3 && level_to_load->data->lvl == 1) || (actual_level == 1 && level_to_load->data->lvl == 2) || (actual_level == 2 && level_to_load->data->lvl == 1))
	{
		App->entityManager->player->CleanUp();
		if (level_to_load->data->lvl != 3)
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
		App->render->camera.x = -50;
		App->render->camera.y = -300;
		actual_level = level_to_load->data->lvl;
		Create_UI_Elements();
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
	if (level_to_load->data->lvl == 1)
	{
		App->entityManager->CreateEntity(2060, 880, SMASHER);
		App->entityManager->CreateEntity(4125, 950, SMASHER);
		App->entityManager->CreateEntity(5571, 790, SMASHER);
		App->entityManager->CreateEntity(5874, 116, BAT);
		App->entityManager->CreateEntity(2430, 537, BAT);
		App->entityManager->CreateEntity(3770, 450, BAT);

		App->entityManager->CreateEntity(800, 950, COIN);
		App->entityManager->CreateEntity(900, 950, COIN);
	}
	else if (level_to_load->data->lvl == 2)
	{
		App->entityManager->CreateEntity(630, 700, SMASHER);
		App->entityManager->CreateEntity(5280, 600, SMASHER);
		App->entityManager->CreateEntity(4676, 820, SMASHER);
		App->entityManager->CreateEntity(2357, 650, BAT);
		App->entityManager->CreateEntity(4038, 570, BAT);
		App->entityManager->CreateEntity(5585, 118, BAT);
	}
}

void j1Scene::Create_UI_Elements()
{

	Gui_Elements* Play = App->gui->Create_Button(Element_type::BUTTON, { 415, 400 }, { 1070, 260 , 190, 49 }, App->gui->GetAtlas(), Function::PLAY);
	Gui_Elements* Exit = App->gui->Create_Button(Element_type::BUTTON, { 415, 500 }, { 1070, 260 , 190, 49 }, App->gui->GetAtlas(), Function::EXIT);

	App->gui->Create_Label(Element_type::LABEL, { 57, 6 }, { 0,0,77, 30 }, "PLAY", { 255,255,255,0 }, App->font->default, Play);
	App->gui->Create_Label(Element_type::LABEL, { 57, 8 }, { 0,0,70, 30 }, "EXIT", { 255,255,255,0 }, App->font->default, Exit);

}