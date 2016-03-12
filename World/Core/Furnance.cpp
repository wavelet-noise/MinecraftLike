#include "Furnance.h"
#include <imgui.h>
#include <glm\gtx\string_cast.hpp>
#include <Core\Chest.h>
#include <Core\Fuel.h>

Furnance::Furnance()
  : Agent(nullptr, "Furnance", "")
{
}

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
  if (remain_heat <= 0)
  {
    heat = 0;
    int slot;
    auto f = mParent->GetFromFullName<Chest>("Chest")->PopFirst(slot);
    if (f.obj)
      if (auto fuel = f.obj->GetFromFullName<Fuel>("Fuel"))
      {
        remain_heat = heat = fuel->getHeat();
        if (f.count > 0)
        {
          f.count--;
          mParent->GetFromFullName<Chest>("Chest")->Push(f.obj, f.count, slot);
        }
      }
  }
  else
  {
    remain_heat -= params.dt;
    T += params.dt;
  }
  if(T > 0)
    T -= params.dt / 4.f;


}

void Furnance::DrawGui()
{
  ImGui::LabelText("Temp", "%g C", T);
  ImGui::LabelText("Fuel efficiensy", "%g Mj/kg", heat);
  ImGui::ProgressBar(remain_heat / heat);
}

void Furnance::Interact(const InteractParams & params)
{
}
