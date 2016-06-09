#pragma once
#include "Template.h"
#include <vector>
#include <tools\StringIntern.h>

struct TemplateItemMaterial : public Template
{
	// Унаследовано через Template
	std::list<PGameObject> Generate() override;
	void JsonLoad(const rapidjson::Value & val) override;

	std::vector<StringIntern> materials;
};

REGISTER_TEMPLATE(TemplateItemMaterial)