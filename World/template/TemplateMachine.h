#pragma once
#include "Template.h"
#include <vector>
#include <tools\StringIntern.h>

struct TemplateMachine : public Template
{
  // Унаследовано через Template
  virtual void Generate() override;
  virtual void JsonLoad(const rapidjson::Value & val) override;

  StringIntern material_base;
  std::vector<StringIntern> materials_metal;
};

REGISTER_TEMPLATE(TemplateMachine)