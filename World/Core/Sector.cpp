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

}


Sector::~Sector()
{
}

const SPos & Sector::GetPos() const
{
  return mPos;
}


void Sector::SetBlock(const SBPos &pos, PGameObject block)
{
  SectorBase<PGameObject>::SetBlock(pos, block);

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
  if (mTessellator /*&& !mBlocks.empty()*/)
  {
    auto currentTime = glfwGetTime();
    std::vector<std::tuple<size_t, StringIntern>> blocks;
    blocks.reserve(mCountBlocks);
    for (size_t i = 0; i < mBlocks.size(); ++i)
    {
      if (mBlocks[i])
      {
        blocks.emplace_back(i, GetBlock(i)->GetId());
      }
    }
    mTessellator->Set(mPos, std::move(blocks));
    mTessellator->SayChanged(mPos);
    //LOG(trace) << "SectorOnDraw: " << glfwGetTime() - currentTime;
  }
}
