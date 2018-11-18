#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"

j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
		pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

Direction j1PathFinding::SetDirection(const p2DynArray<iPoint>& path, char* type) const
{
	Direction ret = NONE;

	if (path.Count() >= 2)
	{
		iPoint actual_tile, next_tile;
		actual_tile = path[0];
		next_tile = path[1];

		int dx = next_tile.x - actual_tile.x;
		int dy = next_tile.y - actual_tile.y;

		if (type == "bat")
		{

			if (dx == 1 && dy == -1)
				ret = NORTH_EAST;
			else if (dx == -1 && dy == -1)
				ret = NORTH_WEST;
			else if (dx == 1 && dy == 1)
				ret = SOUTH_EAST;
			else if (dx == -1 && dy == 1)
				ret = SOUTH_WEST;
			else if (dy == -1)
				ret = NORTH;
			else if (dy == 1)
				ret = SOUTH;
			else if (dx == -1)
				ret = WEST;
			else if (dx == 1)
				ret = EAST;
		}
		else if (type == "smasher")
		{
			if (dy == -1)
				ret = NORTH;
			else if (dy == 1)
				ret = SOUTH;
			else if (dx == -1)
				ret = WEST;
			else if (dx == 1)
				ret = EAST;
		}
		
			
	}
	return ret;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	// north
	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	//north-east
	cell.create(pos.x + 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	//north-west
	cell.create(pos.x - 1, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south-east
	cell.create(pos.x + 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	// south-west
	cell.create(pos.x - 1, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.count();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	//h = pos.DistanceTo(destination);
	h = pos.DistanceNoSqrt(destination);
	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	// TODO 1: if origin or destination are not walkable, return -1
	if (IsWalkable(origin) == false || IsWalkable(destination) == false)
	{
		return -1;
	}

	// TODO 2: Create two lists: open, close
	// Add the origin tile to open
	// Iterate while we have tile in the open list
	PathList open;
	PathList closed;
	PathList adjacent;

	//Add origin to the open list:
	PathNode origin_node = { 0,0,origin,nullptr };
	open.list.add(origin_node);

	p2List_item<PathNode>* Path_Node;

	while (open.list.count() > 0)
	{
		// TODO 3: Move the lowest score cell from open list to the closed list
		Path_Node = open.GetNodeLowestScore();
		p2List_item<PathNode>* lowest_Path_Node = closed.list.add(Path_Node->data);
		open.list.del(Path_Node);

		// TODO 4: If we just added the destination, we are done!
		// Backtrack to create the final path
		
		// Use the Pathnode::parent and Flip() the path when you are finish
		if (lowest_Path_Node->data.pos == destination)
		{
			last_path.Clear();
			//Backtracking
			const PathNode* BackTrack_Node = &lowest_Path_Node->data;
			while (BackTrack_Node->pos != origin)
			{
				last_path.PushBack(BackTrack_Node->pos);
				BackTrack_Node = BackTrack_Node->parent;
			}
			last_path.Flip();
			//returning the number of steps
			return last_path.Count();
		}

		// TODO 5: Fill a list of all adjancent nodes
		adjacent.list.clear();
		lowest_Path_Node->data.FindWalkableAdjacents(adjacent);

		// TODO 6: Iterate adjancent nodes:

		for (p2List_item<PathNode>* item = adjacent.list.start; item; item = item->next)
		{
			// ignore nodes in the closed list
			if (closed.Find(item->data.pos) == NULL)
			{
				p2List_item <PathNode>* adjacent_open = open.Find(item->data.pos);
				// If it is NOT found, calculate its F and add it to the open list
				if (adjacent_open == NULL)
				{
					item->data.CalculateF(destination);
					if (abs(item->data.pos.x - item->data.parent->pos.x) == abs(item->data.pos.y - item->data.parent->pos.y))
					{
						item->data.g += 1;
						open.list.add(item->data);
						continue;
					}
					open.list.add(item->data);
				}
				// If it is already in the open list, check if it is a better path (compare G)
				// If it is a better path, Update the parent
				else if (adjacent_open->data.g > item->data.g + 30) // If it is a better path, Update the parent
				{
					adjacent_open->data.parent = item->data.parent;
					adjacent_open->data.CalculateF(destination);
				}
			}

		}

	}
}

