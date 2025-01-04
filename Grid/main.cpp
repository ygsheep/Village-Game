#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <iostream>
#include "camera.h"


// ���峣��
const int GRID_CELL_SIZE = 36;    // ����Ԫ���С
const int GRID_WIDTH = 29;        // �����ȣ�������
const int GRID_HEIGHT = 20;       // ����߶ȣ�������
const int INITIAL_WINDOW_WIDTH = GRID_WIDTH * GRID_CELL_SIZE + 1;  // ��ʼ���ڿ��
const int INITIAL_WINDOW_HEIGHT = GRID_HEIGHT * GRID_CELL_SIZE + 1; // ��ʼ���ڸ߶�

// ��������ɫ
SDL_Color axis_x_color = { 255, 0, 0, 255 }; // ��ɫ X ��
SDL_Color axis_y_color = { 0, 255, 0, 255 }; // ��ɫ Y ��


// ��ʼ�� SDL
bool InitSDL(SDL_Window*& window, SDL_Renderer*& renderer) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL ��ʼ��ʧ��: %s", SDL_GetError());
    return false;
  }
  if (SDL_CreateWindowAndRenderer(INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, 0, &window, &renderer) < 0) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "���ں���Ⱦ������ʧ��: %s", SDL_GetError());
    return false;
  }
  SDL_SetWindowTitle(window, u8"SDL ������ͼ");
  return true;
}

// �������񱳾�
void DrawGridBackground(SDL_Renderer* renderer, const SDL_Color& background_color) {
  SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);
  SDL_RenderClear(renderer);
}

// ����������
void DrawGridLines(SDL_Renderer* renderer, const SDL_Color& line_color, const Camera& camera, int cell_size, int grid_width, int grid_height) {
  SDL_SetRenderDrawColor(renderer, line_color.r, line_color.g, line_color.b, line_color.a);

  // ����������ź�ƽ�ƺ��������λ��
  int scaled_cell_size = static_cast<int>(cell_size * camera.zoom);
  int startX = -camera.offsetX % scaled_cell_size;
  int startY = -camera.offsetY % scaled_cell_size;

  // ���ƴ�ֱ��
  for (int x = startX; x < INITIAL_WINDOW_WIDTH; x += scaled_cell_size) {
    SDL_RenderDrawLine(renderer, x, 0, x, INITIAL_WINDOW_HEIGHT);
  }

  // ����ˮƽ��
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

  // ��ʼ�����
  Camera camera;

  // ��ʼ�����
  SDL_Rect grid_cursor = { GRID_WIDTH / 2 * GRID_CELL_SIZE, GRID_HEIGHT / 2 * GRID_CELL_SIZE, GRID_CELL_SIZE, GRID_CELL_SIZE };

  // ��ɫ����
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
      case SDL_MOUSEWHEEL: // ����������
        if (event.wheel.y > 0) { // �Ŵ�
          camera.zoom *= 1.1f;
        }
        else if (event.wheel.y < 0) { // ��С
          camera.zoom /= 1.1f;
        }
        break;
      case SDL_MOUSEBUTTONDOWN: // ��갴��
        if (event.button.button == SDL_BUTTON_RIGHT) {
          camera.isDragging = true;
          camera.lastMouseX = event.button.x;
          camera.lastMouseY = event.button.y;
        }
        break;
      case SDL_MOUSEBUTTONUP: // ����ͷ�
        if (event.button.button == SDL_BUTTON_RIGHT) {
          camera.isDragging = false;
        }
        break;
      case SDL_MOUSEMOTION: // ����ƶ�
        if (camera.isDragging) {
          camera.UpdateDrag(event.motion.x, event.motion.y);
        }
        break;
      }
    }

    // ���Ƴ���
    DrawGridBackground(renderer, grid_background);
    DrawGridLines(renderer, grid_line_color, camera, GRID_CELL_SIZE, GRID_WIDTH, GRID_HEIGHT);
    SDL_RenderPresent(renderer); // ������Ⱦ����
  }

  // �ͷ���Դ
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}


