// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef KeyBinder_h__
#define KeyBinder_h__

#include <vector>
#include <stdint.h>
#include <memory>
#include <tuple>
#include "..\Core\GameEvent.h"
#include <functional>
#include "Keyboard.h"
#include "Mouse.h"

/// Возвращает true единожды в момент нажатия.
struct KeyHandlerClick
{
  bool operator()(bool pressed)
  {
    auto oldPressed = mPressed;
    mPressed = pressed;

    return pressed && !oldPressed;
  }
private:
  bool mPressed = false;
};

/// Возвращает true если клавиша нажата.
struct KeyHandlerPress
{
  bool operator()(bool pressed)
  {
    return pressed;
  }
};



/// Формирует игровые события.
class KeyBinder
{
public:
  KeyBinder(Keyboard &keyboard, Mouse &mouse);

  template<class Event>
  inline void Register(const std::string &name, int32_t defaultKey, const Event &gameEvent, std::function<bool(bool)> keyHandler)
  {
    mMap.emplace_back(std::forward_as_tuple
      (
        name,
        defaultKey, 
        [event = gameEvent]
        {
          return std::make_unique<Event>(event);
        }, keyHandler)
      );
  }


  void Update();

  void SetCallback(std::function<void(std::unique_ptr<GameEvent>)> callback)
  {
    mCallback = callback;
  }

private:
  std::vector<std::tuple<std::string, int32_t, std::function<std::unique_ptr<GameEvent>()>, std::function<bool(bool)>>> mMap;

  Keyboard &mKeyboard;
  Mouse &mMouse;

  std::function<void(std::unique_ptr<GameEvent>)> mCallback;
};



#endif // KeyBinder_h__
