



#pragma once
#ifndef PhysicAgent_h__
#define PhysicAgent_h__

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "GameObject.h"
#include "../tools/CoordSystem.h"



using PPhysicAgent = std::unique_ptr<class PhysicAgent>;

class PhysicAgent : public Agent
{
public:
	AGENT(PhysicAgent)

	PhysicAgent();
	~PhysicAgent();

	PAgent Clone(GameObject *parent, const std::string &name = "") override;

	void Update(const GameObjectParams &params) override;

	void JsonLoad(const rapidjson::Value &val) override;

	inline const glm::mat3 &GetDirection() const noexcept
	{
		return mDirection;
	}

	void SetPos(const WPos &pos);

	const WPos &GetPos() const;

	const glm::quat &GetRot() const
	{
		return mQuat;
	}

	// Повернуть камеру относительно текущей ориентации на заданный угол по трем осям.
	void Rotate(const glm::vec3 &degrees);

	// Переместиться, относительно текущего положения и ориентации.
	void Move(const glm::vec3 &dist);

	void Accelerate(const glm::vec3 & vel);

	void SetAcceleration(const glm::vec3 & vel);

private:
	static const StringIntern mPositionAgentName;

	glm::mat3 mDirection;
	glm::vec3 mDeltaPos;
	glm::vec3 mVelocity;

	glm::quat mQuat;
	glm::vec3 mDir;


};

REGISTER_AGENT(PhysicAgent)


#endif // PhysicAgent_h__