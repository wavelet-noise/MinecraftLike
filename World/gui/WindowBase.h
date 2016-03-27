#pragma once
#include <imgui.h>
#include <glm\glm.hpp>

class WindowBase
{
public:
  virtual void Draw(glm::vec2 mainwin_size) = 0;

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