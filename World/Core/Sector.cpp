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

Sector::Sector(const SPos &position)
  : mPos(position), mRenderSector(position)
{
}


Sector::~Sector()
{
}

const SPos & Sector::GetSectorPosition() const
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
  mBlocks[pos.z * SECTOR_SIZE * SECTOR_SIZE + pos.y * SECTOR_SIZE + pos.x] = block;
}

void Sector::Update(World *world)
{

}

void Sector::UpdateGraphic(World *world, Render &render)
{
  auto currentTime = glfwGetTime();
  GameObjectParams params{ world , this,{} };

  if (mRenderSector.IsNeedBuild())
  {
    const size_t size = static_cast<size_t>(SECTOR_SIZE);
    for (size_t i = 0; i < mBlocks.size(); ++i)
    {
      if (mBlocks[i])
      {
        params.pos = cs::SBtoWB({ i % size, (i / size) % size, i / (size * size) }, mPos);
        mBlocks[i]->Draw(params);
      }
    }
    LOG(trace) << "SectorBuild: " << glfwGetTime() - currentTime;
  }
  mRenderSector.Update(render);
}

RenderSector &Sector::GetRenderSector()
{
  return mRenderSector;
}
