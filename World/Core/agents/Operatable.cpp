#include "Operatable.h"
#include <core/orders/OrderOperate.h>
#include <imgui.h>

void Operatable::JsonLoad(const rapidjson::Value & val)
{
}

PAgent Operatable::Clone(GameObject * parent, const std::string & name)
{
	auto t = MakeAgent<Operatable>(*this);
	t->mParent = parent;
	return t;
}

void Operatable::Update(const GameObjectParams & params)
{
	if(!operated && !mWork.empty())
		OrderBus::Get().IssueOrder(std::make_shared<OrderOperate>(params.pos, shared_from_this()));
}

bool Operatable::DrawGui(const GameObjectParams& params, float gt)
{
	ImGui::Text("%g", mCollectedWork);
	ImGui::Button("WorkSome");
	return true;
}

void Operatable::EndOperate()
{
	operated = false;
}
