#include "Fuel.h"
#include <Serealize.h>

Fuel::Fuel()
  : Agent(nullptr, "Fuel", "")
{
}

void Fuel::JsonLoad(const rapidjson::Value & val)
{
  JSONLOAD(sge::make_nvp("heat", mHeat));
}

PAgent Fuel::Clone(GameObject * parent, const std::string & name)
{
  auto t = MakeAgent<Fuel>(*this);
  t->mParent = parent;
  return t;
}

void Fuel::Update(const GameObjectParams & params)
{
}
