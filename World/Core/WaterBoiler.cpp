#include "WaterBoiler.h"
#include <Serealize.h>
#include <Core\GameObject.h>
#include <Core\Heatable.h>
#include <imgui.h>
#include <Core\LiquidPipe.h>
#include <Core\DB.h>

void WaterBoiler::JsonLoad(const rapidjson::Value &val)
{
}

PAgent WaterBoiler::Clone(GameObject * parent, const std::string &name)
{
	auto t = MakeAgent<WaterBoiler>(*this);
	t->mParent = parent;
	return t;
}

void WaterBoiler::Update(const GameObjectParams & params)
{
	float T = mParent->GetAgent<Heatable>()->GetTemp();

	if (T > 100)
	{
		steam += T - 100;
		mParent->GetAgent<Heatable>()->Heat(-T + 100);
	}

	if (steam > 0)
	{
		if (auto p = mParent->GetAgent<LiquidPipe>())
		{
			if (p->PushLiquid({ DB::Get().Create("material_steam"), steam }))
			{
				steam = 0;
			}
		}
	}
}

void WaterBoiler::DrawGui()
{
}