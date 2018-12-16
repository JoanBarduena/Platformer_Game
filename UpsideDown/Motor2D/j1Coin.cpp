#include "j1App.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Coin.h"
#include "j1Scene.h"

j1Coin::j1Coin(int x, int y, EntityType Type) : j1Entity(x, y, Type)
{
	current_animation = NULL;

	idle.LoadAnimations("coin", "idle");
	position.x = x;
	position.y = y;

	type = Type;

	start_pos.x = x;
	start_pos.y = y;
}

void  j1Coin::LoadValues()
{

}

j1Coin::~j1Coin() {}

bool j1Coin::Start()
{
	if (graphics == nullptr)
	{
		graphics = App->tex->Load("textures/coin.png");
	}
	if (collider == nullptr)
	{
		collider = App->collision->AddCollider({ (int)position.x, (int)position.y, 28, 26 }, COLLIDER_COIN, App->entityManager);
	}

	coin_fx = App->audio->LoadFx("audio/fx/get_coin.wav");

	position.x = start_pos.x;
	position.y = start_pos.y;

	current_animation = &idle;
	return true;
}

bool j1Coin::Update(float dt, bool do_logic)
{
	BROFILER_CATEGORY("Bat Update", Profiler::Color::Green);

	dt_coin = dt;
	collider->SetPos(position.x , position.y );

	
	return true;
}

bool j1Coin::PostUpdate()
{
	Draw();
	return true;
}

bool j1Coin::CleanUp()
{
	App->tex->UnLoad(graphics);
	graphics = nullptr;
	if (collider != nullptr)
	{
		collider->to_delete = true;
		collider = nullptr;
	}
	
	return true;
}

void j1Coin::Draw()
{
	SDL_Rect rect = current_animation->GetCurrentFrame(dt_coin);
	App->render->Blit(graphics, (int)position.x, (int)position.y, &rect, SDL_FLIP_NONE);
}

void j1Coin::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_PLAYER)
	{
		App->audio->PlayFx(coin_fx); 
		App->entityManager->DestroyThisEntity(this);
		//App->gui->Delete_UI_Elements(); 
		//App->scene->Create_UI_Elements();
		App->entityManager->player->collected_coins++;
	}
}

bool j1Coin::Load(pugi::xml_node&)
{
	return true;
}
bool j1Coin::Save(pugi::xml_node&) const
{
	return true;
}
