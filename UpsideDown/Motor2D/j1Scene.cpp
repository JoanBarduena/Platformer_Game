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
	keyboard = App->tex->Load("textures/keyboard.png"); 

	App->entityManager->AddPlayer();
	App->entityManager->player->Start();

	//title = App->tex->Load("textures/upsidedown_title.png");
	//title = App->tex->Load("textures/upsidedown_title2.png");
	//title = App->tex->Load("textures/upsidedown_title3.png");
	title = App->tex->Load("textures/upsidedown_title4.png");

	App->audio->PlayMusic("audio/music/Galway.ogg");

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

	// Gui Logic ------------------------------------------------------------------------------------------

	for (p2List_item<Gui_Elements*>* iterator = App->gui->List_elem.start; iterator != nullptr; iterator = iterator->next)
	{
		if (iterator->data->type == Element_type::BUTTON && iterator->data->do_action == true && App->fade->IsFadingOut() == false)
		{
			if (iterator->data->funct == Function::PLAY)
			{
				LOG("CLICKING");
				App->entityManager->player->lifes = 3;
				if (actual_level == 0)
				{
					App->fade->FadeToBlack(this, this, 1.3f);
					loading_tutorial = true;
				}
			}
			else if (iterator->data->funct == Function::LOAD && saved_game)
			{
				start_pos = false;
				App->fade->FadeToBlack(this, this, 1.0f);
				loading_saved_game = true;
			}
			else if (iterator->data->funct == Function::SETTINGS )
			{
				pause = !pause;
				Main_Menu->visible = !Main_Menu->visible;
				Settings_Menu->visible = !Settings_Menu->visible;
			}
			else if (iterator->data->funct == Function::EXIT)
			{
				return false;
			}
			else if (iterator->data->funct == Function::GITHUB)
			{
				ShellExecuteA(NULL, "open", "https://github.com/JosepLleal/Platformer_Game", NULL, NULL, SW_SHOWNORMAL);
			}
			else if (iterator->data->funct == Function::SKIP)
			{
				start_pos = true;
				App->fade->FadeToBlack(this, this, 1.3f);
				loading_lvl1 = true;
			}

		}
		iterator->data->do_action = false;
		if (iterator->data->type == Element_type::IMAGE)
		{
			if (iterator->data == Menu)
			{
				for (p2List_item<Gui_Elements*>* iterator = Menu->childrens.start; iterator != nullptr; iterator = iterator->next)
				{
					if (iterator->data->type == Element_type::BUTTON && iterator->data->do_action == true && App->fade->IsFadingOut() == false)
					{
						if (iterator->data->funct == Function::RESUME)
						{
							pause = !pause;
							Menu->visible = !Menu->visible;
						}
						else if (iterator->data->funct == Function::LOAD && saved_game)
						{
							start_pos = false;
							App->fade->FadeToBlack(this, this, 1.0f);
							loading_saved_game = true;
						}
						else if (iterator->data->funct == Function::SAVE && actual_level != 3)
						{
							start_pos = false;
							App->SaveGame();
						}
						
						else if (iterator->data->funct == Function::SETTINGS)
						{
							Menu->visible = !Menu->visible;
							Settings_Menu->visible = !Settings_Menu->visible;
						}
						else if (iterator->data->funct == Function::MENU)
						{
							App->fade->FadeToBlack(this, this, 1.3f);
							loading_menu = true;
						}
					}
					iterator->data->do_action = false;
				}
			}
			if (iterator->data == Settings_Menu)
			{
				for (p2List_item<Gui_Elements*>* iterator = Settings_Menu->childrens.start; iterator != nullptr; iterator = iterator->next)
				{
					if (iterator->data->type == Element_type::BUTTON && iterator->data->do_action == true && App->fade->IsFadingOut() == false)
					{
						if (iterator->data->funct == Function::RESUME)
						{
							if (actual_level == 0)
							{
								pause = !pause;
								Settings_Menu->visible = !Settings_Menu->visible;
								Main_Menu->visible = !Main_Menu->visible;
							}
							else
							{
								Menu->visible = !Menu->visible;
								Settings_Menu->visible = !Settings_Menu->visible;
							}

						}
					}
					
				}
			}
		}
	}
	// -------------------------------------------------------------------------------------------

	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && !pause)
	{
		start_pos = false;
		App->fade->FadeToBlack(this, this, 1.0f);
		loading_saved_game = true;

	}
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && !pause && actual_level!=0 && actual_level != 3)
	{
		start_pos = false; 
		App->SaveGame();
	}
		

	if (actual_level == 0)
	{
		App->entityManager->player->position.x = 150;
	}

	//F1 Starts form the very first level 
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && !pause)
	{
		start_pos = true;
		App->fade->FadeToBlack(this, this, 1.3f);
		loading_lvl1 = true;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN && !pause)
	{
		//counter for animation speed
		App->caped_frames = !App->caped_frames;
	}

	//F2 Starts from the beginning of the current level
	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN && !pause)
	{
		start_pos = true;
		if (level_to_load->data->lvl == 2)
		{
			App->fade->FadeToBlack(this, this, 1.3f);
			loading_lvl2 = true;
		}
		else
		{
			App->fade->FadeToBlack(this, this, 1.3f);
			loading_lvl1 = true; 
		}
	}
	//F3 Starts the second level  
	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN && !pause)
	{
		start_pos = true;
		App->fade->FadeToBlack(this, this, 1.3f);
		loading_lvl2 = true; 
	}

	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN && !pause)
	{
		App->fade->FadeToBlack(this, this, 1.3f);
		loading_menu = true; 
	}

	//Load Tutorial 
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN && !pause)
	{
		if(actual_level == 0)
		{ 
			App->fade->FadeToBlack(this, this, 1.3f);
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
			if (pause)
				pause = false;
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
			if (pause)
				pause = false;
			loading_saved_game = false; 
		}
	}
	App->audio->masterVolume = Slider_Music->Value_percentage;
	App->audio->SetMusicVolume();
	App->map->Draw();

	if (actual_level == 3)
	{
		if (actual_level == 3 && App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT && !pause)
			current_keyA = &keyA_pressed;
		else
			current_keyA = &keyA;

		if (actual_level == 3 && App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT && !pause)
			current_keyD = &keyD_pressed;
		else
			current_keyD = &keyD;

		if (actual_level == 3 && App->input->GetKey(SDL_SCANCODE_L) == KEY_REPEAT && !pause)
			current_keyL = &keyL_pressed;
		else
			current_keyL = &keyL;

		if (actual_level == 3 && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && !pause)
			current_keySpace = &keySpace_pressed;
		else
			current_keySpace = &keySpace;

		App->render->Blit(keyboard, 300, 885, &current_keyA->GetCurrentFrame(dt_scene), SDL_FLIP_NONE);
		App->render->Blit(keyboard, 350, 885, &current_keyD->GetCurrentFrame(dt_scene), SDL_FLIP_NONE);
		App->render->Blit(keyboard, 2200, 900, &current_keyL->GetCurrentFrame(dt_scene), SDL_FLIP_NONE);
		App->render->Blit(keyboard, 3400, 650, &current_keyL->GetCurrentFrame(dt_scene), SDL_FLIP_NONE);
		App->render->Blit(keyboard, 1100, 885, &current_keySpace->GetCurrentFrame(dt_scene), SDL_FLIP_NONE);

	}
	for (p2List_item<Gui_Elements*>* iterator = App->gui->List_elem.start; iterator != nullptr; iterator = iterator->next)
	{
		if (iterator->data->in_game)
		{
			iterator->data->Draw_InGame_Element();
		}
	}


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

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && App->fade->IsFading() == false)
	{
		if (actual_level != 0)
		{
			if (Menu->visible)
			{
				pause = !pause;
				Menu->visible = !Menu->visible;
			}
			else if (!Menu->visible && !Settings_Menu->visible)
			{
				pause = !pause;
				Menu->visible = !Menu->visible;
			}
			else if (!Menu->visible && Settings_Menu->visible)
			{
				Menu->visible = !Menu->visible;
				Settings_Menu->visible = !Settings_Menu->visible;
			}
		}
		else
		{
			if (Settings_Menu->visible == true)
			{
				Settings_Menu->visible = !Settings_Menu->visible;
				Main_Menu->visible = !Main_Menu->visible;
				pause = !pause;
			}
		}
		
		
	}
	

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

	

	if (start_pos)
	{
		App->entityManager->player->initial_pos = true;
	}	
	else
	{
		if(level_to_load==0)
			App->entityManager->player->initial_pos = true;
		else
			App->entityManager->player->initial_pos = false;
	}
		

	if (actual_level == 0 && level_to_load->data->lvl > 0)
	{
		App->gui->Delete_UI_Elements();
		App->entityManager->player->CleanUp();
		App->map->Load(level_to_load->data->mapPath.GetString());
		RespawnEntities();
		App->entityManager->Start();
		App->entityManager->player->Start();
				
		Create_UI_Elements();

		actual_level = level_to_load->data->lvl;
	}
	else if (actual_level == level_to_load->data->lvl)
	{
		
		App->entityManager->player->CleanUp();
	
		if (level_to_load->data->lvl != 3)
			App->entityManager->DestroyEnemies();

		App->gui->Delete_UI_Elements(); 

		App->map->Load(level_to_load->data->mapPath.GetString());

		if (level_to_load->data->lvl != 3)
			RespawnEntities();

		Create_UI_Elements(); 

		App->entityManager->Start();
	}
	else if ((actual_level == 3 && level_to_load->data->lvl == 1) || (actual_level == 1 && level_to_load->data->lvl == 2) || (actual_level == 2 && level_to_load->data->lvl == 1) || (actual_level == 3 && level_to_load->data->lvl == 2))
	{

		App->gui->Delete_UI_Elements(); 
		
		App->entityManager->player->CleanUp();
		if (actual_level != 3)
		{
			App->entityManager->DestroyEnemies();
		}
		
		App->map->Load(level_to_load->data->mapPath.GetString());
		RespawnEntities();
		Create_UI_Elements(); 
		App->entityManager->Start();
		actual_level = level_to_load->data->lvl;
	}
	else if ((actual_level > 0) && (level_to_load->data->lvl == 0))
	{
		App->entityManager->DestroyEnemies();
		App->entityManager->player->CleanUp();
		App->gui->Delete_UI_Elements(); 
		App->map->Load(level_to_load->data->mapPath.GetString());
		App->render->camera.x = -50;
		App->render->camera.y = -300;
		Create_UI_Elements();
		App->entityManager->player->initial_pos = true;
		App->entityManager->player->Start();
		actual_level = level_to_load->data->lvl;
		/*App->audio->PlayMusic("audio/music/Galway.ogg");*/
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
		App->entityManager->CreateEntity(300, 950, HEART);
	}
	else if (level_to_load->data->lvl == 2)
	{
		App->entityManager->CreateEntity(630, 700, SMASHER);
		App->entityManager->CreateEntity(3620, 670, SMASHER);
		App->entityManager->CreateEntity(4272, 710, SMASHER);
		App->entityManager->CreateEntity(1875, 167, BAT);
		App->entityManager->CreateEntity(5291, 248, BAT);
		App->entityManager->CreateEntity(6052, 483, BAT);
	}
}

