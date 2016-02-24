#pragma once
#include <imgui.h>

class WindowBase
{
public:
  virtual void Draw() = 0;

  void Hide()
  {
    mOpen = false;
  }

  void Show()
  {
    mOpen = true;
  }

  void SetVisibility(bool val)
  {
    mOpen = val;
  }

  bool GetVisibility()
  {
    return mOpen;
  }

protected:
  bool mOpen = true;
};