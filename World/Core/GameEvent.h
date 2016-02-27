// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#pragma once
#ifndef GameEvent_h__
#define GameEvent_h__

#include <glm/glm.hpp>

class GameEvent
{
public:
  virtual ~GameEvent() = default;

  virtual void Execute(class World *world) = 0;

};


class GameEventMove : public GameEvent
{
public:
  GameEventMove(const glm::vec3 &moved)
    : mMoved(mMoved)
  {
  }

  void Execute(class World *world) override;

private:
  glm::vec3 mMoved;
};


class GameEventRotate : public GameEvent
{
public:
  GameEventRotate(const glm::vec3 &rotated)
    : mRotated(rotated)
  {
  }

  void Execute(class World *world) override;

private:
  glm::vec3 mRotated;
};


#endif // GameEvent_h__