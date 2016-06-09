#pragma once
#include "WindowBase.h"

class World;

class WindowInventory : public WindowBase
{
public:

  WindowInventory();
  World *w;

  // Унаследовано через WindowBase
  virtual void Draw(glm::vec2 mainwin_size, float gt) override;

  static WindowInventory &Get()
  {
    static WindowInventory w;
    return w;
  }
};