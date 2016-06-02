#include "Agent.h"
#include <vector>

class ChestSlot;

class Ore : public Agent
{
public:
	AGENT(Ore);

	struct OrePart
	{
		StringIntern id;
		int count = 1;
		float chance = 1.f;

		void OrePart::JsonLoad(const rapidjson::Value & val)
		{
			id = val.Begin()->GetString();
			if (val.Capacity() >= 2)
				count = val[1].GetInt();
			if (val.Capacity() >= 3)
				chance = val[2].GetDouble();
		}
	};

	void JsonLoad(const rapidjson::Value &val) override;

	// ������������ ����� Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void DrawGui() override;

	ChestSlot DigSome();
	bool Expire();

private:
	std::vector<OrePart> contains;
	int size = 1;
};

REGISTER_AGENT(Ore)