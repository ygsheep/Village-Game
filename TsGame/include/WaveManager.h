/**
    @file      WaveManager.h
    @brief     ~
    @details   ~
    @author    yy sheep
    @date      25-6-17
    @copyright © yy sheep, 2025. All right reserved.
**/
#pragma once
#include "singleton.h"

class WaveManager final: public Manager<WaveManager> {
  friend class Manager<WaveManager>;
  friend class std::default_delete<WaveManager>;

protected:
    WaveManager(){
    
    };
    ~WaveManager() override {

    };
};