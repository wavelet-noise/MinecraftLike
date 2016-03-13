#include "Heatable.h"
#include <Serealize.h>

Heatable::Heatable()
  : Agent(nullptr, "Heatable", "")
{
}

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
  T -= params.dt / 5.f;
}

void Heatable::Heat(float f)
{
  T += f;
}
