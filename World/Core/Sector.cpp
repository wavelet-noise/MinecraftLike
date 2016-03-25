



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

  if (mTessellator)
  {
    if (block)
    {
      mTessellator->Set(cs::SBtoWB(pos, mPos), DB::Get().CreateTesselator(block->GetId()));
      mTessellator->SayChanged(mPos);
    }
    else
    {
      mTessellator->Set(cs::SBtoWB(pos, mPos), nullptr);
      mTessellator->SayChanged(mPos); 
    }
  }
 
}

void Sector::Update(World *world, float dt)
{
  GameObjectParams gop{ world, this, {}, dt };
  for (size_t i = 0; i < mUniqueBlocks.size(); ++i)
  {
    gop.pos = cs::SBtoWB(cs::ItoSB(mUniquePoses[i]), mPos);
    if(mUniqueBlocks[i]) mUniqueBlocks[i]->Update(gop);
  }
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
