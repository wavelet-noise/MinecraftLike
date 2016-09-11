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
				chance = static_cast<float>(val[2].GetDouble());
		}

		void save(boost::archive::binary_oarchive& ar, const unsigned) const;
		void load(boost::archive::binary_iarchive& ar, const unsigned);

		BOOST_SERIALIZATION_SPLIT_MEMBER()
	};

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void DrawGui(float gt) override;

	ChestSlot DigSome();
	bool Expire() const;

	float GetFreq() const override
	{
		return std::numeric_limits<float>::max();
	}

	void save(boost::archive::binary_oarchive& ar, const unsigned) const override;
	void load(boost::archive::binary_iarchive& ar, const unsigned) override;

	BOOST_SERIALIZATION_SPLIT_MEMBER()

private:
	std::vector<OrePart> contains;
	int size = 1;
};

REGISTER_AGENT(Ore)