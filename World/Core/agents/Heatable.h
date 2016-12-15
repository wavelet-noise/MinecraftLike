#ifndef Heatable_H
#define Heatable_H
#include <Core/agents/Agent.h>

class Heatable : public Agent
{
public:
	AGENT(Heatable)

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;
	void Heat(float f);
	inline float GetTemp()
	{
		return T;
	}

	bool DrawGui(const GameObjectParams& params, float gt) override;

private:
	float T;
};

REGISTER_AGENT(Heatable)

#endif // Heatable_H