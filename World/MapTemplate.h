#pragma once
#include <vector>
#include "tools/StringIntern.h"
#include "tools/CoordSystem.h"
#include <rapidjson/document.h>
#include <map>

class World;

struct MapTemplateCell
{
	StringIntern id;
};

class MapTemplate
{
public:
	MapTemplate();
	~MapTemplate();

	std::vector<MapTemplateCell> cells;
	glm::ivec3 size;
	std::string name;

	void ForceApplyToWorld(World * w, WBPos begin);
	void ApplyToWorld(World* w, WBPos begin);

	bool JsonLoad(const rapidjson::Value & val);
};
using PMapTemptale = std::shared_ptr<MapTemplate>;

class MapTemplates
{
public:
	void ReloadDirectory(const std::string & s);

	std::map<StringIntern, PMapTemptale> templates;

	static MapTemplates & Get()
	{
		static MapTemplates mt;
		return mt;
	}
};

