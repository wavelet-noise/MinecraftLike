#pragma once
#include <Core/agents/Agent.h>

class Xyz : public Agent
{
public:
	AGENT(Xyz)

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	void OnCreate(const GameObjectParams & params) override;
};

REGISTER_AGENT(Xyz)
