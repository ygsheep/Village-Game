#pragma once
#include "Tile.h"
#include "Route.h"
#include <SDL.h>
#include <string>
#include <string_view>
#include <fstream>
#include <sstream>
#include <unordered_map>



class Map
{
public:
	typedef std::unordered_map<int, Route> SpawnerRoutePool; // Â·¾¶Éú³ÉÆ÷


	Map()  = default;
	~Map() = default;

	inline const size_t get_width() const
	{
		if (m_tileMap.empty())
			return 0;

		return m_tileMap[0].size();
	}

	inline const size_t get_height() const
	{
		return m_tileMap.size();
	}

	inline const SDL_Point& get_idx_home() const {
		return m_Idx_home;
	}

	inline const TileMap& get_tile_map() const {
		return m_tileMap;
	}
	
	inline const SpawnerRoutePool& get_spawner_route_pool() const {
		return m_spawner_route_pool;
	}

	void place_tower(const SDL_Point& pos) {
		m_tileMap[pos.y][pos.x].has_tower = true;
	}

	bool load(const std::string& Path) noexcept {
		std::ifstream file(Path);
		if (!file.good()) return false;

		TileMap tilemap_tmp;

		std::string str_line;
		size_t idx_x = -1, idx_y = -1;

		while (std::getline(file, str_line))
		{
			str_line = this->trim_str(str_line);
			if (str_line.empty()) continue;
			std::string str_row;
			std::stringstream str_stream(str_line);
			idx_y++;
			idx_x = -1;

			while (std::getline(str_stream, str_row, ','))
			{
				idx_x++;
				tilemap_tmp[idx_y].emplace_back();
				load_tile_from_str(tilemap_tmp[idx_y].back(), str_row);
			}
			file.close();
			
			if (tilemap_tmp.empty()|| tilemap_tmp[0].empty()) return false;

			m_tileMap = tilemap_tmp;
			generate_map_cache();
			return true;
		}
	}

private:
	TileMap m_tileMap;
	SDL_Point m_Idx_home = { 0 };
	SpawnerRoutePool m_spawner_route_pool;

	std::string trim_str(const std::string& str)const {
			size_t begin_idx = str.find_first_not_of(" \t");
			if (begin_idx == std::string::npos) 
				return "";

			size_t end_idx = str.find_last_not_of(" \t");
			return str.substr(begin_idx, end_idx);
	}

	void load_tile_from_str(Tile& tile, std::string str) {
		std::string str_trim = this->trim_str(str);
		std::string str_value;

		std::stringstream str_stream(str_trim);
		std::vector<int> values;

		while (std::getline(str_stream, str_value, '\\'))
		{
			int value = -1;
			try
			{
				value = std::stoi(str_value);
			}
			catch (const std::invalid_argument&)
			{
				value = -1;
			}
			values.push_back(value);
		}
		tile.terrian = (values.size() < 1 || values[0] < 0) ? 0 : values[0];
		tile.decoration = (values.size() < 2) ? -1 : values[1];
		tile.direction = (Tile::Direction)((values.size() < 3 || values[2] < 0 ? 0 : values[2]));
		tile.special_flag = (values.size() <= 3) ? -1 : values[3];
	}

	void generate_map_cache()
	{
		for (int y = 0; y < get_height(); y++)
		{
			for (int x = 0; x < get_width(); x++)
			{
				const Tile& tile = m_tileMap[y][x];
				if (tile.special_flag < 0)
					continue;

				if (tile.special_flag == 0)
				{
					m_Idx_home.x = x;
					m_Idx_home.y = y;
				}
				else {
					m_spawner_route_pool[tile.special_flag] = Route(m_tileMap, SDL_Point{x, y});
				}
			}
		}
	}
};

