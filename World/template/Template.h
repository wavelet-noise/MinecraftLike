#pragma once
#include <boost/core/noncopyable.hpp>
#include <Core\TemplateFactory.h>
#include "../tools/StringIntern.h"
#include <rapidjson\document.h>

class GameObject;

class Template
{
public:
  virtual void Generate() = 0;
  virtual void JsonLoad(const rapidjson::Value &val) = 0;

  GameObject *go;
};

#define REGISTER_TEMPLATE(type) REGISTER_ELEMENT(type, ObjectTemplateFactory::Get(), StringIntern(#type))

struct ObjectTemplateFactory : public boost::noncopyable
{
  using FactoryType = TemplateFactory<StringIntern, Template>;
  static FactoryType &Get();
};