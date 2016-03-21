#include "Template.h"
#include <Core\DB.h>

void Template::JsonLoad(const rapidjson::Value & val)
{
}

// разворачивает все строки tag_* в соответствующий массив id
void Template::Expand(std::vector<StringIntern> &vec)
{
  std::vector<StringIntern> expanded;
  for (const auto &s : vec)
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

  vec = std::move(expanded);
}

ObjectTemplateFactory::FactoryType & ObjectTemplateFactory::Get()
{
  static FactoryType object;
  return object;
}