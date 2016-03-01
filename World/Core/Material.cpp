#include "Material.h"
#include <Serealize.h>

Material::Material()
  : Agent(nullptr, "Material", "")
{
}

void Material::JsonLoad(const rapidjson::Value &val)
{
  JSONLOAD(NVP(mtex), NVP(dens));
}

PAgent Material::Clone(GameObject * parent, const std::string &)
{
  return std::make_shared<Material>(*this);
}

void Material::Update(const GameObjectParams & params)
{
}
