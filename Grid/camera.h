#pragma once
#include <SDL_rect.h>


// 定义相机类
class Camera {
public:
  float zoom = 1.0f;         // 缩放比例
  int offsetX = 0;           // 平移偏移量（X轴）
  int offsetY = 0;           // 平移偏移量（Y轴）
  bool isDragging = false;   // 是否处于拖拽状态
  int lastMouseX = 0;        // 上一次鼠标位置（X轴）
  int lastMouseY = 0;        // 上一次鼠标位置（Y轴）

  // 更新平移（拖拽操作）
  void UpdateDrag(int mouseX, int mouseY);
  // 将坐标从屏幕空间映射到世界空间
  SDL_Point ScreenToWorld(int screenX, int screenY) const {
    return {
        static_cast<int>((screenX - offsetX) / zoom),
        static_cast<int>((screenY - offsetY) / zoom)
    };
  }
};


