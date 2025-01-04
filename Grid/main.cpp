#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>
#include "camera.h"


// 定义常量
const int GRID_CELL_SIZE = 36;    // 网格单元格大小
const int GRID_WIDTH = 29;        // 网格宽度（列数）
const int GRID_HEIGHT = 20;       // 网格高度（行数）
const int INITIAL_WINDOW_WIDTH = GRID_WIDTH * GRID_CELL_SIZE + 1;  // 初始窗口宽度
const int INITIAL_WINDOW_HEIGHT = GRID_HEIGHT * GRID_CELL_SIZE + 1; // 初始窗口高度

// 坐标轴颜色
SDL_Color axis_x_color = { 255, 0, 0, 255 }; // 红色 X 轴
SDL_Color axis_y_color = { 0, 255, 0, 255 }; // 绿色 Y 轴


// 初始化 SDL
bool InitSDL(SDL_Window*& window, SDL_Renderer*& renderer) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL 初始化失败: %s", SDL_GetError());
    return false;
  }
  if (SDL_CreateWindowAndRenderer(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, 0, &window, &renderer) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "窗口和渲染器创建失败: %s", SDL_GetError());
    return false;
  }
  SDL_SetWindowTitle(window, u8"SDL 网格视图");
  return true;
}

// 绘制网格背景
void DrawGridBackground(SDL_Renderer* renderer, const SDL_Color& background_color) {
  SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);
  SDL_RenderClear(renderer);
}

// 绘制网格线
void DrawGridLines(SDL_Renderer* renderer, const SDL_Color& line_color, const Camera& camera, int cell_size, int grid_width, int grid_height) {
  SDL_SetRenderDrawColor(renderer, line_color.r, line_color.g, line_color.b, line_color.a);

  // 计算基于缩放和平移后的网格线位置
  int scaled_cell_size = static_cast<int>(cell_size * camera.zoom);
  int startX = -camera.offsetX % scaled_cell_size;
  int startY = -camera.offsetY % scaled_cell_size;

  // 绘制垂直线
  for (int x = startX; x < INITIAL_WINDOW_WIDTH; x += scaled_cell_size) {
    SDL_RenderDrawLine(renderer, x, 0, x, INITIAL_WINDOW_HEIGHT);
  }

  // 绘制水平线
  for (int y = startY; y < INITIAL_WINDOW_HEIGHT; y += scaled_cell_size) {
    SDL_RenderDrawLine(renderer, 0, y, INITIAL_WINDOW_WIDTH, y);
  }
}
int main() {
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;

  if (!InitSDL(window, renderer)) {
    return 0;
  }

  // 初始化相机
  Camera camera;

  // 初始化光标
  SDL_Rect grid_cursor = { GRID_WIDTH / 2 * GRID_CELL_SIZE, GRID_HEIGHT / 2 * GRID_CELL_SIZE, GRID_CELL_SIZE, GRID_CELL_SIZE };

  // 配色方案
  SDL_Color grid_background = { 22, 22, 22, 255 };
  SDL_Color grid_line_color = { 44, 44, 44, 255 };
  SDL_Color grid_cursor_color = { 255, 255, 255, 255 };

  bool quit = false;

  while (!quit) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        quit = true;
        break;
      case SDL_MOUSEWHEEL: // 鼠标滚轮缩放
        if (event.wheel.y > 0) { // 放大
          camera.zoom *= 1.1f;
        }
        else if (event.wheel.y < 0) { // 缩小
          camera.zoom /= 1.1f;
        }
        break;
      case SDL_MOUSEBUTTONDOWN: // 鼠标按下
        if (event.button.button == SDL_BUTTON_RIGHT) {
          camera.isDragging = true;
          camera.lastMouseX = event.button.x;
          camera.lastMouseY = event.button.y;
        }
        break;
      case SDL_MOUSEBUTTONUP: // 鼠标释放
        if (event.button.button == SDL_BUTTON_RIGHT) {
          camera.isDragging = false;
        }
        break;
      case SDL_MOUSEMOTION: // 鼠标移动
        if (camera.isDragging) {
          camera.UpdateDrag(event.motion.x, event.motion.y);
        }
        break;
      }
    }

    // 绘制场景
    DrawGridBackground(renderer, grid_background);
    DrawGridLines(renderer, grid_line_color, camera, GRID_CELL_SIZE, GRID_WIDTH, GRID_HEIGHT);
    SDL_RenderPresent(renderer); // 更新渲染内容
  }

  // 释放资源
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}


