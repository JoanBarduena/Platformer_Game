#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#define MAX_FRAMES 100

class Animation
{
public:
	bool loop = true;
	float speed;
	SDL_Rect frames[MAX_FRAMES];

private:
	float current_frame = 0.0f;
	int last_frame = 0;
	int loops = 0;

	pugi::xml_document config_xml; 

public:

	Animation()
	{}

	Animation(const Animation& anim) : loop(anim.loop), speed(anim.speed), last_frame(anim.last_frame)
	{
		SDL_memcpy(&frames, anim.frames, sizeof(frames));
	}

	void PushBack(const SDL_Rect& rect)
	{
		frames[last_frame++] = rect;
	}

	SDL_Rect& GetCurrentFrame(float dt)
	{
		current_frame += speed*dt;
		if (current_frame >= last_frame)
		{
			current_frame = (loop) ? 0.0f : last_frame - 1;
			loops++;
		}

		return frames[(int)current_frame];
	}

	void LoadAnimations(p2SString name, p2SString animation)
	{
		pugi::xml_parse_result result = config_xml.load_file("animations.xml");
		if (result != NULL)
		{
			pugi::xml_node animation_name = config_xml.child("animations").child(name.GetString()).child(animation.GetString());
			speed = animation_name.attribute("speed").as_float();
			loop = animation_name.attribute("loop").as_bool();
			for (pugi::xml_node animation = animation_name.child("animation"); animation; animation = animation.next_sibling("animation"))
			{
				PushBack({ animation.attribute("x").as_int(), animation.attribute("y").as_int(), animation.attribute("width").as_int(), animation.attribute("height").as_int() });
			}
		}
	}


	bool Finished() const
	{
		return loops > 0;
	}

	void Reset()
	{
		loops = 0;
		current_frame = 0.0f;
	}
};

#endif
