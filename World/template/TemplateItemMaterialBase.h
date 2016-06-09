#pragma once
#include "Template.h"
#include <vector>
#include <tools\StringIntern.h>

struct TemplateItemMaterialBase : public Template
{
  // Унаследовано через Template
  std::list<PGameObject> Generate() override;
  void JsonLoad(const rapidjson::Value & val) override;

  StringIntern material_base;
  std::vector<StringIntern> materials;
};

REGISTER_TEMPLATE(TemplateItemMaterialBase)