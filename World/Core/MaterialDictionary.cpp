#include "MaterialDictionary.h"
#include <Serealize.h>
#include <core\DB.h>
#include <Render\TextureManager.h>

MaterialDictionary::MaterialDictionary()
  : Agent(nullptr, "MaterialDictionary", "")
{
}

void MaterialDictionary::JsonLoad(const rapidjson::Value &val)
{
  std::vector<std::string> materials;
  JSONLOAD(NVP(materials));

  for (const auto &s : materials)
    this->materials.push_back(StringIntern(s));
}

PAgent MaterialDictionary::Clone(GameObject * parent, const std::string &)
{
  return PAgent();
}

void MaterialDictionary::Update(const GameObjectParams & params)
{
}

void MaterialDictionary::Afterload(GameObject * parent)
{
  std::vector<StringIntern> expanded;

  //expand tags to materials
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
    auto basemodel = DB::Get().GetModel(parent->GetId());
    auto nmodel = std::make_shared<Model>(*basemodel);
    nmodel->SetSprite(StringIntern(parent->GetId()) + s);
    DB::Get().PushModel(StringIntern(parent->GetId()) + s, nmodel);
  }
}
