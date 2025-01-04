#pragma once
#include "singleton.h"
#include "Map.h"
#include "wave.h"

class ConfigManager: public Manager<ConfigManager>
{
public:
	friend class Manager<ConfigManager>;

  struct BasicTemplate
  {
    std::string window_title = u8"村庄保卫站";
    int window_width = 1280;
    int window_height = 720;
  };

  // 定义玩家模板结构体，包含玩家的基本属性
  struct PlayerTemplate
  {
    // 玩家移动速度，默认值为3.0
    double speed = 3;

    // 玩家普通攻击的间隔时间（秒），默认值为0.5秒
    double normal_attack_interval = 0.5;

    // 玩家普通攻击造成的伤害，默认值为0.0
    double normal_attack_damage = 0;

    // 玩家技能的冷却时间（秒），默认值为10秒
    double skill_interval = 10;

    // 玩家技能造成的伤害，默认值为1.0
    double skill_damage = 1;
  };

  // 定义塔模板结构体，包含塔的不同级别的属性
  struct TowerTemplate
  {
    // 塔的攻击间隔时间（秒），数组长度为10，表示10个级别
    double interval[10] = { 1 };

    // 塔的攻击伤害，数组长度为10，表示10个级别
    double damage[10] = { 25 };

    // 塔的视野范围，数组长度为10，表示10个级别
    double view_range[10] = { 5 };

    // 塔的建造成本，数组长度为10，表示10个级别
    double cost[10] = { 50 };

    // 塔的升级成本，数组长度为9，表示从1级到9级的升级成本
    double upgrade_cost[9] = { 75 };
  };

  // 定义敌人模板结构体，包含敌人的基本属性
  struct EnemyTemplate
  {
    // 敌人的生命值，默认值为100.0
    double hp = 100;

    // 敌人的移动速度，默认值为1.0
    double speed = 1;

    // 敌人造成的伤害，默认值为1.0
    double damage = 1;

    // 击杀敌人后获得的奖励比例，默认值为0.5
    double reward_ratio = 0.5;

    // 敌人恢复生命的间隔时间（秒），默认值为10秒
    double recover_interval = 10;

    // 敌人恢复生命的范围，默认值为0.0
    double recover_range = 0;

    // 敌人每次恢复的生命值强度，默认值为25.0
    double recover_intensity = 25;
  };

public:
	Map map;
	std::vector<Wave> wave_list;

	int level_archer = 0;
	int level_axeman = 0;
	int level_gunner = 0;

	bool is_game_win = true;
	bool is_game_over = false;
	SDL_Rect rect_tile_map = { 0 };

  BasicTemplate basic_template;

  PlayerTemplate player_template;

	TowerTemplate archer_template;
	TowerTemplate axeman_template;
	TowerTemplate gunner_template;

	EnemyTemplate slim_template;
	EnemyTemplate king_slim_template;
	EnemyTemplate skeleton_template;
	EnemyTemplate goblin_template;
	EnemyTemplate goblin_priest_template;

	const double num_initial_hp = 10;
	const double num_initial_coin = 100;
	const double num_coin_per_prop = 10;


private:
	ConfigManager()  = default;
	~ConfigManager() = default;
};

