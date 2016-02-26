#include "Tags.h"
#include <Serealize.h>

void Tags::JsonLoad(const rapidjson::Value &val)
{
  std::vector<std::string> tags;
  JSONLOAD(NVP(tags));

  for(const auto &s : tags)
    this->tags.push_back(StringIntern(s));
}

PAgent Tags::Clone(GameObject * parent, const std::string & name = "")
{
  return PAgent();
}

void Tags::Update(const GameObjectParams & params)
{
}
