#pragma once
#include "enemy_type.h"
#include <vector>


struct Wave
{
	Wave() = default;
	~Wave() = default;

	struct SpawnEnemy
	{
		double interval = 0;
		int spawn_point = 1;
		EnemyType enemy_type = EnemyType::Slim;
	};

	double rawards = 0;
	double interal = 0;
	std::vector<SpawnEnemy> spawn_enevt_list;

};

