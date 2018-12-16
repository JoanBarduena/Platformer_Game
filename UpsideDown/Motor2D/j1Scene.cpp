#include "p2Defs.h"
#include "p2Log.h"
#include "string"
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

	Create_UI_Elements();
	App->audio->musicVolume = Slider_Music->Value_percentage;
	App->audio->sfxVolume = Slider_FX->Value_percentage;
	App->audio->SetMusicVolume();
	App->audio->SetSfxVolume();
	App->audio->PlayMusic("audio/music/main_menu.ogg");

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
				App->entityManager->player->lifes = 3;
				App->entityManager->player->player_score = 0; 
				if (actual_level == 0)
				{
					App->fade->FadeToBlack(this, this, 1.3f);
					App->entityManager->player->saved_timer = 0;
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
			else if (iterator->data->funct == Function::CREDITS)
			{
				pause = !pause;
				Main_Menu->visible = !Main_Menu->visible;
				Credits_Menu->visible = !Credits_Menu->visible;
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
							App->entityManager->player->paused_timer = 0; 
							App->fade->FadeToBlack(this, this, 1.3f);
							loading_menu = true;
						}
					}
					iterator->data->do_action = false;
				}
			}
			else if (iterator->data == Settings_Menu)
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
						else if (iterator->data->funct == Function::APPLY)
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
							Music_Slider_pos = (Slider_Music->Rect.w * Slider_Music->Value_percentage)/100;
							FX_Slider_pos = (Slider_FX->Rect.w * Slider_FX->Value_percentage) / 100;
							App->audio->musicVolume = Slider_Music->Value_percentage;
							App->audio->sfxVolume = Slider_FX->Value_percentage;
							App->audio->SetMusicVolume();
							App->audio->SetSfxVolume();
						}
					}
					
				}
			
			}
			else if (iterator->data == Credits_Menu)
			{
				for (p2List_item<Gui_Elements*>* iterator = Credits_Menu->childrens.start; iterator != nullptr; iterator = iterator->next)
				{
					if (iterator->data->type == Element_type::BUTTON && iterator->data->do_action == true && App->fade->IsFadingOut() == false)
					{
						if (iterator->data->funct == Function::RESUME)
						{
							pause = !pause;
							Credits_Menu->visible = !Credits_Menu->visible;
							Main_Menu->visible = !Main_Menu->visible;
						}
						else if (iterator->data->funct == Function::GITHUB_JOAN)
						{
							ShellExecuteA(NULL, "open", "https://github.com/JoanBarduena", NULL, NULL, SW_SHOWNORMAL);
						}
						else if (iterator->data->funct == Function::GITHUB_JOSEP)
						{
							ShellExecuteA(NULL, "open", "https://github.com/JosepLleal", NULL, NULL, SW_SHOWNORMAL);
						}
					}
				}
			}
		}
	}
	// -------------------------------------------------------------------------------------------

	Slider_Music_Value->ValuetoString(Slider_Music->Value_percentage, Slider_Music_Value->text);
	Slider_FX_Value->ValuetoString(Slider_FX->Value_percentage, Slider_FX_Value->text);

	if(Coins != nullptr)
		Coins->ValuetoString(App->entityManager->player->collected_coins, Coins->text);

	if (Timer != nullptr)
		Timer->SetHour(App->entityManager->player->timer); 

	if (Score_num != nullptr)
		Score_num->ValuetoString(App->entityManager->player->player_score, Score_num->text);

	//F1 Starts form the very first level 
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN && !pause)
	{
		start_pos = true;
		App->fade->FadeToBlack(this, this, 1.3f);
		loading_lvl1 = true;
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
	//F4 Go to main menu
	if (App->input->GetKey(SDL_SCANCODE_F4) == KEY_DOWN && !pause)
	{
		App->fade->FadeToBlack(this, this, 1.3f);
		loading_menu = true;
	}
	//F5 SAVE
	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN && !pause && actual_level != 0 && actual_level != 3)
	{
		start_pos = false;
		App->SaveGame();
	}
	//F6 LOAD
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN && !pause)
	{
		start_pos = false;
		App->fade->FadeToBlack(this, this, 1.0f);
		loading_saved_game = true;

	}
	
	//Load Tutorial 
	if (App->input->GetKey(SDL_SCANCODE_F7) == KEY_DOWN && !pause)
	{
		if (actual_level == 0)
		{
			App->fade->FadeToBlack(this, this, 1.3f);
			loading_tutorial = true;
		}
	}

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN && !pause)
	{
		debug_UI = !debug_UI;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN && !pause)
	{
		App->caped_frames = !App->caped_frames;
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

	if (actual_level == 0)
	{
		App->entityManager->player->position.x = 150;
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
			if (Settings_Menu->visible)
			{
				Settings_Menu->visible = !Settings_Menu->visible;
				Main_Menu->visible = !Main_Menu->visible;
				pause = !pause;
			}
			else if (Credits_Menu->visible)
			{
				Credits_Menu->visible = !Credits_Menu->visible;
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
		if (level_to_load == 0)
			App->entityManager->player->initial_pos = true;
		else
			App->entityManager->player->initial_pos = false;
	}


	if (actual_level == 0 && level_to_load->data->lvl > 0)
	{
		App->audio->PlayMusic("audio/music/Galway.ogg");
		App->gui->Delete_UI_Elements();
		App->entityManager->player->CleanUp();
		App->map->Load(level_to_load->data->mapPath.GetString());
		RespawnEnemies();
		RespawnCoinsHearts();
		App->entityManager->Start();
		App->entityManager->player->Start();

		Create_UI_Elements();

		from_menu = true; 

		actual_level = level_to_load->data->lvl;
	}
	else if (actual_level == level_to_load->data->lvl)
	{
		App->entityManager->player->CleanUp();

		if (level_to_load->data->lvl != 3)
			App->entityManager->DestroyEnemies();

		App->entityManager->CleanUp();

		App->gui->Delete_UI_Elements();

		App->map->Load(level_to_load->data->mapPath.GetString());

		if (level_to_load->data->lvl != 3)
			RespawnEnemies();

		Create_UI_Elements();

		from_menu = false; 

		App->entityManager->Start();
	}
	else if ((actual_level == 3 && level_to_load->data->lvl == 1) || (actual_level == 1 && level_to_load->data->lvl == 2) || (actual_level == 2 && level_to_load->data->lvl == 1) || (actual_level == 3 && level_to_load->data->lvl == 2))
	{
		App->gui->Delete_UI_Elements();

		App->entityManager->player->CleanUp();
		if (actual_level != 3)
		{
			App->entityManager->DestroyAllEntities();
		}

		App->map->Load(level_to_load->data->mapPath.GetString());
		RespawnEnemies();
		RespawnCoinsHearts();
		Create_UI_Elements();
		App->entityManager->Start();
		from_menu = false;
		actual_level = level_to_load->data->lvl;
	}
	else if ((actual_level > 0) && (level_to_load->data->lvl == 0))
	{
		App->audio->PlayMusic("audio/music/main_menu.ogg");
		App->entityManager->DestroyAllEntities();
		Coins = nullptr; 
		Timer = nullptr; 
		Score_num = nullptr;
		App->entityManager->player->CleanUp();
		App->gui->Delete_UI_Elements();
		App->map->Load(level_to_load->data->mapPath.GetString());
		App->render->camera.x = -50;
		App->render->camera.y = -300;
		Create_UI_Elements();
		App->entityManager->player->initial_pos = true;
		App->entityManager->player->Start();
		App->entityManager->player->collected_coins = 0; 
		from_menu = false;
		actual_level = level_to_load->data->lvl;
		/*App->audio->PlayMusic("audio/music/Galway.ogg");*/
	}

	App->entityManager->player->apply_score = true;

	int w, h;
	uchar* data = NULL;
	if (App->map->CreateWalkabilityMap(w, h, &data))
	{
		App->pathfinding->SetMap(w, h, data);
	}
	RELEASE_ARRAY(data);
}


void j1Scene::RespawnEnemies()
{
	if (level_to_load->data->lvl == 1)
	{
		App->entityManager->CreateEntity(2060, 880, SMASHER);
		App->entityManager->CreateEntity(4125, 950, SMASHER);
		App->entityManager->CreateEntity(5571, 790, SMASHER);
		App->entityManager->CreateEntity(5874, 116, BAT);
		App->entityManager->CreateEntity(2430, 537, BAT);
		App->entityManager->CreateEntity(3770, 450, BAT);
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

void j1Scene::RespawnCoinsHearts()
{
	if (level_to_load->data->lvl == 1)
	{
		App->entityManager->CreateEntity(2215, 940, COIN);
		App->entityManager->CreateEntity(2589, 815, COIN);
		App->entityManager->CreateEntity(3378, 561, COIN);
		App->entityManager->CreateEntity(3532, 561, COIN);
		App->entityManager->CreateEntity(4265, 1052, COIN);
		App->entityManager->CreateEntity(5055, 889, COIN);
		App->entityManager->CreateEntity(5695, 585, COIN);
		App->entityManager->CreateEntity(5807, 585, COIN);
		App->entityManager->CreateEntity(5352, 107, COIN);
		App->entityManager->CreateEntity(800, 950, COIN);

		App->entityManager->CreateEntity(4386, 144, HEART);
	}
	else if (level_to_load->data->lvl == 2)
	{
		App->entityManager->CreateEntity(2077, 103, COIN);
		App->entityManager->CreateEntity(1738, 409, COIN);
		App->entityManager->CreateEntity(959, 222, COIN);
		App->entityManager->CreateEntity(4378, 183, COIN);
		App->entityManager->CreateEntity(3722, 354, COIN);
		App->entityManager->CreateEntity(3861, 500, COIN);
		App->entityManager->CreateEntity(3861, 690, COIN);
		App->entityManager->CreateEntity(3861, 780, COIN);
		App->entityManager->CreateEntity(5702, 776, COIN);
		App->entityManager->CreateEntity(2570, 924, COIN);

		App->entityManager->CreateEntity(3640, 782, HEART);
	}
}

void j1Scene::Create_UI_Elements()
{
	if (level_to_load->data->lvl == 0)
	{

		Main_Menu = App->gui->Create_Image(Element_type::IMAGE, { 0, 0 }, {4000, 4000, (int)App->win->width, (int)App->win->height}, true, false, false, App->gui->GetAtlas(), nullptr);

		SDL_Rect title_rect = { 0, 0, 500, 300 };
		App->gui->Create_Image(Element_type::IMAGE, { 275, -20 }, title_rect, true, false, false, title, Main_Menu);

		Gui_Elements* Play = App->gui->Create_Button(Element_type::BUTTON, { 415, 300 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::PLAY, Main_Menu);
		Gui_Elements* Load_Game = App->gui->Create_Button(Element_type::BUTTON, { 415, 370 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::LOAD, Main_Menu);
		Gui_Elements* MainMenu_Settings = App->gui->Create_Button(Element_type::BUTTON, { 415, 440 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::SETTINGS, Main_Menu);
		Gui_Elements* Credits = App->gui->Create_Button(Element_type::BUTTON, { 415, 510 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::CREDITS, Main_Menu);
		Gui_Elements* Exit = App->gui->Create_Button(Element_type::BUTTON, { 415, 580 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, false, App->gui->GetAtlas(), Function::EXIT, Main_Menu);
		//Gui_Elements* GitHub = App->gui->Create_Button(Element_type::BUTTON, { 946, 690 }, { 19, 789 , 77, 77 }, { 19, 789 , 77, 77 }, { 19, 789 , 77, 77 }, true, false, App->gui->GetAtlas(), Function::GITHUB);

	
		App->gui->Create_Label(Element_type::LABEL, { 30, 6 }, true, true, "NEW GAME", { 255,255,255,0 }, App->font->default, Play);
		App->gui->Create_Label(Element_type::LABEL, { 30, 6 }, true, true, "CONTINUE", { 255,255,255,0 }, App->font->default, Load_Game);
		App->gui->Create_Label(Element_type::LABEL, { 30, 6 }, true, true, "SETTINGS", { 255,255,255,0 }, App->font->default, MainMenu_Settings);
		App->gui->Create_Label(Element_type::LABEL, { 40, 6 }, true, true, "CREDITS", { 255,255,255,0 }, App->font->default, Credits);
		App->gui->Create_Label(Element_type::LABEL, { 57, 6 }, true, true, "EXIT", { 255,255,255,0 }, App->font->default, Exit);

		Credits_Menu = App->gui->Create_Image(Element_type::IMAGE, { 120, 80 }, { 621, 377, 785, 568 }, false, false, false, App->gui->GetAtlas());

		Gui_Elements* Esc = App->gui->Create_Button(Element_type::BUTTON, { 765, -10 }, { 460, 463 , 35, 38 }, { 422, 463 , 35, 38 }, { 384, 463 , 35, 38 }, false, false, App->gui->GetAtlas(), Function::RESUME, Credits_Menu);
		Gui_Elements* X = App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 355, 474, 15, 15 }, false, false, false, App->gui->GetAtlas(), Esc);

		App->gui->Create_Label(Element_type::LABEL, { 37, 40 }, true, true, "MIT License", { 100,40, 0 ,0 }, App->font->default, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 90 }, true, true, "Copyright (c) 2018 Josep Lleal and Joan Barduena", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 110 }, true, true, "Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 130 }, true, true, "files (the 'Software'), to deal in the Software without restriction, including without limitation the rights to use, copy,", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 150 }, true, true, "modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the ", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 170 }, true, true, "Software is furnished to do so, subject to the following conditions:", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 200 }, true, true, "The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 230 }, true, true, "THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT ", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 250 }, true, true, "LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 270 }, true, true, "NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, ", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 290 }, true, true, "WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE ", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 37, 310 }, true, true, "SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.", { 100,40, 0 ,0 }, App->font->Credits, Credits_Menu);

		App->gui->Create_Label(Element_type::LABEL, { 37, 350 }, true, true, "The Team:", { 100,40, 0 ,0 }, App->font->default, Credits_Menu);

		App->gui->Create_Button(Element_type::BUTTON, { 100, 450 }, { 650, 117 , 45, 49 }, { 699, 117 , 45, 49 }, { 748, 113 , 45, 53 }, false, false, App->gui->GetAtlas(), Function::GITHUB_JOAN, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 170, 460 }, true, true, "Joan Barduena", { 100,40, 0 ,0 }, App->font->default, Credits_Menu);
		App->gui->Create_Button(Element_type::BUTTON, { 430, 450 }, { 650, 117 , 45, 49 }, { 699, 117 , 45, 49 }, { 748, 113 , 45, 53 }, false, false, App->gui->GetAtlas(), Function::GITHUB_JOSEP, Credits_Menu);
		App->gui->Create_Label(Element_type::LABEL, { 500, 460 }, true, true, "Josep Lleal", { 100,40, 0 ,0 }, App->font->default, Credits_Menu);

	}
	else if (level_to_load->data->lvl == 3 || level_to_load->data->lvl == 1 || level_to_load->data->lvl == 2)
	{
		if (level_to_load->data->lvl == 3)
		{
			Skip = App->gui->Create_Button(Element_type::BUTTON, { 800, 700 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, true, true,  App->gui->GetAtlas(), Function::SKIP);
			Skip_Text = App->gui->Create_Label(Element_type::LABEL, { 3, 6 }, false, true, "SKIP TUTORIAL", { 255,255,255,0 }, App->font->default, Skip);
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

			Gui_Elements* Coin = App->gui->Create_Image(Element_type::IMAGE, {870, 18}, {648, 33, 56, 52}, true, true, false, App->gui->GetAtlas(), nullptr); 
			Coins = App->gui->Create_Label(Element_type::LABEL, { 70, -18 }, true, true, "0", { 255, 255, 255, 0 }, App->font->bigger, Coin);

			Timer = App->gui->Create_Label(Element_type::LABEL, { 360, 0 }, true, true, "0", { 255, 255, 255, 0 }, App->font->bigger, nullptr);
			
			Gui_Elements* Score = App->gui->Create_Label(Element_type::LABEL, { 820, 720 }, true, true, "SCORE:", { 255, 255, 255, 0 }, App->font->default, nullptr);
			Score_num = App->gui->Create_Label(Element_type::LABEL, {100, 0 }, true, true, "0", { 255, 255, 255, 0 }, App->font->default, Score);
		}
		
		Menu = App->gui->Create_Image(Element_type::IMAGE, { 355, 200 }, { 8, 459, 315, 402 }, false, false, false, App->gui->GetAtlas());
		
		Gui_Elements* Resume = App->gui->Create_Button(Element_type::BUTTON, { 62, 45 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, false, false, App->gui->GetAtlas(), Function::RESUME, Menu);
		App->gui->Create_Label(Element_type::LABEL, { 35, 6 }, false, false, "RESUME", { 255,255,255,0 }, App->font->default, Resume);

		Gui_Elements* Load = App->gui->Create_Button(Element_type::BUTTON, { 62, 115 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, false, false, App->gui->GetAtlas(), Function::LOAD, Menu);
		App->gui->Create_Label(Element_type::LABEL, { 25, 6 }, false, false, "LOAD GAME", { 255,255,255,0 }, App->font->default, Load);

		Gui_Elements* Save = App->gui->Create_Button(Element_type::BUTTON, { 62, 185 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, false, false, App->gui->GetAtlas(), Function::SAVE, Menu);
		App->gui->Create_Label(Element_type::LABEL, { 25, 6 }, false, false, "SAVE GAME", { 255,255,255,0 }, App->font->default, Save);

		Gui_Elements* Settings = App->gui->Create_Button(Element_type::BUTTON, { 62, 255 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, false, false, App->gui->GetAtlas(), Function::SETTINGS, Menu);
		App->gui->Create_Label(Element_type::LABEL, { 30, 6 }, false, false, "SETTINGS", { 255,255,255,0 }, App->font->default, Settings);

		Gui_Elements* Main_Menu = App->gui->Create_Button(Element_type::BUTTON, { 62, 325 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, false, false, App->gui->GetAtlas(), Function::MENU, Menu);
		App->gui->Create_Label(Element_type::LABEL, { 25, 6 }, false, false, "MAIN MENU", { 255,255,255,0 }, App->font->default, Main_Menu);

		Gui_Elements* Esc = App->gui->Create_Button(Element_type::BUTTON, { 290, -10 }, { 460, 463 , 35, 38 }, { 422, 463 , 35, 38 }, { 384, 463 , 35, 38 }, false, false, App->gui->GetAtlas(), Function::RESUME, Menu);
		Gui_Elements* X = App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 355, 474, 15, 15 }, false, false, false, App->gui->GetAtlas(), Esc);


	}
	Settings_Menu = App->gui->Create_Image(Element_type::IMAGE, { 205, 160 }, { 6, 7, 611, 442 }, false, false, false, App->gui->GetAtlas());

	Gui_Elements* Esc = App->gui->Create_Button(Element_type::BUTTON, { 586, -10 }, { 460, 463 , 35, 38 }, { 422, 463 , 35, 38 }, { 384, 463 , 35, 38 }, false, false, App->gui->GetAtlas(), Function::RESUME, Settings_Menu);
	Gui_Elements* X = App->gui->Create_Image(Element_type::IMAGE, { 10, 10 }, { 355, 474, 15, 15 }, false, false, false, App->gui->GetAtlas(), Esc);

	Slider_Music = App->gui->Create_Slider(Element_type::SLIDER, { 350, 140 }, { 860,337,190,2 }, false, App->gui->GetAtlas(), Settings_Menu);
	Button_Music = App->gui->Create_Image(Element_type::IMAGE, { Music_Slider_pos, -20 }, { 770, 197, 28, 42 }, false, false, true, App->gui->GetAtlas(), Slider_Music);
	Gui_Elements* Music_Volume = App->gui->Create_Label(Element_type::LABEL, { 100, 120 }, false, false, "Music Volume", { 100,40, 0,0 }, App->font->default, Settings_Menu);
	Slider_Music_Value = App->gui->Create_Label(Element_type::LABEL, { 490, 80 }, false, false, "0%%", { 100,40, 0,0 }, App->font->smaller, Settings_Menu);


	Slider_FX = App->gui->Create_Slider(Element_type::SLIDER, { 350, 230 }, { 860,337,190,2 }, false, App->gui->GetAtlas(), Settings_Menu);
	Button_FX = App->gui->Create_Image(Element_type::IMAGE, { FX_Slider_pos, -20 }, { 770, 197, 28, 42 }, false, false, true, App->gui->GetAtlas(), Slider_FX);
	Gui_Elements* FX_Volume = App->gui->Create_Label(Element_type::LABEL, { 100, 210 }, false, false, "FX Volume", { 100,40, 0,0 }, App->font->default, Settings_Menu);
	Slider_FX_Value = App->gui->Create_Label(Element_type::LABEL, { 490, 170 }, false, false, "0%%", { 100,40, 0,0 }, App->font->smaller, Settings_Menu);

	Gui_Elements* Apply = App->gui->Create_Button(Element_type::BUTTON, { 200, 380 }, { 1070, 260 , 190, 49 }, { 650, 260, 190, 49 }, { 860, 260, 190, 49 }, false, false, App->gui->GetAtlas(), Function::APPLY, Settings_Menu);
	App->gui->Create_Label(Element_type::LABEL, { 50, 6 }, false, false, "APPLY", { 255,255,255,0 }, App->font->default, Apply);

	
}