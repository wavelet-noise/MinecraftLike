#include "LiquidPipe.h"
#include <Serealize.h>
#include <imgui.h>
#include <Core\GameObject.h>
#include <Core\World.h>

LiquidPipe::LiquidPipe()
	: Agent(nullptr, "LiquidPipe", "")
{
}

void LiquidPipe::JsonLoad(const rapidjson::Value &val)
{
	JSONLOAD(NVP(max_volume));
}

PAgent LiquidPipe::Clone(GameObject * parent, const std::string &name)
{
	auto t = MakeAgent<LiquidPipe>(*this);
	t->mParent = parent;
	return t;
}

static glm::vec3 neib[] = {
	{ 1, 0, 0 },{ -1,  0,  0 },
	{ 0, 1, 0 },{  0, -1,  0 },
	{ 0, 0, 1 },{  0,  0, -1 }
};

void LiquidPipe::Update(const GameObjectParams & params)
{
	if (!liq.obj)
		return;

	for (int i = 0; i < 6; i++)
	{
		if (auto n = params.world->GetBlock(params.pos + neib[i]))
		{
			if (auto p = n->GetFromFullName<LiquidPipe>("LiquidPipe"))
			{
				if (p->PushLiquid({ liq.obj, liq.count / 2 }))
				{
					liq.count /= 2;
				}
			}
		}
	}
}

void LiquidPipe::DrawGui()
{
	ImGui::Text("LiquidPipe");
	ImGui::Checkbox("Locked", &mLocked);
	ImGui::Checkbox("Disabled", &mDisabled);
	ImGui::Button("Clean liquid");
	if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
	{
		liq.obj.reset();
		liq.count = 0;
	}

	liq.DrawGui();
}

StringIntern LiquidPipe::GetLiquidID()
{
	if (liq.obj)
		return liq.obj->GetId();

	return StringIntern("");
}

bool LiquidPipe::PushLiquid(ChestSlot cs)
{
	if (!mDisabled)
	{
		if (!cs.obj)
			return true;

		if (liq.obj && *cs.obj == *liq.obj)
		{
			liq.count += cs.count;
			return true;
		}

		liq.obj = cs.obj;
		liq.count = cs.count;
		return true;
	}

	return false;
}
