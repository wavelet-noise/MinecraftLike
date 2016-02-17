// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "SplitBlockTessellator.h"
#include "..\Render\TextureManager.h"
#include "Config.h"
#include "Sector.h"
#include "World.h"
#include "..\tools\CoordSystem.h"
#include "Block.h"
#include "Serialize.h"

SplitBlockTessellator::SplitBlockTessellator()
{
  mModel.SetTexture(std::get<0>(TextureManager::Get().GetTexture("Textures/stone.png")));
}


const Model & SplitBlockTessellator::GetModel(const GameObjectParams &params)
{
  /*if (params.sector->GetRenderSector().IsNeedBuild())
  {
    const int32_t size = static_cast<int32_t>(SECTOR_SIZE);

    size_t sides = MeshBlockGenerator::ALL;
    auto pos = cs::WBtoSB(params.pos, params.sector->GetSectorPosition());
    if (pos.x < size - 1)
    {
      ++pos.x;
      if (auto block = params.sector->GetBlock(pos))
      {
        if (!block->GetFromFullName<BlockTessellator>(mRenderAgentName)->IsTransparent())
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
        if (!block->GetFromFullName<BlockTessellator>(mRenderAgentName)->IsTransparent())
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
        if (!block->GetFromFullName<BlockTessellator>(mRenderAgentName)->IsTransparent())
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
        if (!block->GetFromFullName<BlockTessellator>(mRenderAgentName)->IsTransparent())
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
        if (!block->GetFromFullName<BlockTessellator>(mRenderAgentName)->IsTransparent())
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
        if (!block->GetFromFullName<BlockTessellator>(mRenderAgentName)->IsTransparent())
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
        if (!block->GetFromFullName<BlockTessellator>(mRenderAgentName)->IsTransparent())
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
        if (!block->GetFromFullName<BlockTessellator>(mRenderAgentName)->IsTransparent())
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
        if (!block->GetFromFullName<BlockTessellator>(mRenderAgentName)->IsTransparent())
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
        if (!block->GetFromFullName<BlockTessellator>(mRenderAgentName)->IsTransparent())
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
        if (!block->GetFromFullName<BlockTessellator>(mRenderAgentName)->IsTransparent())
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
        if (!block->GetFromFullName<BlockTessellator>(mRenderAgentName)->IsTransparent())
        {
          sides &= ~MeshBlockGenerator::BOTTOM;
        }
      }
    }

    mModel.GetMesh() = mGenerator.Create(static_cast<MeshBlockGenerator::Side>(sides));
  }
*/

  return mModel;
}

void SplitBlockTessellator::JsonLoad(const rapidjson::Value & val)
{
  JSONLOAD(sge::make_nvp("model", mModel), sge::make_nvp("generator", mGenerator));
}

PBlockTessellator SplitBlockTessellator::Clone()
{
  return MakeBlockTessellator<SplitBlockTessellator>(*this);
}