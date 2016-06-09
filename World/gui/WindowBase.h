#pragma once
#include <imgui.h>
#include <glm\glm.hpp>

class WindowBase
{
public:
	virtual ~WindowBase()
	{
	}

	virtual void Draw(glm::vec2 mainwin_size, float gt) = 0;

  void Hide()
  {
    mOpen = false;
  }

  void Show()
  {
    mOpen = true;
  }

  void Toggle()
  {
	  mOpen = !mOpen;
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