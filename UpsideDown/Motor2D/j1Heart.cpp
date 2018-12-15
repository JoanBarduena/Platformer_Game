#include "j1App.h"
#include "j1Collision.h"
#include "j1Player.h"
#include "j1Scene.h"
#include "j1Map.h"
#include "j1Heart.h"
#include "j1Scene.h"

j1Heart::j1Heart(int x, int y, EntityType Type) : j1Entity(x, y, Type)
{
	current_animation = NULL;

	idle.LoadAnimations("heart", "idle");
	position.x = x;
	position.y = y;

	type = Type;

	start_pos.x = x;
	start_pos.y = y;
}

void  j1Heart::LoadValues()
{

}

j1Heart::~j1Heart() {}

bool j1Heart::Start()
{
	if (graphics == nullptr)
	{
		graphics = App->tex->Load("textures/hearts.png");
	}
	if (collider == nullptr)
	{
		collider = App->collision->AddCollider({ (int)position.x, (int)position.y, 28, 26 }, COLLIDER_COIN, App->entityManager);
	}

	heart_fx = App->audio->LoadFx("audio/fx/heal.wav");

	position.x = start_pos.x;
	position.y = start_pos.y;

	current_animation = &idle;
	return true;
}

bool j1Heart::Update(float dt, bool do_logic)
{
	BROFILER_CATEGORY("Bat Update", Profiler::Color::Green);

	dt_heart = dt;
	collider->SetPos(position.x, position.y);

	return true;
}

bool j1Heart::PostUpdate()
{
	Draw();
	return true;
}

bool j1Heart::CleanUp()
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

void j1Heart::Draw()
{
	SDL_Rect rect = current_animation->GetCurrentFrame(dt_heart);
	App->render->Blit(graphics, (int)position.x, (int)position.y, &rect, SDL_FLIP_NONE);
}

void j1Heart::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_PLAYER)
	{
		App->entityManager->player->is_healing = true; 

		if (App->entityManager->player->lifes < 5)
			App->entityManager->player->lifes++; 

		App->audio->PlayFx(heart_fx);
		App->entityManager->DestroyThisEntity(this);
		App->gui->Delete_UI_Elements();
		App->scene->Create_UI_Elements();
	}
}

bool j1Heart::Load(pugi::xml_node&)
{
	return true;
}
bool j1Heart::Save(pugi::xml_node&) const
{
	return true;
}
