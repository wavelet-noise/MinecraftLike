#pragma once
#include <Core/agents/Agent.h>
#include "../GameObject.h"
#include <vector>
#include "../ChestSlot.h"

class Furnance : public Agent
{
public:
	AGENT(Furnance)

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;
	virtual bool DrawGui(float gt) override;
	virtual void Interact(const InteractParams &params) override;

	float GetFreq() const override
	{
		return 1 / 10.f;
	}

private:
	float T;
	float remain_heat, heat;
};

REGISTER_AGENT(Furnance)