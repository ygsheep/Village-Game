#include "camera.h"

void Camera::UpdateDrag(int mouseX, int mouseY)
{
    if (isDragging) {
      offsetX += mouseX - lastMouseX;
      offsetY += mouseY - lastMouseY;
      lastMouseX = mouseX;
      lastMouseY = mouseY;
    }
}



