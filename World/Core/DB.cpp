
// ==                   Copyright (c) 2016, VASYAN                           ==


#include <boost\filesystem.hpp>
#include <boost\exception\diagnostic_information.hpp>
#include <rapidjson\document.h>
#include <tools\Log.h>
#include <render\TextureManager.h>

#include "DB.h"

//possibly must be moved into <agents> file
//deserialize autoreg
#include "BlockTessellator.h"
#include "PositionAgent.h"
#include "PhysicAgent.h"
#include "Material.h"
#include "Furnance.h"
#include "Fuel.h"
#include "Chest.h"
#include "Heatable.h"
#include "Xyz.h"
#include "agents/ChestBlock.h"

//possibly must be moved into <templates> file
//deserialize autoreg
#include <template\TemplateItemMaterial.h>
#include <template\TemplateItemMaterialBase.h>

//possibly must be moved into <tesselators> file
//deserialize autoreg
#include <Core\SplitBlockTessellator.h>

#include <boost/algorithm/string/predicate.hpp>
#include <Game.h>

DB &DB::Get()
{
	static DB obj;
	return obj;
}

void DB::Registry(const StringIntern &name, PGameObject block, bool isStatic, bool isActive)
{
	mObjects[name] = std::make_tuple(block, isStatic, isActive);
}

void DB::RegistryTesselator(const StringIntern & name, PGameObjectTessellator tess)
{
	mTess[name] = tess;
}

