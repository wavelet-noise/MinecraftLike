



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


class GameEventSelectPlayerSlot : public GameEvent
{
public:
  GameEventSelectPlayerSlot(int slot)
    : mSlot(slot)
  {
  }

  virtual void Execute(World * world) override;

private:
  int mSlot;
};

class GameEventMove : public GameEvent
{
public:
  GameEventMove(const glm::vec3 &moved)
    : mMoved(moved)
  {
  }

  void Execute(World *world) override;

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

  void Execute(World *world) override;

private:
  glm::vec3 mRotated;
};


#endif // GameEvent_h__