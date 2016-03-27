#include "MetalSmelter.h"
#include <Serealize.h>
#include <Core\GameObject.h>
#include <Core\Heatable.h>
#include <Core\Chest.h>

MetalSmelter::MetalSmelter()
	: Agent(nullptr, "MetalSmelter", "")
{
}

void MetalSmelter::JsonLoad(const rapidjson::Value &val)
{
}

PAgent MetalSmelter::Clone(GameObject * parent, const std::string &name)
{
	auto t = MakeAgent<MetalSmelter>(*this);
	t->mParent = parent;
	return t;
}

void MetalSmelter::Update(const GameObjectParams & params)
{
	float T = mParent->GetFromFullName<Heatable>("Heatable")->GetTemp();
	mParent->GetFromFullName<Heatable>("Heatable")->Heat(-T);

	auto chest = mParent->GetFromFullName<Chest>("Chest");
	auto &metal = chest->PopSlot(0);
	auto &form = chest->PopSlot(1);
	if (metal.obj)
	{

	}

	chest->Push(metal.obj, metal.count, 0);
	chest->Push(form.obj, form.count, 1);
}