#include "KeyBinder.h"
#include <GLFW\glfw3.h>


KeyBinder::KeyBinder(Keyboard &keyboard, Mouse &mouse)
  : mKeyboard(keyboard), mMouse(mouse)
{
  Register("MOVE_FORWARD",  GLFW_KEY_W, GameEventMove({    0,  0.5, 0 }), KeyHandlerPress());
  Register("MOVE_BACKWARD", GLFW_KEY_S, GameEventMove({    0, -0.5, 0 }), KeyHandlerPress());
  Register("MOVE_LEFT",     GLFW_KEY_A, GameEventMove({ -0.5,    0, 0 }), KeyHandlerPress());
  Register("MOVE_RIGHT",    GLFW_KEY_D, GameEventMove({  0.5,    0, 0 }), KeyHandlerPress());
  Register("SELECT_1",      GLFW_KEY_1, GameEventSelectPlayerSlot(0),     KeyHandlerPress());
  Register("SELECT_2",      GLFW_KEY_2, GameEventSelectPlayerSlot(1),     KeyHandlerPress());
  Register("SELECT_3",      GLFW_KEY_3, GameEventSelectPlayerSlot(2),     KeyHandlerPress());
  Register("SELECT_4",      GLFW_KEY_4, GameEventSelectPlayerSlot(3),     KeyHandlerPress());
  Register("SELECT_5",      GLFW_KEY_5, GameEventSelectPlayerSlot(4),     KeyHandlerPress());
  Register("SELECT_6",      GLFW_KEY_6, GameEventSelectPlayerSlot(5),     KeyHandlerPress());
  Register("SELECT_7",      GLFW_KEY_7, GameEventSelectPlayerSlot(6),     KeyHandlerPress());
  Register("SELECT_8",      GLFW_KEY_8, GameEventSelectPlayerSlot(7),     KeyHandlerPress());
  Register("SELECT_9",      GLFW_KEY_9, GameEventSelectPlayerSlot(8),     KeyHandlerPress());
  Register("SELECT_0",      GLFW_KEY_0, GameEventSelectPlayerSlot(9),     KeyHandlerPress());
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

