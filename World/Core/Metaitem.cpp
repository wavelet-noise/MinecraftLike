#include "Metaitem.h"
#include <Serealize.h>

void Metaitem::JsonLoad(const rapidjson::Value &val)
{

}

PAgent Metaitem::Clone(GameObject * parent, const std::string & name = "")
{
  return PAgent();
}

void Metaitem::Update(const GameObjectParams & params)
{
}
