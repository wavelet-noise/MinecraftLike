// ============================================================================
// ==                   Copyright (c) 2016, VASYAN                           ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include <boost\filesystem.hpp>
#include <boost\exception\diagnostic_information.hpp>
#include <rapidjson\document.h>
#include <tools\Log.h>

#include "DB.h"

//possibly must be moved into <agents> file
//deserialize autoreg
#include "BlockTessellator.h"
#include "PositionAgent.h"
#include "PhysicAgent.h"
#include "Tags.h"

DB &DB::Get()
{
  static DB obj;
  return obj;
}

void DB::Registry(const StringIntern &name, PGameObject block, bool isStatic)
{
  mObjects[name] = block;
}

void DB::ReloadDirectory(const std::string & dir)
{
  mObjects.clear();

  boost::filesystem::path targetDir(dir);
  boost::filesystem::recursive_directory_iterator iter(targetDir);

  int loaded = 0;
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

          if (!val.HasMember("id"))
          {
            LOG(error) << "record #" << i + 1 << " from " << file << " has no \"id\"";
            continue;
          }
          std::string id = val["id"].GetString();
          LOG(trace) << "\"" << id << "\" parsing";

          auto b = std::make_shared<GameObject>(StringIntern(id));

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

                  if (agenttype == "Tags")
                  {
                    LOG(trace) << "tagged as";
                    for (const std::string &s : std::static_pointer_cast<Tags>(c)->tags)
                    {
                      LOG(trace) << "   " << s;
                      tags_ref[StringIntern(s)].push_back(b);
                    }
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

          if (val.HasMember("render"))
          {
            rapidjson::Value &arr = val["render"];
            if (val["render"].IsArray())
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

          mObjects[StringIntern(id)] = b;
          loaded++;
        }
        
      }
    }
  }

  LOG(info) << loaded << " loaded";
}

PGameObject DB::Create(const StringIntern &name)
{
  return mObjects[name];
}

PGameObject DB::Create(const std::string &name)
{
  return mObjects[StringIntern(name)];
}

PGameObjectTessellator DB::CreateTesselator(const StringIntern &name)
{
  return mTess[name];
}
