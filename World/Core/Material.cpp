#include "Material.h"
#include <Serealize.h>

void Material::JsonLoad(const rapidjson::Value &val)
{

}

PAgent Material::Clone(GameObject * parent, const std::string & name = "")
{
  return PAgent();
}

void Material::Update(const GameObjectParams & params)
{
}
