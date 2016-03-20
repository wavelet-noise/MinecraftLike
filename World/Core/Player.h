



#pragma once
#ifndef Player_h__
#define Player_h__

#include "Creature.h"
#include "ChestSlot.h"

class Player : public Creature
{
public:
  Player(const StringIntern &__id);
  ~Player();

  PGameObject Clone() override;

  // Повернуть камеру относительно текущей ориентации на заданный угол по трем осям.
  void Rotate(const glm::vec3 &degrees);

  // Переместиться, относительно текущего положения и ориентации.
  void Move(const glm::vec3 &dist);

  void SetAcceleration(const glm::vec3 & vel);

  void Accelerate(const glm::vec3 & vel);

  const glm::vec3 &GetPosition() const;

  void SetPosition(const glm::vec3 &pos);

  const glm::quat &GetRot() const;

private:
  static const StringIntern mPhysicAgentName;
};



#endif // Player_h__

