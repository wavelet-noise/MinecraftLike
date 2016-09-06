#include "Agent.h"

class WaterBoiler : public Agent
{
public:
	AGENT(WaterBoiler)

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;
	virtual void DrawGui(float gt) override;

	float GetFreq() const override
	{
		return 1 / 10.f;
	}

private:
	float steam;
};

REGISTER_AGENT(WaterBoiler)