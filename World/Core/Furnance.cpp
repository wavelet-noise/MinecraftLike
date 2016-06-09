#include "Furnance.h"
#include <imgui.h>
#include <glm\gtx\string_cast.hpp>
#include <Core\Chest.h>
#include <Core\Fuel.h>
#include <core\World.h>
#include <Core\Heatable.h>

void Furnance::JsonLoad(const rapidjson::Value &val)
{

}

PAgent Furnance::Clone(GameObject * parent, const std::string &name)
{
	auto t = MakeAgent<Furnance>(*this);
	t->mParent = parent;
	return t;
}

void Furnance::Update(const GameObjectParams & params)
{
	float delta = params.dt * 10;

	if (remain_heat <= 0)
	{
		heat = 0;
		int slot;
		auto f = mParent->GetAgent<Chest>()->PopFirst(slot);
		if (f.obj)
			if (auto fuel = f.obj->GetAgent<Fuel>())
			{
				remain_heat = heat = fuel->getHeat();
				if (f.count > 0)
				{
					f.count--;
					mParent->GetAgent<Chest>()->Push(f.obj, f.count, slot);
				}
			}
	}
	else
	{
		remain_heat -= delta;
		T += delta;
	}
	if (T > 0)
	{
		bool founded = false;
		if (auto upper = params.world->GetBlock(params.pos + glm::vec3(0, 0, 1)))
		{
			if (auto he = upper->GetAgent<Heatable>())
			{
				founded = true;
				he->Heat(T);
				T = 0;
			}
		}
		if (!founded)
			T -= params.dt / 4.f;
	}

}

void Furnance::DrawGui(float gt)
{
	ImGui::Text("Furnance");
	ImGui::LabelText("Temp", "%g C", T);
	ImGui::LabelText("Fuel efficiensy", "%g Mj/kg", heat);
	ImGui::ProgressBar(remain_heat / heat);
}

void Furnance::Interact(const InteractParams & params)
{
}
