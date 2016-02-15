// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "PhysicAgent.h"

#include <glm/gtc/matrix_transform.hpp>
#include "World.h"
#include "../tools/CoordSystem.h"


const StringIntern PhysicAgent::mPositionAgentName = StringIntern("PositionAgent");


PhysicAgent::PhysicAgent() 
  : Agent(nullptr, "PhysicAgent", "")
{
  mQuat = glm::quat_cast(glm::lookAt
    (
      glm::vec3(0.0f, 0.0f, 0.0f), // eye
      glm::vec3(0.0f, 1.0f, 0.0f), // center
      glm::vec3(0.0f, 0.0f, 1.0f)  // up
      ));
}

PhysicAgent::PhysicAgent(GameObject *parent, const std::string &name)
  : Agent(parent, "PhysicAgent", name)
{
  mQuat = glm::quat_cast(glm::lookAt
    (
      glm::vec3(0.0f, 0.0f, 0.0f), // eye
      glm::vec3(0.0f, 1.0f, 0.0f), // center
      glm::vec3(0.0f, 0.0f, 1.0f)  // up
      ));
}


PhysicAgent::PhysicAgent(const PhysicAgent &object, GameObject *parent, const std::string &name)
  : Agent(parent, "PhysicAgent", name)
{

}

PhysicAgent::~PhysicAgent()
{
}

PAgent PhysicAgent::Clone(GameObject *parent, const std::string &name)
{
  return MakeAgent<PhysicAgent>(*this, parent, name);
}

void PhysicAgent::Update(const GameObjectParams &params)
{
  const auto &pitch = glm::angleAxis(mDir.x, glm::vec3(1, 0, 0));
  const auto &yaw = glm::angleAxis(mDir.z, glm::vec3(0, 0, 1));
  const auto &roll = glm::angleAxis(mDir.y, glm::vec3(0, 1, 0));
  mDir = {};

  mQuat = pitch * mQuat * yaw;
  mQuat = glm::normalize(mQuat);

  mDirection = glm::mat3_cast(mQuat);
  //mVelocity.z -= params.dt * 0.3f;
  mDeltaPos += mVelocity;

  auto pos = GetPos();
  if (0)
  {
    auto s = params.world->GetSector(cs::WtoS(pos));
    if (!s)
    {
      mDeltaPos = {};
      mVelocity = {};
    }
  }

  if (glm::length(mDeltaPos) > 0)
  {
    auto dir = glm::normalize(mDeltaPos);
    auto sphere_checker = mDeltaPos + dir * 0.2f;

    if (!params.world->GetBlock(cs::WtoWB(pos + glm::vec3(sphere_checker.x, 0, 0))))
    {
      pos.x += mDeltaPos.x;
    }
    else
    {
      mVelocity.x = 0;
    }

    if (!params.world->GetBlock(cs::WtoWB(pos + glm::vec3(0, sphere_checker.y, 0))))
    {
      pos.y += mDeltaPos.y;
    }
    else
    {
      mVelocity.y = 0;
    }

    if (!params.world->GetBlock(cs::WtoWB(pos + glm::vec3(0, 0, sphere_checker.z))))
    {
      pos.z += mDeltaPos.z;
    }
    else
    {
      mVelocity.z = 0;
    }
  }

  SetPos(pos);
  mDeltaPos = {};
}

void PhysicAgent::Load(const rapidjson::Value & val)
{
  Agent::Load(val);
}

void PhysicAgent::SetPos(const WPos &pos)
{
  mParent->GetFromFullName<PositionAgent>(mPositionAgentName)->Set(pos);
}

const WPos &PhysicAgent::GetPos() const
{
  return mParent->GetFromFullName<PositionAgent>(mPositionAgentName)->Get();
}

void PhysicAgent::Rotate(const glm::vec3 &degrees)
{
  mDir += degrees;
}

void PhysicAgent::Move(const glm::vec3 &dist)
{
  mDeltaPos += glm::vec3(dist.x, dist.z, -dist.y) * mQuat;
}

void PhysicAgent::Accelerate(const glm::vec3 &vel)
{
  mVelocity += vel;
}

void PhysicAgent::SetAcceleration(const glm::vec3 &vel)
{
  mVelocity = vel;
}