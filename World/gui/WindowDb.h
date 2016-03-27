#pragma once
#include "WindowBase.h"

class WindowDb : public WindowBase
{
public:

  // Унаследовано через WindowBase
  virtual void Draw(glm::vec2 mainwin_size) override;

  static WindowDb &Get()
  {
    static WindowDb w;
    return w;
  }
};