void j1Scene::Create_UI_Elements()
{
	if (level_to_load->data->lvl == 0)
	{

		Main_Menu = App->gui->Create_Image(Element_type::IMAGE, { 0, 0 }, {4000, 4000, (int)App->win->width, (int)App->win->height}, true, false, false, App->gui->GetAtlas(), nullptr);

		SDL_Rect title_rect = { 0, 0, 500, 300 };
		App->gui->Create_Image(Element_type::IMAGE, { 275, -20 }, title_rect, true, false, true, title, Main_Menu);

		Gui_Elements* Play = App->gui->Create_Button(Element_type::BUTTON, { 415, 300 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::PLAY, Main_Menu);
		Gui_Elements* Load_Game = App->gui->Create_Button(Element_type::BUTTON, { 415, 370 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::LOAD, Main_Menu);
		Gui_Elements* MainMenu_Settings = App->gui->Create_Button(Element_type::BUTTON, { 415, 440 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::SETTINGS, Main_Menu);
		Gui_Elements* Credits = App->gui->Create_Button(Element_type::BUTTON, { 415, 510 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::NONE, Main_Menu);
		Gui_Elements* Exit = App->gui->Create_Button(Element_type::BUTTON, { 415, 580 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::EXIT, Main_Menu);
		//Gui_Elements* GitHub = App->gui->Create_Button(Element_type::BUTTON, { 946, 690 }, { 19, 789 , 77, 77 }, { 19, 789 , 77, 77 }, { 19, 789 , 77, 77 }, true, false, App->gui->GetAtlas(), Function::GITHUB);

	
		App->gui->Create_Label(Element_type::LABEL, { 57, 6 }, { 0,0,77, 30 }, true, true, "PLAY", { 255,255,255,0 }, App->font->default, Play);
		App->gui->Create_Label(Element_type::LABEL, { 30, 6 }, { 0,0,120, 30 }, true, true, "CONTINUE", { 255,255,255,0 }, App->font->default, Load_Game);
		App->gui->Create_Label(Element_type::LABEL, { 30, 6 }, { 0,0,120, 30 }, true, true, "SETTINGS", { 255,255,255,0 }, App->font->default, MainMenu_Settings);
		App->gui->Create_Label(Element_type::LABEL, { 40, 6 }, { 0,0,100, 30 }, true, true, "CREDITS", { 255,255,255,0 }, App->font->default, Credits);
		App->gui->Create_Label(Element_type::LABEL, { 57, 6 }, { 0,0,70, 30 }, true, true, "EXIT", { 255,255,255,0 }, App->font->default, Exit);
	}
	else if (level_to_load->data->lvl == 3 || level_to_load->data->lvl == 1 || level_to_load->data->lvl == 2)
	{
		if (level_to_load->data->lvl == 3)
		{
			Skip = App->gui->Create_Button(Element_type::BUTTON, { 800, 700 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, true,  App->gui->GetAtlas(), Function::SKIP);
			Skip_Text = App->gui->Create_Label(Element_type::LABEL, { 3, 6 }, { 0,0,180, 30 }, false, true, "SKIP TUTORIAL", { 255,255,255,0 }, App->font->default, Skip);
		}
		if (level_to_load->data->lvl == 1 || level_to_load->data->lvl == 2)
		{
			if (App->entityManager->player->lifes == 5)
				App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 1551, 21, 143, 56 }, true, true, false, App->gui->GetAtlas(), nullptr);
			if (App->entityManager->player->lifes == 4)
				App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 1551, 81, 143, 56 }, true, true, false, App->gui->GetAtlas(), nullptr);
			if (App->entityManager->player->lifes == 3)
				App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 1551, 141, 143, 56 }, true, true, false, App->gui->GetAtlas(), nullptr);
			if (App->entityManager->player->lifes == 2)
				App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 1551, 202, 143, 56 }, true, true, false, App->gui->GetAtlas(), nullptr);
			if (App->entityManager->player->lifes == 1)
				App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 1551, 261, 143, 56 }, true, true, false, App->gui->GetAtlas(), nullptr);
		}
		
		Menu = App->gui->Create_Image(Element_type::IMAGE, { 355, 200 }, { 8, 459, 315, 402 }, false, false, false, App->gui->GetAtlas());
		
		Gui_Elements* Resume = App->gui->Create_Button(Element_type::BUTTON, { 62, 45 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, false, false, App->gui->GetAtlas(), Function::RESUME, Menu);
		App->gui->Create_Label(Element_type::LABEL, { 35, 6 }, { 0,0,110, 30 }, false, false, "RESUME", { 255,255,255,0 }, App->font->default, Resume);

		Gui_Elements* Load = App->gui->Create_Button(Element_type::BUTTON, { 62, 115 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, false, false, App->gui->GetAtlas(), Function::LOAD, Menu);
		App->gui->Create_Label(Element_type::LABEL, { 25, 6 }, { 0,0,140, 30 }, false, false, "LOAD GAME", { 255,255,255,0 }, App->font->default, Load);

		Gui_Elements* Save = App->gui->Create_Button(Element_type::BUTTON, { 62, 185 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, false, false, App->gui->GetAtlas(), Function::SAVE, Menu);
		App->gui->Create_Label(Element_type::LABEL, { 25, 6 }, { 0,0,140, 30 }, false, false, "SAVE GAME", { 255,255,255,0 }, App->font->default, Save);

		Gui_Elements* Settings = App->gui->Create_Button(Element_type::BUTTON, { 62, 255 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, false, false, App->gui->GetAtlas(), Function::SETTINGS, Menu);
		App->gui->Create_Label(Element_type::LABEL, { 30, 6 }, { 0,0,120, 30 }, false, false, "SETTINGS", { 255,255,255,0 }, App->font->default, Settings);

		Gui_Elements* Main_Menu = App->gui->Create_Button(Element_type::BUTTON, { 62, 325 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, false, false, App->gui->GetAtlas(), Function::MENU, Menu);
		App->gui->Create_Label(Element_type::LABEL, { 25, 6 }, { 0,0,140, 30 }, false, false, "MAIN MENU", { 255,255,255,0 }, App->font->default, Main_Menu);

		Gui_Elements* Esc = App->gui->Create_Button(Element_type::BUTTON, { 290, -10 }, { 460, 463 , 35, 38 }, { 422, 463 , 35, 38 }, { 384, 463 , 35, 38 }, false, false, App->gui->GetAtlas(), Function::RESUME, Menu);
		Gui_Elements* X = App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 355, 474, 15, 15 }, false, false, false, App->gui->GetAtlas(), Esc);


	}
	Settings_Menu = App->gui->Create_Image(Element_type::IMAGE, { 205, 160 }, { 6, 7, 611, 442 }, false, false, false, App->gui->GetAtlas());

	Gui_Elements* Esc = App->gui->Create_Button(Element_type::BUTTON, { 586, -10 }, { 460, 463 , 35, 38 }, { 422, 463 , 35, 38 }, { 384, 463 , 35, 38 }, false, false, App->gui->GetAtlas(), Function::RESUME, Settings_Menu);
	Gui_Elements* X = App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 355, 474, 15, 15 }, false, false, false, App->gui->GetAtlas(), Esc);

	Slider_Music = App->gui->Create_Slider(Element_type::SLIDER, { 350, 140 }, { 860,337,190,2 }, false, App->gui->GetAtlas(), Settings_Menu);
	Gui_Elements* SliderButton = App->gui->Create_Image(Element_type::IMAGE, iPoint{ 0, -20 }, { 770, 197, 28, 42 }, false, false, true, App->gui->GetAtlas(), Slider_Music);
	Gui_Elements* Music_Volume = App->gui->Create_Label(Element_type::LABEL, { 100, 120 }, { 0,0,200, 30 }, false, false, "Music Volume", { 100,40, 0,0 }, App->font->default, Settings_Menu);

}