#include "TemplateItemMaterialBase.h"
#include <core\GameObject.h>
#include <core\DB.h>
#include <Render\TextureManager.h>

std::list<PGameObject> TemplateItemMaterialBase::Generate()
{
	std::list<PGameObject> ngo_list;

	Expand(materials);

	for (const auto &s : materials)
	{
		auto ngo = go->Clone();
		ngo->id = go->id + StringIntern("_") + s;

		TextureManager::Get().LoadTexturesMultipliedBackground(go->id, s, material_base);

		ngo_list.push_back(ngo);
	}

	return ngo_list;
}

void TemplateItemMaterialBase::JsonLoad(const rapidjson::Value & val)
{
	JSONLOAD(NVP(materials), NVP(material_base));
}