void DB::ReloadDirectory(const std::string & mDir)
{
	mObjects.clear();

	int json_count = 1;
	{
		boost::filesystem::path targetDir(mDir);
		boost::filesystem::recursive_directory_iterator iter(targetDir);

		for (const boost::filesystem::path &file : iter) {
			if (boost::filesystem::is_regular_file(file) && boost::filesystem::extension(file) == ".json")
				json_count++;
		}
	}

	Game::SetLoadPhase(0, "Json loading", 0, 4);

	boost::filesystem::path targetDir(mDir);
	boost::filesystem::recursive_directory_iterator iter(targetDir);
	float json_current = 1;
	for (const boost::filesystem::path &file : iter) {
		if (boost::filesystem::is_regular_file(file) && boost::filesystem::extension(file) == ".json")
		{
			json_current++;
			Game::SetLoadPhase(json_current / json_count);

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

			if (d.IsArray())
			{
				for (decltype(d.Size()) i = 0; i < d.Size(); i++)
				{
					rapidjson::Value &val = d[i];

					std::string type;
					if (val.HasMember("type"))
						type = val["type"].GetString();
					else
					{
						LOG(error) << "record #" << i + 1 << " from " << file << " has no \"type\"";
						continue;
					}

					bool dyn = false;
					if (val.HasMember("dynamic"))
					{
						dyn = val["dynamic"].GetBool_();;
					}

					bool act = false;
					if (val.HasMember("active"))
					{
						act = val["active"].GetBool_();;
					}

					std::string id = "noid";
					if (val.HasMember("id"))
					{
						id = val["id"].GetString();
					}
					else
						LOG(error) << "record #" << i + 1 << " from " << file << " has no \"id\"";

					LOG(trace) << "\"" << id << "\" parsing";

					auto b = std::make_shared<GameObject>(StringIntern(id));

					if (val.HasMember("placeable"))
					{
						b->placable = val["placeable"].GetBool_();;
					}

					if (val.HasMember("walkable"))
					{
						b->walkable = val["walkable"].GetBool_();;
					}

					if (val.HasMember("agents"))
					{
						rapidjson::Value &arr = val["agents"];
						if (val["agents"].IsArray())
						{
							for (decltype(arr.Size()) a = 0; a < arr.Size(); a++)
							{
								rapidjson::Value &part = arr[a];
								if (part.HasMember("type")) {
									std::string agenttype = part["type"].GetString();
									auto c = AgentFactory::Get().Create(StringIntern(agenttype));
									if (!c)
									{
										LOG(error) << "record \"" << id << "\" agent #" << a + 1 << " has unknown type = " << agenttype;
										continue;
									}
									try {
										c->JsonLoad(part);
									}
									catch (...) {
										LOG(error) << boost::current_exception_diagnostic_information(true);
										LOG(error) << id << "'s agent " << agenttype << " json deserialize failed. See agents documentation";
										continue;
									}

									b->mAgents[StringIntern(agenttype)] = std::move(c);
								}
								else
								{
									LOG(error) << "record \"" << id << "\" agent #" << a + 1 << " has no type";
								}
							}
						}
						else
						{
							LOG(error) << "record \"" << id << "\" parts is not valid agents array";
						}
					}

					if (val.HasMember("tesselator"))
					{
						rapidjson::Value &arr = val["tesselator"];
						if (val["tesselator"].IsArray())
						{
							for (decltype(arr.Size()) a = 0; a < arr.Size(); a++)
							{
								rapidjson::Value &part = arr[a];
								if (part.HasMember("type")) {
									std::string tesstype = part["type"].GetString();
									auto bt = BlockTessellatorFactory::Get().Create(StringIntern(tesstype));
									if (!bt)
									{
										LOG(error) << "record \"" << id << "\" tesselator #" << a + 1 << " has unknown type = " << tesstype;
										continue;
									}
									try {
										bt->JsonLoad(part);
									}
									catch (...) {
										LOG(error) << boost::current_exception_diagnostic_information(true);
										LOG(error) << id << "'s tesselator " << tesstype << " json deserialize failed. See tesselators documentation";
										continue;
									}

									if (arr.Size() > 1)
										mTess[StringIntern(id + "_" + std::to_string(a))] = std::move(bt);
									else
										mTess[StringIntern(id)] = std::move(bt);
								}
								else
								{
									LOG(error) << "record \"" << id << "\" tesselator #" << a + 1 << " has no type";
								}
							}
						}
						else
						{
							LOG(error) << "record \"" << id << "\" parts is not valid agents array";
						}
					}

					std::vector<std::string> tags;
					JSONLOAD(NVP(tags));
					if (!tags.empty())
					{
						for (const std::string &s : tags)
						{
							mTags[StringIntern(s)].push_back(StringIntern(id));
							mObjTags[StringIntern(id)].push_back(StringIntern(s));
						}
					}

					if (val.HasMember("template"))
					{
						rapidjson::Value &part = val["template"];
						if (part.HasMember("type")) {
							std::string temptype = part["type"].GetString();
							auto c = ObjectTemplateFactory::Get().Create(StringIntern(temptype));
							c->JsonLoad(part);
							c->go = b.get();

							mTempl.insert(mTempl.end(), c);
						}
					}

					if (val.HasMember("recipe")) // incomplete, ready after "final recipe filling"
					{
						rapidjson::Value &rec = val["recipe"];
						auto r = std::make_shared<Recipe>();
						mRecipe.push_back(r);

						r->JsonLoad(rec);
					}

					if (val.HasMember("recipes"))
					{
						rapidjson::Value &part = val["recipes"];

						for(int i = 0; i < part.Size(); i++)
						{
							rapidjson::Value &sub_part = part[i];

							auto r = std::make_shared<Recipe>();
							mRecipe.push_back(r);

							r->JsonLoad(sub_part);
						}
					}


					mObjects[StringIntern(id)] = std::make_tuple(b, dyn, act);
				}

			}
		}
	}

	LOG(info) << mObjects.size() << " loaded";
	LOG(info) "template expansion";

	Game::SetLoadPhase(0, "Template expanding", 1, 4);
	float i = 0;
	for (auto &t : mTempl)
	{
		Game::SetLoadPhase(i / mTempl.size());

		auto objtags = Tag(t->go->GetId());
		auto ngo = t->Generate();
		auto oldgo = mObjects[t->go->GetId()];

		for(auto & n : ngo)
		{
			mObjects[n->GetId()] = std::make_tuple(n, std::get<1>(oldgo), std::get<2>(oldgo));
			auto &l = mObjTags[n->GetId()];
			l.insert(l.end(), objtags.begin(), objtags.end());

			for(auto & s : objtags)
			{
				mTags[s].push_back(n->GetId());
			}
		}
		i++;
	}

	LOG(info) "expanded to " << mObjects.size() << " objects, then fill requirements";

	Game::SetLoadPhase(0, "Requirements checking", 2, 4);
	i = 0;
	for (auto &o : mObjects)
	{
		Game::SetLoadPhase(i / mObjects.size());
		std::get<0>(o.second)->Requirements();
		i++;
	}

	LOG(info) "done, then afterload";

	Game::SetLoadPhase(0, "Objects afterload", 3, 4);
	i = 0;
	for (auto &o : mObjects)
	{
		Game::SetLoadPhase(i / mObjects.size());
		std::get<0>(o.second)->Afterload();
		i++;
	}

	Game::SetLoadPhase(0, "Recipe expanding", 4, 4);
	std::list<PRecipe> old_recipes = std::move(mRecipe); //final recipe filling
	i = 0;
	for (const auto &r : old_recipes)
	{
		Game::SetLoadPhase(i / old_recipes.size());
		auto exp = r->Expand();
		if (!exp.empty())
		{
			for (auto &a : exp)
				AddRecipe(a);
		}
		else
		{
			AddRecipe(r);
		}
		i++;
	}

	LOG(info) "expanded to " << mRecipe.size() << " recipes";

	LOG(info) << "db done";

	auto b = std::make_shared<GameObject>(StringIntern("allagents"));
	for (auto &m : AgentFactory::Get().map_)
	{
		b->mAgents[m.first] = m.second();
	}
	mObjects[StringIntern("allagents")] = std::make_tuple(b, false, false);
}

