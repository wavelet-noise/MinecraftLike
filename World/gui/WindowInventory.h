#pragma once
#include "WindowBase.h"

class World;

class WindowInventory : public WindowBase
{
public:

  WindowInventory();
  World *w;

  // Унаследовано через WindowBase
  virtual void Draw() override;

  static WindowInventory &Get()
  {
    static WindowInventory w;
    return w;
  }
};