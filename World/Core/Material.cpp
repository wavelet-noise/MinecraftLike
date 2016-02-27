#include "Material.h"
#include <Serealize.h>

Material::Material()
  : Agent(nullptr, "Material", "")
{
}

void Material::JsonLoad(const rapidjson::Value &val)
{

}

PAgent Material::Clone(GameObject * parent, const std::string &)
{
  return PAgent();
}

void Material::Update(const GameObjectParams & params)
{
}
