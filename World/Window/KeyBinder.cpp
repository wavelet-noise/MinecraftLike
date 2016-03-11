// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "KeyBinder.h"
#include <GLFW\glfw3.h>


KeyBinder::KeyBinder(Keyboard &keyboard, Mouse &mouse)
  : mKeyboard(keyboard), mMouse(mouse)
{
  Register("MOVE_FORWARD",  GLFW_KEY_W, GameEventMove({    0,  0.5, 0 }), KeyHandlerPress());
  Register("MOVE_BACKWARD", GLFW_KEY_S, GameEventMove({    0, -0.5, 0 }), KeyHandlerPress());
  Register("MOVE_LEFT",     GLFW_KEY_A, GameEventMove({ -0.5,    0, 0 }), KeyHandlerPress());
  Register("MOVE_RIGHT",    GLFW_KEY_D, GameEventMove({  0.5,    0, 0 }), KeyHandlerPress());
}

void KeyBinder::Update()
{
  float val = mMouse.DeltaX();
  if (mMouse.GetCentring())
  {
    mCallback(std::make_unique<GameEventRotate>(glm::vec3(0.0f, 0.0f, val / 200.f)));
  }

  val = mMouse.DeltaY();
  if (mMouse.GetCentring())
  {
    mCallback(std::make_unique<GameEventRotate>(glm::vec3(val / 200.f, 0.0f, 0.0f)));
  }

  for (auto &i : mMap)
  {
    if (std::get<3>(i)(mKeyboard.IsKeyDown(std::get<1>(i))))
    {
      mCallback(std::get<2>(i)());
    }
  }
}

