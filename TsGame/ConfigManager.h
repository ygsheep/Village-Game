#pragma once
#include "cJSON.h"
#include "singleton.h"
#include "Map.h"
#include "wave.h"

class ConfigManager: public Manager<ConfigManager>
{
public:
	friend class Manager<ConfigManager>;

  struct BasicTemplate
  {
    std::string window_title = u8"��ׯ����վ";
    int window_width = 1280;
    int window_height = 720;
  };

  // �������ģ��ṹ�壬������ҵĻ�������
  struct PlayerTemplate
  {
    // ����ƶ��ٶȣ�Ĭ��ֵΪ3.0
    double speed = 3;

    // �����ͨ�����ļ��ʱ�䣨�룩��Ĭ��ֵΪ0.5��
    double normal_attack_interval = 0.5;

    // �����ͨ������ɵ��˺���Ĭ��ֵΪ0.0
    double normal_attack_damage = 0;

    // ��Ҽ��ܵ���ȴʱ�䣨�룩��Ĭ��ֵΪ10��
    double skill_interval = 10;

    // ��Ҽ�����ɵ��˺���Ĭ��ֵΪ1.0
    double skill_damage = 1;
  };

  // ������ģ��ṹ�壬�������Ĳ�ͬ���������
  struct TowerTemplate
  {
    // ���Ĺ������ʱ�䣨�룩�����鳤��Ϊ10����ʾ10������
    double interval[10] = { 1 };

    // ���Ĺ����˺������鳤��Ϊ10����ʾ10������
    double damage[10] = { 25 };

    // ������Ұ��Χ�����鳤��Ϊ10����ʾ10������
    double view_range[10] = { 5 };

    // ���Ľ���ɱ������鳤��Ϊ10����ʾ10������
    double cost[10] = { 50 };

    // ���������ɱ������鳤��Ϊ9����ʾ��1����9���������ɱ�
    double upgrade_cost[9] = { 75 };
  };

  // �������ģ��ṹ�壬�������˵Ļ�������
  struct EnemyTemplate
  {
    // ���˵�����ֵ��Ĭ��ֵΪ100.0
    double hp = 100;

    // ���˵��ƶ��ٶȣ�Ĭ��ֵΪ1.0
    double speed = 1;

    // ������ɵ��˺���Ĭ��ֵΪ1.0
    double damage = 1;

    // ��ɱ���˺��õĽ���������Ĭ��ֵΪ0.5
    double reward_ratio = 0.5;

    // ���˻ָ������ļ��ʱ�䣨�룩��Ĭ��ֵΪ10��
    double recover_interval = 10;

    // ���˻ָ������ķ�Χ��Ĭ��ֵΪ0.0
    double recover_range = 0;

    // ����ÿ�λָ�������ֵǿ�ȣ�Ĭ��ֵΪ25.0
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

  bool load_level_config(std::string& path) {
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
            spawm_enemy.spawn_point = spawm_enemy_spawn_point->valuedouble;

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

			cJSON_Delete(json_root);
      if (wave_list.empty()) {
        return false;
      }
			return true;
    }
  }

  bool load_game_config(std::string& path) {

  }
private:
	ConfigManager()  = default;
	~ConfigManager() = default;
};

