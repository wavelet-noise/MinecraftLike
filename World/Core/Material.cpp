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

PAgent Material::Clone(GameObject * parent, const std::string &name)
{
  auto t = MakeAgent<Material>(*this);
  t->mParent = parent;
  return t;
}

void Material::Update(const GameObjectParams & params)
{
}
