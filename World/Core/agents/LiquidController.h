#pragma once
/*
#include <Core/agents/Agent.h>
#include "GameObject.h"
#include <vector>
#include "ChestSlot.h"

class LiquidController : public Agent
{
public:
	LiquidController();

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;
	virtual bool DrawGui(const GameObjectParams& params, float gt) override;

	inline bool IsLocked() 
	{
		return mLocked;
	}

	inline float GetLiquidCount()
	{
		return liq.count;
	}

	void OnDestroy(const GameObjectParams & params) override;
	void OnCreate(const GameObjectParams & params) override;

	StringIntern GetLiquidID();

	bool SetLiquid(ChestSlot cs);

	bool PushLiquid(ChestSlot cs);
	

private:
	bool mLocked = false;
	bool mDisabled = false;
	ChestSlot liq;
	float max_volume;
};

REGISTER_AGENT(LiquidController)
*/