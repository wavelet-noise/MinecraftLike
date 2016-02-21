// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "Sector.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdint.h>
#include "../tools/Log.h"
#include "../Render/Render.h"
#include "World.h"
#include "Tessellator.h"
#include "SectorBase.h"
#include <tuple>

Sector::Sector(const SPos &position)
  : mPos(position)
{
  //SectorBase<PBlock> t;
  //t.SetBlock({}, nullptr);
}


Sector::~Sector()
{
}

const SPos & Sector::GetPos() const
{
  return mPos;
}

PBlock Sector::GetBlock(const SBPos &pos)
{
  assert(glm::clamp(pos, 0, static_cast<int32_t>(SECTOR_SIZE - 1)) == pos);
  return mBlocks[pos.z * SECTOR_SIZE * SECTOR_SIZE + pos.y * SECTOR_SIZE + pos.x];
}

void Sector::SetBlock(const SBPos &pos, PBlock block)
{
  assert(glm::clamp(pos, 0, static_cast<int32_t>(SECTOR_SIZE - 1)) == pos);
  mBlocks[pos.z * SECTOR_SIZE * SECTOR_SIZE + pos.y * SECTOR_SIZE + pos.x] = block;

  if (mTessellator && block)
  {
    mTessellator->Set(cs::SBtoWB(pos, mPos), DB::Get().CreateTesselator(block->GetId()));
    mTessellator->SayChanged(mPos);
  }
}

void Sector::Update(World *world)
{

}



void Sector::Draw(class Tessellator *tess)
{
  mTessellator = tess;
  if (mTessellator)
  {
    auto currentTime = glfwGetTime();
    std::vector<std::tuple<size_t, StringIntern>> blocks;
    blocks.reserve(mBlocks.size() / 2);
    for (size_t i = 0; i < mBlocks.size(); ++i)
    {
      if (mBlocks[i])
      {
        blocks.emplace_back(i, mBlocks[i]->GetId());
      }
    }
    mTessellator->Set(mPos, std::move(blocks));
    mTessellator->SayChanged(mPos);
    //LOG(trace) << "SectorOnDraw: " << glfwGetTime() - currentTime;
  }
}
