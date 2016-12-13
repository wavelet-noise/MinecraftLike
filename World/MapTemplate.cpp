#include "MapTemplate.h"
#include <core/World.h>
#include <tools/StringIntern.h>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>

MapTemplate::MapTemplate()
{
}


MapTemplate::~MapTemplate()
{
}

void MapTemplate::ForceApplyToWorld(World* w, WBPos begin)
{
	for(int i = 0; i < size.x; i++)
	{
		for (int j = 0; j < size.y; j++)
		{
			for (int k = 0; k < size.z; k++)
			{
				w->SetBlock(begin + WBPos{ i,j,-k - size.z }, DB::Get().Create(cells[i*size.x*size.y + j*size.y + k].id));
			}
		}
	}
}

void MapTemplate::ApplyToWorld(World* w, WBPos begin)
{
	for (int i = 0; i < size.x; i++)
	{
		for (int j = 0; j < size.y; j++)
		{
			for (int k = 0; k < size.z; k++)
			{
				w->SetBlock(begin + WBPos{ i,j,-k - size.z }, DB::Get().Create(cells[i*size.x*size.y + j*size.y + k].id), true);
			}
		}
	}
}

#define error_return { LOG(error) << "map template has no \"data\" member"; return false; }
bool MapTemplate::JsonLoad(const rapidjson::Value& val)
{
	if (!val.HasMember("name"))
	{
		LOG(error) << "map template has no \"name\" member";
		return false;
	}

	name = val["name"].GetString();

	if (!val.HasMember("size"))
	{
		LOG(error) << "map template has no \"size\" member";
		return false;
	}

	const rapidjson::Value& sz = val["size"];
	if (!sz.IsArray())
		error_return;

	size.x = sz.Begin()->GetInt();
	size.y = sz[1].GetInt();
	size.z = sz[2].GetInt();

	cells.resize(size.x * size.y * size.z);

	if(!val.HasMember("data"))
	{
		LOG(error) << "map template has no \"data\" member";
		return false;
	}

	if (!val.HasMember("dictionary"))
	{
		LOG(error) << "map template has no \"dictionary\" member";
		return false;
	}

	std::map<char, StringIntern> dictionary;

	const rapidjson::Value& dict = val["dictionary"];
	if (!dict.IsArray())
		error_return;
	for (size_t i = 0; i < dict.Size(); i++)
	{
		const rapidjson::Value& pr = dict[i];
		if (!pr.IsArray())
			error_return;

		std::pair<char, StringIntern> pair;

		pair.first = pr.Begin()->GetString()[0];
		pair.second = pr[1].GetString();

		dictionary.insert(pair);
	}

	const rapidjson::Value& data = val["data"];
	if (!data.IsArray())
		error_return;

	if (data.Size() != size.z)
		error_return;

	for(size_t i = 0; i < data.Size(); i++)
	{
		const rapidjson::Value& slise = data[i];
		if (!slise.IsArray())
			error_return;

		if (slise.Size() != size.y)
			error_return;

		for (size_t j = 0; j < slise.Size(); j++)
		{
			const rapidjson::Value& row = slise[j];
			if (!row.IsString())
				error_return;

			const char * row_s = row.GetString();

			if (row.GetStringLength() != size.x)
				error_return;

			for (size_t k = 0; k < row.GetStringLength(); k++)
			{
				cells[k*size.x*size.y + j*size.y + i].id = dictionary[row_s[k]];
			}
		}
	}

	return true;
}
#undef error_return

void MapTemplates::ReloadDirectory(const std::string& mDir)
{
	templates.clear();

	boost::filesystem::path targetDir(mDir);
	boost::filesystem::recursive_directory_iterator iter(targetDir);
	float json_current = 1;
	for (const boost::filesystem::path &file : iter) {
		if (boost::filesystem::is_regular_file(file) && boost::filesystem::extension(file) == ".json")
		{
			std::ifstream fs(file.string());
			std::string all((std::istreambuf_iterator<char>(fs)),
				std::istreambuf_iterator<char>());

			LOG(trace) << "---------------";
			LOG(trace) << "parse " << file;
			rapidjson::Document d;
			d.Parse<0>(all.c_str());
			if (d.HasParseError())
			{
				LOG(error) << "while parsing " << file;
				LOG(error) << d.GetParseError();
				LOG(error) << all.substr(glm::max<int>(d.GetErrorOffset() - 20, 0), glm::min<int>(all.length(), 40));
				LOG(error) << "                    ^";
				throw;
			}

			const rapidjson::Value& elem = d;

			auto t = std::make_shared<MapTemplate>();
			if (t->JsonLoad(elem))
				templates[StringIntern(t->name)] = t;
		}
	}
}
