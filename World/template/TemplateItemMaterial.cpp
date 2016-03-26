#include "TemplateItemMaterial.h"
#include <core\GameObject.h>
#include <core\DB.h>
#include <Render\TextureManager.h>

void TemplateItemMaterial::Generate()
{
  Expand(materials);
  
  for (const auto &s : materials)
  {
    auto ngo = go->Clone();
    ngo->id = go->id + StringIntern("_") + s;

    TextureManager::Get().LoadTextureMultiplied(go->id, s);
    DB::Get().Registry(ngo->id, std::shared_ptr<GameObject>(ngo));
  }
}

void TemplateItemMaterial::JsonLoad(const rapidjson::Value & val)
{
  JSONLOAD(NVP(materials));
}
