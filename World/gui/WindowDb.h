#pragma once
#include "WindowBase.h"

class WindowDb : public WindowBase
{
public:
  WindowDb();
  ~WindowDb();

  // Унаследовано через WindowBase
  virtual void Draw() override;

  ImTextureID tid;
};