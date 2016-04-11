#include "Agent.h"

class Plant : public Agent
{
public:
	AGENT(Plant)

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;

	StringIntern GetNext()
	{
		return next;
	}

private:
	StringIntern next;
};

REGISTER_AGENT(Plant)
