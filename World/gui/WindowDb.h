#pragma once
#include "WindowBase.h"

class WindowDb : public WindowBase
{
public:

  // Унаследовано через WindowBase
  virtual void Draw() override;

  static WindowDb &Get()
  {
    static WindowDb w;
    return w;
  }
};