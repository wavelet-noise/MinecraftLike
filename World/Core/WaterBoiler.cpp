#include "WaterBoiler.h"
#include <Serealize.h>
#include <Core\GameObject.h>
#include <Core\Heatable.h>
#include <imgui.h>

WaterBoiler::WaterBoiler()
	: Agent(nullptr, "WaterBoiler", "")
{
}

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
	float T = mParent->GetFromFullName<Heatable>("Heatable")->GetTemp();

	if (T > 100)
	{
		steam += T - 100;
		mParent->GetFromFullName<Heatable>("Heatable")->Heat(-T + 100);
	}
}

void WaterBoiler::DrawGui()
{
	ImGui::LabelText("Steam", "%g m^3", steam);
}