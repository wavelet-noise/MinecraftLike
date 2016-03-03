// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "WaterBlockTesselator.h"
#include "..\Render\TextureManager.h"
#include "Config.h"
#include "Tessellator.h"
#include "SectorTessellator.h"
#include "..\tools\CoordSystem.h"
#include "GameObject.h"
#include <Serealize.h>

WaterBlockTessellator::WaterBlockTessellator()
{
  mModel.SetTexture(std::get<0>(TextureManager::Get().GetTexture("stone")));//TODO:remove
}


const Model & WaterBlockTessellator::GetModel(const TessellatorParams &params)
{
  const int32_t size = static_cast<int32_t>(SECTOR_SIZE);

  size_t sides = MeshPartialBlockGenerator::ALL;
  auto pos = cs::WBtoSB(params.wbpos, params.spos);
  if (pos.x < size - 1)
  {
    ++pos.x;
    if (auto block = params.sector->GetBlock(pos))
    {
      if (!block->IsTransparent())
      {
        sides &= ~MeshPartialBlockGenerator::RIGHT;
      }
    }
    --pos.x;
  }
  else
  {
    auto pos = static_cast<WBPos>(params.wbpos);
    ++pos.x;
    if (auto block = params.tesselator->GetBlock(pos))
    {
      if (!block->IsTransparent())
      {
        sides &= ~MeshPartialBlockGenerator::RIGHT;
      }
    }
  }
  if (pos.x > 0)
  {
    --pos.x;
    if (auto block = params.sector->GetBlock(pos))
    {
      if (!block->IsTransparent())
      {
        sides &= ~MeshPartialBlockGenerator::LEFT;
      }
    }
    ++pos.x;
  }
  else
  {
    auto pos = static_cast<WBPos>(params.wbpos);
    --pos.x;
    if (auto block = params.tesselator->GetBlock(pos))
    {
      if (!block->IsTransparent())
      {
        sides &= ~MeshPartialBlockGenerator::LEFT;
      }
    }
  }

  if (pos.y < size - 1)
  {
    ++pos.y;
    if (auto block = params.sector->GetBlock(pos))
    {
      if (!block->IsTransparent())
      {
        sides &= ~MeshPartialBlockGenerator::BACK;
      }
    }
    --pos.y;
  }
  else
  {
    auto pos = static_cast<WBPos>(params.wbpos);
    ++pos.y;
    if (auto block = params.tesselator->GetBlock(pos))
    {
      if (!block->IsTransparent())
      {
        sides &= ~MeshPartialBlockGenerator::BACK;
      }
    }
  }
  if (pos.y > 0)
  {
    --pos.y;
    if (auto block = params.sector->GetBlock(pos))
    {
      if (!block->IsTransparent())
      {
        sides &= ~MeshPartialBlockGenerator::FRONT;
      }
    }
    ++pos.y;
  }
  else
  {
    auto pos = static_cast<WBPos>(params.wbpos);
    --pos.y;
    if (auto block = params.tesselator->GetBlock(pos))
    {
      if (!block->IsTransparent())
      {
        sides &= ~MeshPartialBlockGenerator::FRONT;
      }
    }
  }

  if (pos.z < size - 1)
  {
    ++pos.z;
    if (auto block = params.sector->GetBlock(pos))
    {
      if (!block->IsTransparent())
      {
        sides &= ~MeshPartialBlockGenerator::TOP;
      }
    }
    --pos.z;
  }
  else
  {
    auto pos = static_cast<WBPos>(params.wbpos);
    ++pos.z;
    if (auto block = params.tesselator->GetBlock(pos))
    {
      if (!block->IsTransparent())
      {
        sides &= ~MeshPartialBlockGenerator::TOP;
      }
    }
  }
  if (pos.z > 0)
  {
    --pos.z;
    if (auto block = params.sector->GetBlock(pos))
    {
      if (!block->IsTransparent())
      {
        sides &= ~MeshPartialBlockGenerator::BOTTOM;
      }
    }
    ++pos.z;
  }
  else
  {
    auto pos = static_cast<WBPos>(params.wbpos);
    --pos.z;
    if (auto block = params.tesselator->GetBlock(pos))
    {
      if (!block->IsTransparent())
      {
        sides &= ~MeshPartialBlockGenerator::BOTTOM;
      }
    }
  }

  mModel.GetMesh() = mGenerator.Create(static_cast<MeshPartialBlockGenerator::Side>(sides), percent);

  return mModel;
}

void WaterBlockTessellator::SayDepth(float d)
{
  percent = d;
}

void WaterBlockTessellator::JsonLoad(const rapidjson::Value & val)
{
  JSONLOAD(sge::make_nvp("model", mModel), sge::make_nvp("generator", mGenerator));
  BlockTessellator::JsonLoad(val);
}

PGameObjectTessellator WaterBlockTessellator::Clone()
{
  return MakeBlockTessellator<WaterBlockTessellator>(*this);
}
