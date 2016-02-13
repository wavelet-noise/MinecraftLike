// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "BlockRenderStratery.h"
#include "..\Render\TextureManager.h"
#include "Config.h"
#include "Sector.h"
#include "World.h"
#include "..\tools\CoordSystem.h"
#include "Block.h"


const StringIntern BlockRenderStratery::mRenderAgentName = StringIntern("RenderAgent");


BlockRenderStratery::BlockRenderStratery()
{
  mModel.SetTexture(std::get<0>(TextureManager::Get().GetTexture("Textures/stone.png")));
}


BlockRenderStratery::~BlockRenderStratery()
{
}

const Model & BlockRenderStratery::GetModel(const GameObjectParams &params)
{
  if (params.sector->GetRenderSector().IsNeedBuild())
  {
    const int32_t size = static_cast<int32_t>(SECTOR_SIZE);

    size_t sides = MeshBlockGenerator::ALL;
    auto pos = cs::WBtoSB(params.pos, params.sector->GetSectorPosition());
    if (pos.x < size - 1)
    {
      ++pos.x;
      if (auto block = params.sector->GetBlock(pos))
      {
        if (!block->GetFromFullName<RenderAgent>(mRenderAgentName)->IsTransparent())
        {
          sides &= ~MeshBlockGenerator::RIGHT;
        }
      }
      --pos.x;
    }
    else
    {
      auto pos = static_cast<WBPos>(params.pos);
      ++pos.x;
      if (auto block = params.world->GetBlock(pos))
      {
        if (!block->GetFromFullName<RenderAgent>(mRenderAgentName)->IsTransparent())
        {
          sides &= ~MeshBlockGenerator::RIGHT;
        }
      }
    }
    if (pos.x > 0)
    {
      --pos.x;
      if (auto block = params.sector->GetBlock(pos))
      {
        if (!block->GetFromFullName<RenderAgent>(mRenderAgentName)->IsTransparent())
        {
          sides &= ~MeshBlockGenerator::LEFT;
        }
      }
      ++pos.x;
    }
    else
    {
      auto pos = static_cast<WBPos>(params.pos);
      --pos.x;
      if (auto block = params.world->GetBlock(pos))
      {
        if (!block->GetFromFullName<RenderAgent>(mRenderAgentName)->IsTransparent())
        {
          sides &= ~MeshBlockGenerator::LEFT;
        }
      }
    }

    if (pos.y < size - 1)
    {
      ++pos.y;
      if (auto block = params.sector->GetBlock(pos))
      {
        if (!block->GetFromFullName<RenderAgent>(mRenderAgentName)->IsTransparent())
        {
          sides &= ~MeshBlockGenerator::BACK;
        }
      }
      --pos.y;
    }
    else
    {
      auto pos = static_cast<WBPos>(params.pos);
      ++pos.y;
      if (auto block = params.world->GetBlock(pos))
      {
        if (!block->GetFromFullName<RenderAgent>(mRenderAgentName)->IsTransparent())
        {
          sides &= ~MeshBlockGenerator::BACK;
        }
      }
    }
    if (pos.y > 0)
    {
      --pos.y;
      if (auto block = params.sector->GetBlock(pos))
      {
        if (!block->GetFromFullName<RenderAgent>(mRenderAgentName)->IsTransparent())
        {
          sides &= ~MeshBlockGenerator::FRONT;
        }
      }
      ++pos.y;
    }
    else
    {
      auto pos = static_cast<WBPos>(params.pos);
      --pos.y;
      if (auto block = params.world->GetBlock(pos))
      {
        if (!block->GetFromFullName<RenderAgent>(mRenderAgentName)->IsTransparent())
        {
          sides &= ~MeshBlockGenerator::FRONT;
        }
      }
    }

    if (pos.z < size - 1)
    {
      ++pos.z;
      if (auto block = params.sector->GetBlock(pos))
      {
        if (!block->GetFromFullName<RenderAgent>(mRenderAgentName)->IsTransparent())
        {
          sides &= ~MeshBlockGenerator::TOP;
        }
      }
      --pos.z;
    }
    else
    {
      auto pos = static_cast<WBPos>(params.pos);
      ++pos.z;
      if (auto block = params.world->GetBlock(pos))
      {
        if (!block->GetFromFullName<RenderAgent>(mRenderAgentName)->IsTransparent())
        {
          sides &= ~MeshBlockGenerator::TOP;
        }
      }
    }
    if (pos.z > 0)
    {
      --pos.z;
      if (auto block = params.sector->GetBlock(pos))
      {
        if (!block->GetFromFullName<RenderAgent>(mRenderAgentName)->IsTransparent())
        {
          sides &= ~MeshBlockGenerator::BOTTOM;
        }
      }
      ++pos.z;
    }
    else
    {
      auto pos = static_cast<WBPos>(params.pos);
      --pos.z;
      if (auto block = params.world->GetBlock(pos))
      {
        if (!block->GetFromFullName<RenderAgent>(mRenderAgentName)->IsTransparent())
        {
          sides &= ~MeshBlockGenerator::BOTTOM;
        }
      }
    }

    mModel.GetMesh() = mMeshBlockGenerator.Create(static_cast<MeshBlockGenerator::Side>(sides));
  }

  return mModel;
}

void BlockRenderStratery::Load(const rapidjson::Value & val)
{
  auto &mg = GetGenerator();

  if (val.HasMember("all"))
  {
    mg.SetTexture(MeshBlockGenerator::ALL, val["all"].GetString());
  }
  if (val.HasMember("separate"))
  {
    const rapidjson::Value &arr = val["separate"];
    mg.SetTexture(MeshBlockGenerator::FRONT,  arr.Begin()->GetString());
    mg.SetTexture(MeshBlockGenerator::RIGHT,  arr[1].GetString());
    mg.SetTexture(MeshBlockGenerator::BACK,   arr[2].GetString());
    mg.SetTexture(MeshBlockGenerator::LEFT,   arr[3].GetString());
    mg.SetTexture(MeshBlockGenerator::TOP,    arr[4].GetString());
    mg.SetTexture(MeshBlockGenerator::BOTTOM, arr[5].GetString());
  }

  mg.Generate();
}
