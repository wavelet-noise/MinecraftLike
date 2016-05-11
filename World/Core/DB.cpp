
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

//possibly must be moved into <templates> file
//deserialize autoreg
#include <template\TemplateItemMaterial.h>
#include <template\TemplateItemMaterialBase.h>

//possibly must be moved into <tesselators> file
//deserialize autoreg
#include <Core\SplitBlockTessellator.h>

DB &DB::Get()
{
  static DB obj;
  return obj;
}

void DB::Registry(const StringIntern &name, PGameObject block, bool isStatic)
{
  mObjects[name] = std::make_tuple(block, isStatic);
}

void DB::RegistryTesselator(const StringIntern & name, PGameObjectTessellator tess)
{
	mTess[name] = tess;
}

void DB::ReloadDirectory(const std::string & mDir)
{
  mObjects.clear();

  boost::filesystem::path targetDir(mDir);
  boost::filesystem::recursive_directory_iterator iter(targetDir);

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

          if (!val.HasMember("id"))
          {
            LOG(error) << "record #" << i + 1 << " from " << file << " has no \"id\"";
            continue;
          }
          std::string id = val["id"].GetString();
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
                  catch ( ... ) {
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

                  if(arr.Size() > 1)
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

            LOG(trace) << "tagged as";
            for (const std::string &s : tags)
            {
              LOG(trace) << "   " << s;
              mTags[StringIntern(s)].push_back(StringIntern(id));
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

		  if (val.HasMember("recipe"))
		  {
			  rapidjson::Value &part = val["recipe"];
			  auto r = std::make_shared<Recipe>();
			  mRecipe.push_back(r);

			  r->JsonLoad(part);
		  }


          mObjects[StringIntern(id)] = std::make_tuple(b, dyn);
        }
        
      }
    }
  }

  LOG(info) << mObjects.size() << " loaded";
  LOG(info) "template expansion";

  for (auto &t : mTempl)
  {
    t->Generate();
  }

  LOG(info) "expanded to " << mObjects.size() << " objects, then fill requirements";

  for (auto &o : mObjects)
  {
	  std::get<0>(o.second)->Requirements();
  }

  LOG(info) "done, then afterload";

  for (auto &o : mObjects)
  {
	  std::get<0>(o.second)->Afterload();
  }

  TextureManager::Get().Compile();

  std::list<PRecipe> old_recipes = std::move(mRecipe);
  for (const auto &r : old_recipes)
  {
	  auto &exp = r->Expand();
	  if (!exp.empty())
	  {
		  for (auto &a : exp)
			  AddRecipe(a);
	  }
	  else
	  {
		  AddRecipe(r);
	  }
  }

  LOG(info) "expanded to " << mRecipe.size() << " recipes";

  LOG(info) << "db done";

  auto b = std::make_shared<GameObject>(StringIntern("allagents"));
  for (auto &m : AgentFactory::Get().map_)
  {
	  b->mAgents[m.first] = m.second();
  }
  mObjects[StringIntern("allagents")] = std::make_tuple(b, false);
}

const std::vector<StringIntern> &DB::Taglist(const StringIntern & name) const
{
  return mTags.find(name)->second;
}

const std::vector<StringIntern> &DB::Taglist(const std::string & name) const
{
	return mTags.find(StringIntern(name))->second;
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
	if(t != mRecipeCache.end())
		return t->second;
	return empty;
}

void DB::AddRecipe(PRecipe r)
{
	mRecipe.push_back(r);

	for (const auto &a : r->input)
	{
		mUsingCache[a.id].push_back(r);
	}

	for (const auto &a : r->output)
	{
		mRecipeCache[a.id].push_back(r);
	}
}
