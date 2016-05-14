#include "CraftingTable.h"
#include <imgui.h>
#include <glm\gtx\string_cast.hpp>
#include <Core\Chest.h>
#include <core\World.h>

void CraftingTable::JsonLoad(const rapidjson::Value &val)
{

}

PAgent CraftingTable::Clone(GameObject * parent, const std::string &name)
{
	auto t = MakeAgent<CraftingTable>(*this);
	t->mParent = parent;
	return t;
}

void CraftingTable::Update(const GameObjectParams & params)
{

}

void CraftingTable::DrawGui()
{
	ImGui::Text("CraftingTable");
}

void CraftingTable::Interact(const InteractParams & params)
{

}

void Pulverizer::JsonLoad(const rapidjson::Value & val)
{
	JSONLOAD(NVP(base_speed));
}

PAgent Pulverizer::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<Pulverizer>(*this);
	t->mParent = parent;
	return t;
}

void Pulverizer::Update(const GameObjectParams & params)
{
	static ChestSlot working;
	if (working.obj == nullptr)
	{
		auto ichest = mParent->GetAgent<InputChest>();
		int slot;
		working = ichest->PopFirst(slot);
		if (working.obj && working.count > 1)
		{
			ichest->Push(working.obj->Clone(), working.count - 1, slot);
		}
	}
	else
	{
		progress += params.dt * base_speed;

		if (progress >= 0)
		{
			progress = 0;

			if (auto p = working.obj->GetAgent<PulverizeTo>())
			{
				for (const auto &i : p->output)
				{
					float rnd = rand() / float(RAND_MAX);

					if (rnd <= i.probability)
					{
						auto ichest = mParent->GetAgent<OutputChest>();
					}
				}
			}

			working.obj = nullptr;
		}
	}
}

void Pulverizer::DrawGui()
{
	ImGui::Text("CraftingTable");
}

void PulverizeTo::JsonLoad(const rapidjson::Value & val)
{
}

void PulverizeTo::Requirements()
{
	REQUIRE(OutputChest) REQUIRE(InputChest)
}

PAgent PulverizeTo::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<PulverizeTo>(*this);
	t->mParent = parent;
	return t;
}

void PulverizeTo::DrawGui()
{
}