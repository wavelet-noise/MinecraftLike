#include <Core/agents/Agent.h>

class Fuel : public Agent
{
public:
	AGENT(Fuel)

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;
	inline float getHeat()
	{
		return mHeat;
	}

private:
	float mHeat;
};

REGISTER_AGENT(Fuel)
