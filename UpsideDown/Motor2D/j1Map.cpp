#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Map.h"
#include "j1Collision.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

// Destructor
j1Map::~j1Map()
{}

// Called before render is available
bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;

	folder.create(config.child("folder").child_value());

	return ret;
}

void j1Map::Draw()
{
	if (map_loaded == false)
		return;

	// Drawing all image_layers
	p2List_item<ImageLayer*>* image = nullptr;
	for (image = data.image_layers.start; image; image = image->next)
	{
		SDL_Texture* tex = image->data->texture;
		SDL_Rect rect = { 0,0,image->data->width, image->data->height };
		if (image->data->position.x < -image->data->width)
		{
			image->data->position.x = image->data->width;
		}
		App->render->Blit(tex, image->data->position.x, image->data->position.y, &rect);
	}

	p2List_item<TileSet*>* item = nullptr;
	item = data.tilesets.start;

	p2List_item<MapLayer*>* layer = nullptr;
	layer = data.map_layers.start;

	// TODO 5: Prepare the loop to draw all tilesets + Blit
	for (uint i = 0; i < data.height; ++i)
	{
		for (uint j = 0; j < data.width; ++j)
		{
			uint id = layer->data->Get(j,i);
			id = layer->data->data[id];

			if (id != 0) {
				SDL_Rect *rect = &item->data->GetTileRect(id);
				iPoint pos = MapToWorld(j, i);
				App->render->Blit(item->data->texture, pos.x, pos.y, rect);
			}
		}


		// TODO 9: Complete the draw function

	}
	
}


iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x * data.tile_width;
	ret.y = y * data.tile_height;

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

// Called before quitting
bool j1Map::CleanUp()
{
	LOG("Unloading map");

	// Remove all tilesets
	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	// TODO 2: clean up all layer data
	// Remove all layers

	p2List_item<MapLayer*>* item_layer;
	item_layer = data.map_layers.start;

	while (item_layer != NULL)
	{
		RELEASE(item_layer->data);
		item_layer = item_layer->next;
	}
	data.map_layers.clear();

	p2List_item<ImageLayer*>* img_layer;
	img_layer = data.image_layers.start;

	while (img_layer != NULL)
	{
		RELEASE(img_layer->data);
		img_layer = img_layer->next;
	}
	data.image_layers.clear();

	// Clean up the pugui tree
	map_file.reset();

	return true;
}

// Load new map
bool j1Map::Load(const char* file_name)
{
	bool ret = true;

	CleanUp();
	App->collision->CleanUp();

	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	// Load general info ----------------------------------------------
	if(ret == true)
	{
		ret = LoadMap();
	}

	// Load all tilesets info ----------------------------------------------
	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	// TODO 4: Iterate all layers and load each of them
	// Load layer info ----------------------------------------------
	pugi::xml_node layer_node;
	for (layer_node = map_file.child("map").child("layer"); layer_node && ret; layer_node = layer_node.next_sibling("layer"))
	{
		MapLayer* layer = new MapLayer();

		ret = LoadLayer(layer_node, layer);

		if (ret == true)
		{
			data.map_layers.add(layer);
		}
			
	}

	//Load ImageLayer info -------------------------------------------
	pugi::xml_node img_layer_node;
	for (img_layer_node = map_file.child("map").child("imagelayer"); img_layer_node && ret; img_layer_node = img_layer_node.next_sibling("imagelayer"))
	{
		ImageLayer* img_layer = new ImageLayer();

		if (ret == true)
		{
			ret = LoadLayerImage(img_layer_node, img_layer);
		}
		data.image_layers.add(img_layer);
	}

	//Load Collisions --------------------------------------------------
	pugi::xml_node collider;
	for (collider = map_file.child("map").child("objectgroup"); collider && ret; collider = collider.next_sibling("objectgroup"))
	{
			LoadColliders(collider);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		// TODO 4: Add info here about your loaded layers
		// Adapt this vcode with your own variables
		
		p2List_item<MapLayer*>* item_layer = data.map_layers.start;
		while(item_layer != NULL)
		{
			MapLayer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

// Load map general properties
bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();
	pugi::xml_node offset = tileset_node.child("tileoffset");

	if(offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

// TODO 3: Create the definition for a function that loads a single layer
bool j1Map::LoadLayer(pugi::xml_node& node, MapLayer* layer)
{
	bool ret = true;

	layer->name = node.attribute("name").as_string();
	layer->width = node.attribute("width").as_uint();
	layer->height = node.attribute("height").as_uint();
	pugi::xml_node layer_data = node.child("data");

	if (layer_data == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'layer/data' tag.");
		ret = false;
		RELEASE(layer);
	}
	else
	{
		layer->data = new uint[(layer->width)*(layer->height)];
		memset(layer->data, 0, sizeof(uint)*(layer->width)*(layer->height));
	}


	int i = 0;

	for (pugi::xml_node tile = layer_data.child("tile"); tile; tile = tile.next_sibling("tile"))
	{
		layer->data[i++] = tile.attribute("gid").as_int(0);
	}


	return ret;

}

bool j1Map::LoadLayerImage(pugi::xml_node& node, ImageLayer* img)
{
	bool ret = true;

	img->name = node.attribute("name").as_string();
	img->offset_x = node.attribute("offsetx").as_int();
	img->offset_y = node.attribute("offsety").as_int();

	img->position.x = img->offset_x;
	img->position.y = img->offset_y;

	pugi::xml_node image = node.child("image");

	if (image == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'imagelayer/image' tag.");
		ret = false;
		RELEASE(img);
	}
	else
	{
		img->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		img->width = image.attribute("width").as_int();
		img->height = image.attribute("height").as_int();

		pugi::xml_node property;
		for (property = node.child("properties").child("property"); property; property = property.next_sibling("property"))
		{
			p2SString name = property.attribute("name").as_string();
			if (name == "speed")
			{
				img->speed = property.attribute("value").as_float();
			}
		}
	}

	return ret;
}

bool j1Map::LoadColliders(pugi::xml_node& node)
{

	bool ret = true;

	pugi::xml_node obj = node.child("object");

	if (obj == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'objectgroup/object' tag.");
		ret = false;
	}

	COLLIDER_TYPE col_type;
	p2SString type;

	for (obj = node.child("object"); obj; obj = obj.next_sibling("object"))
	{
		type = obj.attribute("type").as_string();

		if (type == "floor_jumpable")
		{
			col_type = COLLIDER_FLOOR_JUMPABLE;
		}
		else if (type == "floor")
		{
			col_type = COLLIDER_FLOOR;
		}
		else if (type == "limit")
		{
			col_type = COLLIDER_LIMIT; 
		}
		else if (type == "win1")
		{
			col_type = COLLIDER_WIN1;
		}
		else if (type == "win2")
		{
			col_type = COLLIDER_WIN2;
		}
		SDL_Rect rect;
		rect.x = obj.attribute("x").as_int();
		rect.y = obj.attribute("y").as_int();
		rect.h = obj.attribute("height").as_int();
		rect.w = obj.attribute("width").as_int();

		App->collision->AddCollider(rect, col_type);
		LOG("Collider added with pos %d , %d", rect.x, rect.y);
	}

	return ret;
}


