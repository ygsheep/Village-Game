/**

    @file      ConfigManager.h
    @brief     配置管理器类，用于加载和管理游戏配置文件。
    @details   ConfigManager类是一个单例类，负责加载游戏的配置文件，
    包括地图、波次、玩家、塔和敌人的属性等。它提供了加载配置文件的方法，
    并将解析后的数据存储在相应的成员变量中。
    @author    yy sheep
    @date      17.06.2025
    @copyright © yy sheep, 2025. All right reserved.

**/
#pragma once
#include "cJSON.h"
#include "map.h"
#include "singleton.h"
#include "wave.h"

class ConfigManager final : public Manager<ConfigManager>
{
public:
  friend class Manager<ConfigManager>;
  friend struct ::std::default_delete<ConfigManager>;


  struct BasicTemplate
  {
    std::string window_title = "村庄保卫站";
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
  map map;
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

  bool load_level_config(const std::string& path) {
    std::ifstream file(path);

    if (!file.good()) return false;

    std::stringstream str_stream;

    str_stream << file.rdbuf(); file.close();

    cJSON* json_root = cJSON_Parse(str_stream.str().c_str());

    if (json_root->type != cJSON_Array) {
      cJSON_Delete(json_root);
      return false;
    }

    cJSON* json_wave = nullptr;

    cJSON_ArrayForEach(json_wave, json_root) {

      if (json_wave->type != cJSON_Object)
        continue;

      wave_list.emplace_back();
      Wave& wave = wave_list.back();

      cJSON* json_wave_rewards = cJSON_GetObjectItem(json_wave, "rewards");
      if (json_wave_rewards && json_wave_rewards->type == cJSON_Number)
        wave.rawards = json_wave_rewards->valuedouble;

      cJSON* json_wave_interval = cJSON_GetObjectItem(json_wave, "interval");
      if (json_wave_interval && json_wave_interval->type == cJSON_Number)
        wave.interal = json_wave_interval->valuedouble;

      cJSON* json_wave_spawn_list = cJSON_GetObjectItem(json_wave, "spawn_list");
      if (json_wave_spawn_list && json_wave_spawn_list->type == cJSON_Array)
      {
        cJSON* json_spawm_event = nullptr;
        cJSON_ArrayForEach(json_spawm_event, json_wave_spawn_list) {
          wave.spawn_enevt_list.emplace_back();
          Wave::SpawnEnemy& spawm_enemy = wave.spawn_enevt_list.back();

          cJSON* spawm_enemy_interval = cJSON_GetObjectItem(json_spawm_event, "interval");
          if (spawm_enemy_interval && spawm_enemy_interval->type == cJSON_Number)
            spawm_enemy.interval = spawm_enemy_interval->valuedouble;

          cJSON* spawm_enemy_spawn_point = cJSON_GetObjectItem(json_spawm_event, "spawn_point");
          if (spawm_enemy_spawn_point && spawm_enemy_spawn_point->type == cJSON_Number)
            spawm_enemy.spawn_point = spawm_enemy_spawn_point->valueint;

          cJSON* json_spawn_event_enemy_type = cJSON_GetObjectItem(json_spawm_event, "enemy_type");
          if (json_spawn_event_enemy_type && json_spawn_event_enemy_type->type == cJSON_String)
          {
            const std::string str_enemy_type = json_spawn_event_enemy_type->valuestring;
            if (str_enemy_type == "Slim")
              spawm_enemy.enemy_type = EnemyType::Slim;
            else if (str_enemy_type == "KingSlim")
              spawm_enemy.enemy_type = EnemyType::KingSlim;
            else if (str_enemy_type == "Skeleton")
              spawm_enemy.enemy_type = EnemyType::Skeleton;
            else if (str_enemy_type == "Goblin")
              spawm_enemy.enemy_type = EnemyType::Goblin;
            else if (str_enemy_type == "GoblinPriest")
              spawm_enemy.enemy_type = EnemyType::GoblinPriest;
          }

        }

        if (wave.spawn_enevt_list.empty())
          wave_list.pop_back();
      }

    }

		cJSON_Delete(json_root);
		if (wave_list.empty()) {
			return false;
		}
		return true;

  }

