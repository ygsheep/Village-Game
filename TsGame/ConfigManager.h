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


private:
	ConfigManager()  = default;
	~ConfigManager() = default;
};

