#include "Farmland.h"
#include <Serealize.h>
#include <Core\World.h>
#include <core\agents\Plant.h>

void Farmland::JsonLoad(const rapidjson::Value & val)
{
	JSONLOAD(NVP(hydrated));
}

PAgent Farmland::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<Farmland>(*this);
	t->mParent = parent;
	return t;
}

void Farmland::Update(const GameObjectParams & params)
{
	tick += params.dt;
	if (tick >= 1)
	{
		if (auto b = params.world->GetBlock(params.pos + glm::vec3(0, 0, 1)))
		{
			if (auto p = b->GetAgent<Plant>())
			{
				if (p->GetNext() != StringIntern(""))
				{
					params.world->SetBlock(params.pos + glm::vec3(0, 0, 1), DB::Get().Create(p->GetNext()));
				}
			}
		}
		tick = 0;
	}
}
