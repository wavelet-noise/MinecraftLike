#include "Template.h"

void Template::JsonLoad(const rapidjson::Value & val)
{
}

ObjectTemplateFactory::FactoryType & ObjectTemplateFactory::Get()
{
  static FactoryType object;
  return object;
}