#include "Furnance.h"
#include <imgui.h>

Furnance::Furnance()
  : Agent(nullptr, "Furnance", "")
{
}

void Furnance::JsonLoad(const rapidjson::Value &val)
{

}

PAgent Furnance::Clone(GameObject * parent, const std::string &)
{
  return std::make_shared<Furnance>(*this);
}

void Furnance::Update(const GameObjectParams & params)
{
}

void Furnance::DrawGui(const GameObjectParams & params)
{
  static bool opened = false;
  ImGui::Begin("Furnance", &opened);

  ImGui::LabelText("Temp: ", "%g", T);

  ImGui::End();
}
