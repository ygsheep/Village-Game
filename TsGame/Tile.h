#pragma once
#include <vector>

const int SIZE_TILE = 48; 

struct Tile
{
	enum class Direction
	{
		None = 0,
		Up,
		Down,
		Left,
		Right
	};

	int terrian         = 0;               // 地形索引
	int decoration      = -1;							 // 装饰索引
	int special_flag    = -1;              // 建筑索引
	bool has_tower      = false;           // 是否为建筑
	Direction direction = Direction::None; // 行径方向

};

typedef std::vector<Tile> TileArray;
typedef std::vector<TileArray> TileMap;

