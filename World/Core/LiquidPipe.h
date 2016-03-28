#pragma once
#include "Agent.h"
#include "GameObject.h"
#include <vector>
#include "ChestSlot.h"

class LiquidPipe : public Agent
{
public:
	LiquidPipe();

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;
	virtual void DrawGui() override;

	inline bool IsLocked() 
	{
		return mLocked;
	}

	inline float GetLiquidCount()
	{
		return liq.count;
	}

	StringIntern GetLiquidID();

	bool SetLiquid(ChestSlot cs);

	bool PushLiquid(ChestSlot cs);
	

private:
	bool mLocked = false;
	bool mDisabled = false;
	ChestSlot liq;
	float max_volume;
};

REGISTER_AGENT(LiquidPipe)