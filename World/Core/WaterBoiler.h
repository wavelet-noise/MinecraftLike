#include "Agent.h"

class WaterBoiler : public Agent
{
public:
	AGENT(WaterBoiler)

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;
	virtual void DrawGui() override;

private:
	float steam;
};

REGISTER_AGENT(WaterBoiler)