  bool load_game_config(const std::string& path) {
    std::ifstream file(path);
    if (!file.good()) return false;

    std::stringstream str_stream;

    str_stream << file.rdbuf(); file.close();

    cJSON* json_root = cJSON_Parse(str_stream.str().c_str());

		if (!json_root || json_root->type != cJSON_Object) return false;

    cJSON* json_basic = cJSON_GetObjectItem(json_root, "basic");
    cJSON* json_player = cJSON_GetObjectItem(json_root, "player");
    cJSON* json_tower = cJSON_GetObjectItem(json_root, "tower");
    cJSON* json_enemy = cJSON_GetObjectItem(json_root, "enemy");

    if (!json_basic || !json_player || !json_tower || !json_enemy
      || json_basic->type != cJSON_Object
      || json_player->type != cJSON_Object
      || json_tower->type != cJSON_Object
      || json_enemy->type != cJSON_Object
      ) {
      cJSON_Delete(json_root);
      return false;
    }


    parse_basic_template(basic_template, json_basic);

    parse_player_template(player_template, json_player);

    parse_tower_template(archer_template, cJSON_GetObjectItem(json_tower, "archer"));
    parse_tower_template(axeman_template, cJSON_GetObjectItem(json_tower, "axeman"));
    parse_tower_template(gunner_template, cJSON_GetObjectItem(json_tower, "gunner"));

    parse_enemy_template(slim_template, cJSON_GetObjectItem(json_enemy, "slim"));
    parse_enemy_template(king_slim_template, cJSON_GetObjectItem(json_enemy, "king_slim"));
    parse_enemy_template(skeleton_template, cJSON_GetObjectItem(json_enemy, "skeleton"));
    parse_enemy_template(goblin_template, cJSON_GetObjectItem(json_enemy, "goblin"));
    parse_enemy_template(goblin_priest_template, cJSON_GetObjectItem(json_enemy, "goblin_priest"));

    cJSON_Delete(json_root);
    return true;
  }
protected:
  ConfigManager() = default;
  ~ConfigManager() override = default;

private:
  void parse_basic_template(BasicTemplate& tpi, cJSON* json_root) {

    if (json_root || json_root->type != cJSON_Object) return;

    cJSON* json_window_title = cJSON_GetObjectItem(json_root, "window_title");
    cJSON* json_width = cJSON_GetObjectItem(json_root, "window_width");
    cJSON* json_height = cJSON_GetObjectItem(json_root, "window_height");

    if (json_window_title && json_window_title->type == cJSON_String)
      tpi.window_title = json_window_title->valuestring;

    if (json_width && json_width->type == cJSON_Number)
      tpi.window_width = json_width->valueint;

    if (json_height && json_height->type == cJSON_Number)
      tpi.window_height = json_height->valueint;

  }

