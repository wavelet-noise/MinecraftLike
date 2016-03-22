#pragma once
#include "Agent.h"

class Foliage : public Agent
{
public:
	Foliage();

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;
};

REGISTER_AGENT(Foliage)