#pragma once

#include <memory>
#include "Animation.h"
#include "ConfigManager.h"
#include "Route.h"
#include "Timer.h"
#include "vector2.h"


class Enemy {
public:
  using SKillCallBack = std::function<void(Enemy*)>;

  Enemy() {
    timer_skill.set_one_shot(false);
    timer_skill.set_function([this]() { on_skill_call(this); });

    timer_sketch.set_one_shot(true);
    timer_sketch.set_wait_time(0.075);
    timer_sketch.set_function([this]() { this->is_show_sketch = false; });

    timer_restore_speed.set_one_shot(true);
    timer_restore_speed.set_function([this]() { this->speed = this->max_speed; });

  } ;
  ~Enemy() = default;

  void on_update(const double delta) {
    timer_skill.update(delta);
    timer_sketch.update(delta);
    timer_restore_speed.update(delta);

    const Vector2 move_distance = velocity * delta;
    const Vector2 target_distance = position_target - position;
    position += std::min(move_distance, target_distance);

    if (position.approx_zero()) {
      idx_target++;
      refresh_position_target();
      direction = (position_target - position).normalize();
    }

    velocity.x = direction.x * speed * SIZE_TILE;
    velocity.y = direction.y * speed * SIZE_TILE;
    const bool is_show_x_anim = abs(velocity.x) > abs(velocity.y);

    if (is_show_sketch) {
      if (is_show_x_anim)
        anim_current = velocity.x > 0 ? anim_right_sketch : anim_left_sketch;
      else
        anim_current = velocity.y > 0 ? anim_down_sketch : anim_up_sketch;

    }else {
      if (is_show_x_anim)
        anim_current = velocity.x > 0 ? anim_right : anim_left;
      else
        anim_current = velocity.y > 0 ? anim_down : anim_up;

    }
    anim_current.update(delta);

  }

  void on_render(SDL_Renderer* renderer) const {
    static SDL_Rect hp_rect;
    static constexpr int hp_offset_y = 2;
    static const Vector2 hp_bar = {40, 8};
    static constexpr SDL_Color hp_border_color = { 90,175, 92, 255};
    static constexpr SDL_Color hp_content_color = {11,163,149, 255};


    const SDL_Point pos{
      static_cast<int>(position.x - SIZE_TILE / 2),
      static_cast<int>(position.y - SIZE_TILE / 2),
    };
    anim_current.render(renderer, pos);

    if (hp < max_hp) {
      hp_rect.x = static_cast<int>(position.x - hp_bar.x / 2);
      hp_rect.y = static_cast<int>(position.y - size.y / 2 - hp_offset_y);
      hp_rect.w = static_cast<int>(hp_bar.x * (hp / max_hp));

      SDL_SetRenderDrawColor(renderer, hp_content_color.r, hp_content_color.g,hp_content_color.b, hp_content_color.a);
      SDL_RenderFillRect(renderer, &hp_rect);

      hp_rect.w = static_cast<int>(size.x);
      SDL_SetRenderDrawColor(renderer, hp_border_color.r, hp_border_color.g, hp_border_color.b, hp_border_color.a);
      SDL_RenderDrawRect(renderer, &hp_rect);
    }
  }

  void set_on_skill_released(const SKillCallBack& cb) {
    on_skill_call = cb;
  }

  void increase_hp(const double val) {
    hp += val;
    if (hp > max_hp) {
      hp = max_hp;
    }
  }

  void decrease_hp(const double val) {
    hp -= val;
    if (hp <= 0) {
      hp = 0;
      is_valid = false;
    }
    is_show_sketch = true;
    timer_sketch.restart();
  }

  void slow_down()
  {
    speed = max_speed - 0.5;
    timer_restore_speed.set_wait_time(1);
    timer_restore_speed.restart();
  }

  void set_position(const Vector2& position)
  {
    this->position = position;
  }

  void set_route(Route* route)
  {
    this->route_ptr.reset(route);
    refresh_position_target();
  }

  void make_invalid()
  {
    is_valid = false;
  }

  double get_hp() const
  {
    return hp;
  }

  const Vector2& get_size() const
  {
    return size;
  }

  const Vector2& get_position() const
  {
    return position;
  }

  const Vector2& get_velocity() const
  {
    return velocity;
  }

  double get_damage() const
  {
    return damage;
  }

  double get_reward_ratio() const
  {
    return reward_ratio;
  }

  double get_recover_radius() const
  {
    return SIZE_TILE * recover_range;
  }

  double get_recover_intensity() const
  {
    return recover_intensity;
  }

  bool can_remove() const
  {
    return !is_valid;
  }

  double get_route_process() const
  {
    if (route_ptr.get()->get_idx_list().size() == 1)
      return 1;

    return static_cast<double>(idx_target / (route_ptr.get()->get_idx_list().size() - 1));
  }

protected:
  Vector2 size;
  Timer timer_skill;
  Animation anim_up;
  Animation anim_down;
  Animation anim_left;
  Animation anim_right;

  Animation anim_up_sketch;
  Animation anim_down_sketch;
  Animation anim_left_sketch;
  Animation anim_right_sketch;


  double hp = 0;
  double max_hp = 0;
  double speed = 0;
  double max_speed = 0;
  double damage = 0;
  double reward_ratio = 0;
  double recover_interval = 0;
  double recover_range = 0;
  double recover_intensity = 0;


private:
  Vector2 position;
  Vector2 direction;
  Vector2 velocity;

  bool is_valid = true;

  Timer timer_sketch;
  bool is_show_sketch = false;

  Animation& anim_current;

  SKillCallBack on_skill_call;

  Timer timer_restore_speed;

  std::unique_ptr<Route> route_ptr {nullptr};
  int idx_target = 0;
  Vector2 position_target;

protected:
  void refresh_position_target() {
    if (const Route::PosList &idx_list = route_ptr.get()->get_idx_list(); idx_target < idx_list.size()) {
      const auto &[x, y] = idx_list[idx_target];
      const SDL_Rect& rect_tile_map = ConfigManager::getInstance()->rect_tile_map;

      position_target.x = rect_tile_map.x + x * SIZE_TILE + SIZE_TILE / 2;
      position_target.y = rect_tile_map.y + y * SIZE_TILE + SIZE_TILE / 2;
    }
  }

};

