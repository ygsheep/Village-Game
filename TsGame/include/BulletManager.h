/**
    @file      BulletManager.h
    @brief     ~
    @details   ~
    @author    yy sheep
    @date      25-6-17
    @copyright © yy sheep, 2025. All right reserved.
**/
#pragma once
#include "SipBase.h"

class BulletManager final
  :public Manager<BulletManager>{
  friend class Manager<BulletManager>;
  friend struct std::default_delete<BulletManager>;

public:
  using BulletList = std::vector<Bullet*>;
};