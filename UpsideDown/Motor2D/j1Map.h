#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1App.h"
#include "j1Textures.h"

// TODO 1: Create a struct for the map layer



struct Properties
{
	struct Property
	{
		p2SString name;
		int value;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}
		list.clear();
	}

	int Get(const char* name) const;
	p2List<Property*>	list;
};

struct MapLayer
{

	p2SString			name;
	uint				width;
	uint				height;
	uint*               data;
	Properties			properties;
	float				parallaxSpeed;

	~MapLayer()
	{
		RELEASE(data);
	}


	//TODO 6 HERE
	inline uint Get(uint x, uint y) const
	{
		return x + width * y;
	}

};

struct ImageLayer
{
	p2SString		name;
	SDL_Texture*	texture = nullptr;
	int				offset_x;
	int				offset_y;
	int				width;
	int				height;
	fPoint			position;
	float			speed = 0;

	~ImageLayer()
	{
		if (texture != nullptr)
		{
			App->tex->UnLoad(texture);
			texture = nullptr;
		}
	}
	
};

struct TileSet
{
	// TODO 7: Create a method that receives a tile id and returns it's Rectfind the Rect associated with a specific tile id
	SDL_Rect GetTileRect(int id) const;

	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	SDL_Texture*		texture = nullptr;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;

	~TileSet()
	{
		if (texture != nullptr)
		{
			App->tex->UnLoad(texture);
			texture = nullptr;
		}
	}
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};
// ----------------------------------------------------
struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<ImageLayer*> image_layers;
	p2List<MapLayer*>   map_layers;
};

// ----------------------------------------------------
class j1Map : public j1Module
{
public:

	j1Map();

	// Destructor
	virtual ~j1Map();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	void Draw();

	// Called before quitting
	bool CleanUp();

	// Load new map
	bool Load(const char* path);

	// TODO 8: Create a method that translates x,y coordinates from map positions to world positions
	iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;

	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer) const;

	


private:

	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, MapLayer* layer);
	bool LoadLayerImage(pugi::xml_node& node, ImageLayer* img);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);
	bool LoadColliders(pugi::xml_node& node);

	TileSet* GetTilesetFromTileId(int id) const;

public:

	MapData data;

private:

	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
};

#endif // __j1MAP_H__