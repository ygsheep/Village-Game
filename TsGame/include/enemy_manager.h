/**
    @file      enemy_manager.h
    @brief     ~
    @details   ~
    @author    yy sheep
    @date      25-6-17
    @copyright © yy sheep, 2025. All right reserved.
**/
#pragma once
#include "enemy.h"
#include "singleton.h"

#include "ConfigManager.h"
#include "BulletManager.h"
#include "HomeManager.h"

#include "enemy_slim.h"
#include "enemy_slim_king.h"
#include "enemy_skeleton.h"
#include "enemy_goblin.h"
#include "enemy_goblin_priest.h"

#include <vector>
#include <memory>


class EnemyManager final : public Manager<EnemyManager>{
  friend class Manager<EnemyManager>;
  friend struct std::default_delete<EnemyManager>;
public:
  using EnemyPtr = std::unique_ptr<Enemy>;  // 清晰表明指针类型
  using EnemyArray = std::vector<EnemyPtr>; // 容器定义保持连贯

public:
  void on_update(const double delta)
  {
    for (const auto& enemy : enemy_array)
      enemy->on_update(delta);

    process_home_collision();
    process_bullet_collision();

    remove_invalid_enemy();
  }

  void on_render(SDL_Renderer* renderer) const {
    for (const auto& enemy : enemy_array)
      enemy->on_render(renderer);
  }

  bool check_cleared() const {
    return enemy_array.empty();
  }

  const EnemyManager::EnemyArray& get_enemy_list()
  {
    return enemy_array;
  }
private:
	void process_home_collision() const {
		const auto&[x, y] = ConfigManager::getInstance()->map.get_idx_home();
		static const SDL_Rect& rect_tile_map = ConfigManager::getInstance()->rect_tile_map;
		static const Vector2 position_home_tile =
		{
			static_cast<double>(rect_tile_map.x + x * SIZE_TILE),
			static_cast<double>(rect_tile_map.y + y * SIZE_TILE)
		};

		for (const auto& enemy : enemy_array)
		{
			if (enemy->can_remove()) continue;

			const Vector2& position = enemy->get_position();

			if (position.x >= position_home_tile.x
				&& position.y >= position_home_tile.y
				&& position.x <= position_home_tile.x + SIZE_TILE
				&& position.y <= position_home_tile.y + SIZE_TILE)
			{
				enemy->make_invalid();

				HomeManager::getInstance()->decrease_hp(enemy->get_damage());
			}
		}
	}

	void process_bullet_collision()
	{
		static BulletManager::BulletList& bullet_list
			= BulletManager::getInstance()->get_bullet_list();

		for (const auto& enemy : enemy_array)
		{
			if (enemy->can_remove()) continue;

			const Vector2& size_enemy = enemy->get_size();
			const Vector2& pos_enemy = enemy->get_position();

			for (Bullet* bullet : bullet_list)
			{
				if (!bullet->can_collide()) continue;

				const Vector2& pos_bullet = bullet->get_position();

				if (pos_bullet.x >= pos_enemy.x - size_enemy.x / 2
					&& pos_bullet.y >= pos_enemy.y - size_enemy.y / 2
					&& pos_bullet.x <= pos_enemy.x + size_enemy.x / 2
					&& pos_bullet.y <= pos_enemy.y + size_enemy.y / 2)
				{
					double damage = bullet->get_damage();
					double damage_range = bullet->get_damage_range();
					if (damage_range < 0)
					{
						enemy->decrease_hp(damage);
						if (enemy->can_remove())
							try_spawn_coin_prop(pos_enemy, enemy->get_reward_ratio());
					}
					else
					{
						for (const auto& target_enemy : enemy_array)
						{
							const Vector2& pos_target_enemy = target_enemy->get_position();
							if ((pos_target_enemy - pos_bullet).length() <= damage_range)
							{
								target_enemy->decrease_hp(damage);
								if (target_enemy->can_remove())
									try_spawn_coin_prop(pos_target_enemy, enemy->get_reward_ratio());
							}
						}
					}

					bullet->on_collide(enemy);
				}
			}
		}
	}

	void remove_invalid_enemy()
	{
		std::erase_if(enemy_array,
                  [](const EnemyPtr& enemy)
                  {
                    return enemy->can_remove();
                  });
	}

	void try_spawn_coin_prop(const Vector2& position, const double ratio)
	{
		static ConfigManager* instance = ConfigManager::getInstance();

		if (static_cast<double>(rand() % 100) / 100 <= ratio)
			instance->spawn_coin_prop(position);
	}

protected:
  EnemyManager() {

  }

  ~EnemyManager() override {};

public:
  EnemyArray enemy_array;
};
