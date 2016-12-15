#pragma once
#include <Core/agents/Agent.h>
#include "../GameObject.h"
#include <vector>

class CraftingTable : public Agent
{
public:
	AGENT(CraftingTable)

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	PAgent Clone(GameObject * parent, const std::string & name = "") override;
	void Update(const GameObjectParams & params) override;
	bool DrawGui(const GameObjectParams& params, float gt) override;
	void Interact(const InteractParams &params) override;

private:

};

REGISTER_AGENT(CraftingTable)

class Pulverizer : public Agent
{
public:
	AGENT(Pulverizer)

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	PAgent Clone(GameObject * parent, const std::string & name = "") override;
	void Update(const GameObjectParams & params) override;
	bool DrawGui(const GameObjectParams& params, float gt) override;

	float base_speed = 1;

	float GetFreq() const override
	{
		return 1.0f;
	}

private:

	float progress = 0;
};

REGISTER_AGENT(Pulverizer)

class PulverizeTo : public Agent
{
public:
	AGENT(PulverizeTo)

	struct PulverizePart
	{
		StringIntern id;
		int count;
		float probability;
	};

	void JsonLoad(const rapidjson::Value &val) override;
	virtual void Requirements() override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual bool DrawGui(const GameObjectParams& params, float gt) override;

	std::vector<PulverizePart> output;
};

REGISTER_AGENT(PulverizeTo)