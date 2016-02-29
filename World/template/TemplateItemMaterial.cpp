#include "TemplateItemMaterial.h"
#include <core\GameObject.h>
#include <core\DB.h>
#include <Render\TextureManager.h>

void TemplateItemMaterial::Generate()
{
  std::vector<StringIntern> expanded;
  for (const auto &s : materials)
  {
    if (s.get().find("tag_") != std::string::npos)
    {
      auto taglist = DB::Get().Taglist(s);
      expanded.reserve(expanded.size() + taglist.size());
      for (const auto &t : taglist)
      {
        expanded.push_back(t);
      }
      LOG(trace) << s.get() << " expanded to " << taglist.size() << " materials";
    }
  }
  
  materials = std::move(expanded);
  
  //build metaitem models
  for (const auto &s : materials)
  {
    auto ngo = go->Clone();
    ngo->id = go->id + StringIntern("_") + s;

    auto basemodel = DB::Get().GetModel(go->id);
    auto nmodel = std::make_shared<Model>(*basemodel);
    TextureManager::Get().LoadTextureMultiplied(go->id, s);
    nmodel->SetSprite(ngo->id);
    DB::Get().PushModel(ngo->id, nmodel);


    DB::Get().Registry(ngo->id, std::shared_ptr<GameObject>(ngo));
  }
}

void TemplateItemMaterial::JsonLoad(const rapidjson::Value & val)
{
  std::vector<std::string> t;
  JSONLOAD(sge::make_nvp("materials", t));
  for (const auto &s : t)
  {
    materials.push_back(StringIntern(s));
  }
}
