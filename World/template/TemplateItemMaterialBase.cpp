#include "TemplateItemMaterialBase.h"
#include <core\GameObject.h>
#include <core\DB.h>
#include <Render\TextureManager.h>

void TemplateItemMaterialBase::Generate()
{
  Expand(materials);

  for (const auto &s : materials)
  {
    auto ngo = go->Clone();
    ngo->id = go->id + StringIntern("_") + s;

    TextureManager::Get().LoadTexturesMultipliedBackground(go->id, s, material_base);
    DB::Get().Registry(ngo->id, std::shared_ptr<GameObject>(ngo));
  }
}

void TemplateItemMaterialBase::JsonLoad(const rapidjson::Value & val)
{
  JSONLOAD(NVP(materials), NVP(material_base));
}
