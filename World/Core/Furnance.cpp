#include "Furnance.h"
#include <imgui.h>
#include <glm\gtx\string_cast.hpp>

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

void Furnance::DrawGui()
{
  ImGui::LabelText("Temp: ", "%g", T);
}

void Furnance::Interact(const InteractParams & params)
{
}
