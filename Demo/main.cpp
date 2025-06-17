#include <iostream>
#include <memory>
#include <vector>
#include <algorithm> // 添加algorithm头文件用于remove_if

using namespace std;

struct Enemy {
  double hp;
  explicit Enemy(int hp) : hp(hp) {}
};

class EnemyManager {
public:
  using EnemyArray = vector<unique_ptr<Enemy>>;
  EnemyArray enemy_array;

  EnemyManager() {
    for (int i = 0; i < 10; ++i) {
      enemy_array.push_back(make_unique<Enemy>(i * 100));
    }
  }

  void printf_array() const {
    for (const auto& item : enemy_array) {
      if (item) cout << item->hp << " ";
    }
    cout << endl;
  }

  // 新增：检测并移除hp<=0的敌人
  void remove_dead_enemies() {
    // 使用"擦除-移除"惯用法
    auto new_end = remove_if(enemy_array.begin(), enemy_array.end(),
        [](const unique_ptr<Enemy>& enemy) {
            return enemy && enemy->hp <= 0;
        });

    // 删除死去的敌人
    enemy_array.erase(new_end, enemy_array.end());
  }

  // 新增：伤害敌人（用于测试）
  void damage_enemy(size_t index, double damage) {
    if (index < enemy_array.size() && enemy_array[index]) {
      enemy_array[index]->hp -= damage;
    }
  }
};

int main() {
  EnemyManager e;
  e.printf_array();

  // 测试：伤害一些敌人
  e.damage_enemy(0, 100);  // hp变为0
  e.damage_enemy(3, 300);  // hp变为100 -> 0
  e.damage_enemy(5, 600);  // hp变为500 -> -100

  cout << "Before removing dead enemies: ";
  e.printf_array();

  // 移除hp<=0的敌人
  e.remove_dead_enemies();

  cout << "After removing dead enemies: ";
  e.printf_array();

  return 0;
}