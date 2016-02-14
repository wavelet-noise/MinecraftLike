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
  : mPos(position)
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
  // Если рендер сектор давно не обновлялся, нужно его удалить.
}

void Sector::UpdateGraphic(World *world, Render &render)
{
  // Если рендер сектор отсутствует, нужно его создать.
  if (!mRenderSector)
  {
    mRenderSector = std::make_unique<RenderSector>(mPos);
  }

  auto currentTime = glfwGetTime();
  GameObjectParams params{ world , this,{} };

  if (mRenderSector->IsNeedBuild())
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
  mRenderSector->Update(render);
}

void Sector::SayChanged()
{
  if (!mRenderSector)
  {
    mRenderSector = std::make_unique<RenderSector>(mPos);
  }
  mRenderSector->SayChanged();
}

RenderSector &Sector::GetRenderSector()
{
  return *mRenderSector;
}
