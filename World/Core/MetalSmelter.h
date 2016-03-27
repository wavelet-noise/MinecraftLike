#include "Agent.h"

class MetalSmelter : public Agent
{
public:
	MetalSmelter();

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;
};

REGISTER_AGENT(MetalSmelter)
