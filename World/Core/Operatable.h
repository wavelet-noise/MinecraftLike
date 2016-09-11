#pragma once
#include "Agent.h"
#include "orders\OrderBus.h"
#include <deque>

using POperatable = std::shared_ptr<class Operatable>;

class Operatable : public Agent, public std::enable_shared_from_this<Operatable>
{
public:
	AGENT(Operatable)

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;

	bool HasWork()
	{
		return true;
	}

	void DoWork()
	{
		operated = true;
		mCollectedWork += 1.0f;
	}

	float GetFreq() const override
	{
		return 5.0f;
	}

	virtual void DrawGui(float gt) override;

	void EndOperate();

private:
	std::deque<POrder> mWork;
	float mCollectedWork = 0.0f;
	bool operated = false;
};

REGISTER_AGENT(Operatable)