#include "TemplateItemMaterial.h"
#include <core\GameObject.h>
#include <core\DB.h>
#include <Render\TextureManager.h>
#include <Core\SplitBlockTessellator.h>

std::list<PGameObject> TemplateItemMaterial::Generate()
{
	std::list<PGameObject> ngo_list;
	Expand(materials);

	for (const auto &s : materials)
	{
		auto ngo = go->Clone();
		ngo->id = go->id + StringIntern("_") + s;

		TextureManager::Get().LoadTextureMultiplied(go->id, s);

		ngo_list.push_back(ngo);
	}

	return ngo_list;
}

void TemplateItemMaterial::JsonLoad(const rapidjson::Value & val)
{
	JSONLOAD(NVP(materials));
}
