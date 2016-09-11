#pragma once
#include "Template.h"
#include <vector>
#include <tools\StringIntern.h>

class TemplateItemMaterial : public Template
{
public:
	// Унаследовано через Template
	std::list<PGameObject> Generate() override;
	void JsonLoad(const rapidjson::Value & val) override;

	std::vector<StringIntern> materials;
};

REGISTER_TEMPLATE(TemplateItemMaterial)