  void parse_player_template(PlayerTemplate& tpi, cJSON* json_root) {
    
    if (json_root || json_root->type != cJSON_Object) return;
     
    cJSON* json_speed = cJSON_GetObjectItem(json_root, "speed");
		cJSON* json_normal_attack_interval = cJSON_GetObjectItem(json_root, "normal_attack_interval");
		cJSON* json_normal_attack_damage = cJSON_GetObjectItem(json_root, "normal_attack_damage");
		cJSON* json_skill_interval = cJSON_GetObjectItem(json_root, "skill_interval");
		cJSON* json_skill_damage = cJSON_GetObjectItem(json_root, "skill_damage");


    if (json_speed && json_speed->type == cJSON_Number)
      tpi.speed = json_speed->valueint;

    if (json_normal_attack_interval && json_normal_attack_interval->type == cJSON_Number)
      tpi.normal_attack_interval = json_normal_attack_interval->valuedouble;

    if (json_normal_attack_damage && json_normal_attack_damage->type == cJSON_Number)
      tpi.normal_attack_damage = json_normal_attack_damage->valueint;

    if (json_skill_interval && json_skill_interval->type == cJSON_Number)
      tpi.skill_interval = json_skill_interval->valueint;

    if (json_skill_damage && json_skill_damage->type == cJSON_Number)
      tpi.skill_damage = json_skill_damage->valueint;

  }
  // 通用函数：解析 JSON 数组到固定长度的数组
  template <typename T, size_t N>
  void parse_json_array(cJSON* json_array, T(&target_array)[N]) {
    if (!json_array || json_array->type != cJSON_Array) return;

    size_t index = 0;
    cJSON* item = nullptr;
    cJSON_ArrayForEach(item, json_array) {
      if (index >= N) {
        // 数组长度超过目标数组容量，停止解析
        break;
      }
      if (item && item->type == cJSON_Number) {
        if constexpr (std::is_same_v<T, double>) {
          target_array[index] = item->valuedouble;
        }
        else if constexpr (std::is_same_v<T, int>) {
          target_array[index] = item->valueint;
        }
        index++;
      }
    }
  }
  void parse_tower_template(TowerTemplate& tpi, cJSON* json_root) {
    if (!json_root || json_root->type != cJSON_Object) return;

    // 解析 interval 数组
    cJSON* json_interval = cJSON_GetObjectItem(json_root, "interval");
    parse_json_array(json_interval, tpi.interval);

    // 解析 damage 数组
    cJSON* json_damage = cJSON_GetObjectItem(json_root, "damage");
    parse_json_array(json_damage, tpi.damage);

    // 解析 view_range 数组
    cJSON* json_view_range = cJSON_GetObjectItem(json_root, "view_range");
    parse_json_array(json_view_range, tpi.view_range);

    // 解析 cost 数组
    cJSON* json_cost = cJSON_GetObjectItem(json_root, "cost");
    parse_json_array(json_cost, tpi.cost);

    // 解析 upgrade_cost 数组
    cJSON* json_upgrade_cost = cJSON_GetObjectItem(json_root, "upgrade_cost");
    parse_json_array(json_upgrade_cost, tpi.upgrade_cost);
  }

  void parse_enemy_template(EnemyTemplate& tpi, cJSON* json_enemy) {

    if (!json_enemy || json_enemy->type != cJSON_Object) return;

    cJSON* json_hp = cJSON_GetObjectItem(json_enemy, "hp");
    cJSON* json_speed = cJSON_GetObjectItem(json_enemy, "speed");
    cJSON* json_damage = cJSON_GetObjectItem(json_enemy, "damage");
    cJSON* json_reward_ratio = cJSON_GetObjectItem(json_enemy, "reward_ratio");
    cJSON* json_recover_interval = cJSON_GetObjectItem(json_enemy, "recover_interval");
    cJSON* json_recover_range = cJSON_GetObjectItem(json_enemy, "recover_range");
    cJSON* json_recover_intensity = cJSON_GetObjectItem(json_enemy, "recover_intensity");

    if (json_hp && json_hp->type == cJSON_Number) tpi.hp = json_hp->valueint;
    if (json_speed && json_speed->type == cJSON_Number) tpi.speed = json_speed->valuedouble;
    if (json_damage && json_damage->type == cJSON_Number) tpi.damage = json_damage->valueint;
    if (json_reward_ratio && json_reward_ratio->type == cJSON_Number) tpi.reward_ratio = json_reward_ratio->valuedouble;
    if (json_recover_interval && json_recover_interval->type == cJSON_Number) tpi.recover_interval = json_recover_interval->valueint;
    if (json_recover_range && json_recover_range->type == cJSON_Number) tpi.recover_range = json_recover_range->valueint;
    if (json_recover_intensity && json_recover_intensity->type == cJSON_Number) tpi.recover_intensity = json_recover_intensity->valueint;

  }

};
