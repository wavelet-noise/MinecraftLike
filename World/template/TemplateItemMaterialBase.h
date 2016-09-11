#pragma once
#include "Template.h"
#include <vector>
#include <tools\StringIntern.h>

class TemplateItemMaterialBase : public Template
{
public:
  // Унаследовано через Template
  std::list<PGameObject> Generate() override;
  void JsonLoad(const rapidjson::Value & val) override;

  StringIntern material_base;
  std::vector<StringIntern> materials;
};

REGISTER_TEMPLATE(TemplateItemMaterialBase)