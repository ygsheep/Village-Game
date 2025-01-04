#pragma once
#include <SDL_rect.h>


// ���������
class Camera {
public:
  float zoom = 1.0f;         // ���ű���
  int offsetX = 0;           // ƽ��ƫ������X�ᣩ
  int offsetY = 0;           // ƽ��ƫ������Y�ᣩ
  bool isDragging = false;   // �Ƿ�����ק״̬
  int lastMouseX = 0;        // ��һ�����λ�ã�X�ᣩ
  int lastMouseY = 0;        // ��һ�����λ�ã�Y�ᣩ

  // ����ƽ�ƣ���ק������
  void UpdateDrag(int mouseX, int mouseY);
  // ���������Ļ�ռ�ӳ�䵽����ռ�
  SDL_Point ScreenToWorld(int screenX, int screenY) const {
    return {
        static_cast<int>((screenX - offsetX) / zoom),
        static_cast<int>((screenY - offsetY) / zoom)
    };
  }
};


