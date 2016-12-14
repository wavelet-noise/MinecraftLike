#pragma once
#include <Core/agents/Agent.h>

class Tool : public Agent
{
public:
	AGENT(Tool)

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;

private:
	StringIntern tool;
};

REGISTER_AGENT(Tool)