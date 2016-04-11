#pragma once
#include "Agent.h"
#include "GameObject.h"
#include <vector>

class Water : public Agent
{
public:
	AGENT(Water)

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;

private:
	float fill = 0.5;
};

REGISTER_AGENT(Water)