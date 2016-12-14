#ifndef MATERIAL_H
#define MATERIAL_H
#include <Core/agents/Agent.h>

class Material : public Agent
{
public:
	AGENT(Material)

	void JsonLoad(const rapidjson::Value &val) override;

	// Унаследовано через Agent
	virtual PAgent Clone(GameObject * parent, const std::string & name = "") override;
	virtual void Update(const GameObjectParams & params) override;

private:
	StringIntern mtex;
	int dens;
};

REGISTER_AGENT(Material)

#endif // MATERIAL_H