const std::vector<StringIntern> &DB::Taglist(const StringIntern & name) const
{
	auto t = mTags.find(name);
	if (t != mTags.end())
		return t->second;
	return mTagsEmptyDummy;
}

const std::vector<StringIntern> &DB::Taglist(const std::string & name) const
{
	auto t = mTags.find(StringIntern(name));
	if (t != mTags.end())
		return t->second;
	return mTagsEmptyDummy;
}

const std::vector<StringIntern>& DB::Tag(const StringIntern& id) const
{
	auto t = mObjTags.find(id);
	if (t != mObjTags.end())
		return t->second;
	return mTagsEmptyDummy;
}

// Возвращает список id
const std::vector<StringIntern>& DB::Tag(const std::string& id) const
{
	auto t = mObjTags.find(StringIntern(id));
	if (t != mObjTags.end())
		return t->second;
	return mTagsEmptyDummy;
}

PGameObject DB::Create(const std::string & name) const
{
	const auto &t = mObjects.find(StringIntern(name))->second;
	if (std::get<1>(t))
		return std::get<0>(t)->Clone();
	else
		return std::get<0>(t);
}

PGameObject DB::Create(const StringIntern & name) const
{
	const auto &t = mObjects.find(name)->second;
	if (std::get<1>(t))
		return std::get<0>(t)->Clone();
	else
		return std::get<0>(t);
}

const PGameObject DB::Pick(const std::string & name) const
{
	return Pick(StringIntern(name));
}

const PGameObject DB::Pick(const StringIntern & name) const
{
	auto finded = mObjects.find(name);
	if(finded != mObjects.end())
		return std::get<PGameObject>(finded->second);

	return nullptr;
}

PGameObjectTessellator DB::CreateTesselator(const StringIntern &name) const
{
	return mTess.find(name)->second;
}

const std::list<PRecipe> &DB::GetUsing(const std::string & name) const
{
	return GetUsing(StringIntern(name));
}

const std::list<PRecipe> &DB::GetUsing(const StringIntern & name) const
{
	static auto empty = std::list<PRecipe>();
	const auto &t = mUsingCache.find(name);
	if (t != mUsingCache.end())
		return t->second;
	return empty;
}

const std::list<PRecipe> &DB::GetRecipe(const std::string & name) const
{
	return GetRecipe(StringIntern(name));
}

const std::list<PRecipe> &DB::GetRecipe(const StringIntern & name) const
{
	static auto empty = std::list<PRecipe>();
	const auto &t = mRecipeCache.find(name);
	if (t != mRecipeCache.end())
		return t->second;
	return empty;
}

PDeepRecipe DB::GetDeepRecipe(const StringIntern& name, const std::vector<StringIntern> &materials) const
{
	auto dr = std::make_shared<DeepRecipe>();
	const auto &list = GetRecipe(name);
	if (!list.empty())
	{
		dr->recipe = *list.begin();
		for (const auto &in : dr->recipe->input)
		{
			auto id = in.id;
			if (id.get().find("tag_") != std::string::npos)
			{
				auto taglist = Taglist(id);
				if (!taglist.empty())
				{
					for(const auto &t : taglist)
					{
						for(const auto &m : materials)
						{
							if(boost::algorithm::ends_with(t.get(), m.get()))
							{
								id = t.get();
								goto _done;
							}
						}
					}

					dr->incomplete = true;
					goto _failed;
				}
			}

			_done:

			{
				auto dr2 = GetDeepRecipe(id, materials);
				if (dr2)
				{
					dr->input_expantion.push_back(dr2);
					if (dr2->incomplete)
						dr->incomplete = true;
				}
			}

			_failed:;
		}

		return dr;
	}

	return nullptr;
}

const std::list<PRecipe>& DB::GetMachineRecipe(const std::string & name) const
{
	return GetMachineRecipe(StringIntern(name));
}

const std::list<PRecipe>& DB::GetMachineRecipe(const StringIntern & name) const
{
	static auto empty = std::list<PRecipe>();
	const auto &t = mRecipeMachineTag.find(name);
	if (t != mRecipeMachineTag.end())
		return t->second;

	return empty;
}

void DB::AddRecipe(PRecipe r)
{
	mRecipe.push_back(r);

	if(!r->machine.get().empty())
	{
		mRecipeMachineTag[r->machine].push_back(r);
	}

	for (const auto &a : r->input)
	{
		mUsingCache[a.id].push_back(r);
	}

	for (const auto &a : r->output)
	{
		mRecipeCache[a.id].push_back(r);
	}

	mRecipeMachineTag[r->machine].push_back(r);
}
