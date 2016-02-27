#include "Metaitem.h"
#include <Serealize.h>

Metaitem::Metaitem()
  : Agent(nullptr, "Metaitem", "")
{
}

void Metaitem::JsonLoad(const rapidjson::Value &val)
{

}

PAgent Metaitem::Clone(GameObject * parent, const std::string &)
{
  return PAgent();
}

void Metaitem::Update(const GameObjectParams & params)
{
}
