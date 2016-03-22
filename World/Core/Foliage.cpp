#include "Foliage.h"

Foliage::Foliage()
	: Agent(nullptr, "Foliage", "")
{
}

void Foliage::JsonLoad(const rapidjson::Value & val)
{
}

PAgent Foliage::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<Foliage>(*this);
	t->mParent = parent;
	return t;
}

void Foliage::Update(const GameObjectParams & params)
{
}
