/**
    @file      HomeManager.h
    @brief     ~
    @details   ~
    @author    yy sheep
    @date      25-6-17
    @copyright © yy sheep, 2025. All right reserved.
**/
#pragma once
#include "singleton.h"
#include "ConfigManager.h"
#include "ResourcesManager.h"

class HomeManager final
  :public Manager<HomeManager> {
  friend class Manager<HomeManager>;
  friend struct std::default_delete<HomeManager>;

public:
  double get_current_hp_num() const {
    return num_hp;
  }

  void decrease_hp(const double val)
  {
    num_hp -= val;

    if (num_hp < 0)
      num_hp = 0;

    static const ResourcesManager::SoundPool& sound_pool
      = ResourcesManager::getInstance()->get_sound_pool();

    Mix_PlayChannel(-1, sound_pool.find(ResID::Sound_HomeHurt)->second, 0);
  }

protected:
  HomeManager()
  {
    num_hp = ConfigManager::getInstance()->num_initial_hp;
  }

  ~HomeManager() override = default;

private:
  double num_hp = 0;
};