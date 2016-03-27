#pragma once
#include "Agent.h"
#include "GameObject.h"
#include <vector>

class CraftingTable : public Agent
{
public:
	CraftingTable();

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;
	virtual void DrawGui() override;
	virtual void Interact(const InteractParams &params) override;

private:

};

REGISTER_AGENT(CraftingTable)