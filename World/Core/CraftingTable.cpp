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
	//ImGui::
}

void CraftingTable::Interact(const InteractParams & params)
{
}
