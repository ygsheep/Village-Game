#pragma once
#include "Tile.h"
#include <vector>
#include <SDL.h>

class Route
{
public:
	typedef std::vector<SDL_Point> PosList;
	
	Route() = default;
	Route(const TileMap& tilemap, SDL_Point& idx_pos) {
		const size_t width_tile = tilemap[0].size();
		const size_t height_tile = tilemap.size();
		SDL_Point next_idx_pos = idx_pos;

		while (true)
		{
			// 越界检查
			if (next_idx_pos.x >= width_tile || next_idx_pos.y >= height_tile)
				break;

			// 重复检查
			if (check_repetition_idx(next_idx_pos))
				break;
			else
				idx_list.push_back(next_idx_pos);
		
			const Tile& tile = tilemap[next_idx_pos.y][next_idx_pos.x];
			bool is_next_direction_exit = false;
			switch (tile.direction)
			{
			case Tile::Direction::Up:
				next_idx_pos.y--;
				break;
			case Tile::Direction::Down:
				next_idx_pos.y++;
				break;
			case Tile::Direction::Left:
				next_idx_pos.x--;
				break;
			case Tile::Direction::Right:
				next_idx_pos.x++;
				break;
			default:
				is_next_direction_exit = true;
				break;
			}
			if (is_next_direction_exit) break; // 循环结束条件

		}
	}
	~Route() = default;

private:
	PosList idx_list;

	inline bool check_repetition_idx(const SDL_Point& pos) {
		for (auto &it : idx_list)
		{
			if (it.x == pos.x && it.y == pos.y)
				return true;
		}

		return false;
	}

};


