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

	int terrian         = 0;               // ��������
	int decoration      = -1;							 // װ������
	int special_flag    = -1;              // ��������
	bool has_tower      = false;           // �Ƿ�Ϊ����
	Direction direction = Direction::None; // �о�����

};

typedef std::vector<Tile> TileArray;
typedef std::vector<TileArray> TileMap;

