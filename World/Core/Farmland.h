#ifndef MATERIAL_H
#define MATERIAL_H
#include "Agent.h"

class Farmland : public Agent
{
public:
	AGENT(Farmland)

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;

private:
	bool hydrated = false;
	float tick = 0;
};

REGISTER_AGENT(Farmland)

#endif // MATERIAL_H
