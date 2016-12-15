#include "Heatable.h"
#include <Serealize.h>
#include <imgui.h>

void Heatable::JsonLoad(const rapidjson::Value &val)
{
}

PAgent Heatable::Clone(GameObject * parent, const std::string &name)
{
  auto t = MakeAgent<Heatable>(*this);
  t->mParent = parent;
  return t;
}

void Heatable::Update(const GameObjectParams & params)
{
}

void Heatable::Heat(float f)
{
  T += f;
}

bool Heatable::DrawGui(const GameObjectParams& params, float gt)
{
	ImGui::Text("Heatable");
	ImGui::LabelText("Temp", "%g C", T);
	return true;
}