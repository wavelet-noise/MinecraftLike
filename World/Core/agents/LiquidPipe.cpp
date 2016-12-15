#include "LiquidPipe.h"
#include <Serealize.h>
#include <imgui.h>
#include "Core\GameObject.h"
#include "Core\World.h"
#include "LiquidController.h"

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

	std::vector<LiquidPipe *> pipes;
	float total = liq.count;

	for (int i = 0; i < 6; i++)
	{
		if (auto n = params.world->GetBlock(params.pos + neib[i]))
		{
			if (auto p = n->GetAgent<LiquidPipe>())
			{
				if (!p->CanAccept(liq.obj))
					continue;

				total += p->GetLiquidCount();
				pipes.push_back(p);
			}
		}
	}

	for (auto r : pipes)
	{
		r->SetLiquid({ liq.obj, total / (pipes.size()+1) });
	}

	liq.count /= total / (pipes.size() + 1);
}

bool LiquidPipe::DrawGui(const GameObjectParams& params, float gt)
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
	return true;
}

void LiquidPipe::OnDestroy(const GameObjectParams & params)
{
}

void LiquidPipe::OnCreate(const GameObjectParams & params)
{
}

StringIntern LiquidPipe::GetLiquidID()
{
	if (liq.obj)
		return liq.obj->GetId();

	return StringIntern("");
}

bool LiquidPipe::SetLiquid(ChestSlot cs)
{
	if (!mDisabled)
	{
		if (!cs.obj)
			return true;

		if (liq.obj && *cs.obj == *liq.obj)
		{
			liq.count = cs.count;
			return true;
		}

		liq.obj = cs.obj;
		liq.count = cs.count;
		return true;
	}

	return false;
}

bool LiquidPipe::CanAccept(PGameObject i)
{
	return !i || !liq.obj || liq.obj->GetId() == i->GetId();
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
