



#include "Player.h"
#include "PhysicAgent.h"
#include "Chest.h"

const StringIntern Player::mPhysicAgentName = StringIntern("PhysicAgent");


Player::Player(const StringIntern &__id) : Creature(__id)
{
}


Player::~Player()
{
}

PGameObject Player::Clone()
{
  return MakeGameObject<Player>(id);
}

void Player::Rotate(const glm::vec3 &degrees)
{
  GetFromFullName<PhysicAgent>(mPhysicAgentName)->Rotate(degrees);
}

void Player::Move(const glm::vec3 &dist)
{
  GetFromFullName<PhysicAgent>(mPhysicAgentName)->Move(dist);
}

void Player::SetAcceleration(const glm::vec3 &vel)
{
  GetFromFullName<PhysicAgent>(mPhysicAgentName)->SetAcceleration(vel);
}

void Player::Accelerate(const glm::vec3 &vel)
{
  GetFromFullName<PhysicAgent>(mPhysicAgentName)->Accelerate(vel);
}

const glm::vec3 & Player::GetPosition() const
{
  return GetFromFullName<PhysicAgent>(mPhysicAgentName)->GetPos();
}

void Player::SetPosition(const glm::vec3 &pos)
{
  GetFromFullName<PhysicAgent>(mPhysicAgentName)->SetPos(pos);
}

const glm::quat & Player::GetRot() const
{
  return GetFromFullName<PhysicAgent>(mPhysicAgentName)->GetRot();
}