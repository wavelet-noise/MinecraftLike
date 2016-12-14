#include "Plant.h"
#include <Serealize.h>

void Plant::JsonLoad(const rapidjson::Value & val)
{
	JSONLOAD(NVP(next));
}

PAgent Plant::Clone(GameObject * parent, const std::string &name)
{
	auto t = MakeAgent<Plant>(*this);
	t->mParent = parent;
	return t;
}

void Plant::Update(const GameObjectParams & params)
{
}
