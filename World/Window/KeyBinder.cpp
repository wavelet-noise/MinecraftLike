// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "KeyBinder.h"
#include <GLFW\glfw3.h>


KeyBinder::KeyBinder(Keyboard &keyboard, Mouse &mouse)
  : mKeyboard(keyboard), mMouse(mouse)
{
  Register("MOVE_FORWARD", GLFW_KEY_W, GameEventMove({ 0, 1, 0 }), KeyHandlerPress());
}

void KeyBinder::Update()
{
  if (float val = mMouse.IsMoveX())
  {
    mCallback(std::make_unique<GameEventRotate>(glm::vec3(0.0f, 0.0f, val)));
  }

  if (float val = mMouse.IsMoveY())
  {
    mCallback(std::make_unique<GameEventRotate>(glm::vec3(val, 0.0f, 0.0f)));
  }

  for (auto &i : mMap)
  {
    if (std::get<3>(i)(mKeyboard.IsKeyPress(std::get<1>(i))))
    {
      mCallback(std::get<2>(i)());
    }
  }
}

