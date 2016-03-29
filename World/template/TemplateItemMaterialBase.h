#pragma once
#include "Template.h"
#include <vector>
#include <tools\StringIntern.h>

struct TemplateItemMaterialBase : public Template
{
  // ������������ ����� Template
  virtual void Generate() override;
  virtual void JsonLoad(const rapidjson::Value & val) override;

  StringIntern material_base;
  std::vector<StringIntern> materials;
};

REGISTER_TEMPLATE(TemplateItemMaterialBase)