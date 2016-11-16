#pragma once
#include <Core/Agent.h>

class GameObject;

class ColonyStorage : public Agent
{
public:
	AGENT(ColonyStorage);

	PAgent Clone(GameObject* parent, const std::string& name);
	void OnCreate(const GameObjectParams& params);
};

REGISTER_AGENT(ColonyStorage)
