// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "SectorLoader.h"
#include "World.h"
#include <GLFW\glfw3.h>
#include <iostream>


SectorLoader::SectorLoader(World &world, const SPos &pos, size_t radius)
  : mWorld(world), mPos(pos)
{
  SetRadius(radius);
  Reload();
}

void SectorLoader::Reload()
{
  // Загружаем нужные сектора.
  for (const auto &site : mSite)
  {
    auto a = mWorld.GetSector(mPos + site);
    if (!a)
      return;
  }
}

SectorLoader::~SectorLoader()
{
}

void SectorLoader::SetRadius(size_t radius)
{
  mRadius = radius;

  mSite.clear();

  int begin = -static_cast<int>(mRadius);
  int end = static_cast<int>(mRadius);
  glm::ivec3 pos(begin); 
  for (pos.z = begin; pos.z <= end; ++pos.z)
  for (pos.y = begin; pos.y <= end; ++pos.y)
  for (pos.x = begin; pos.x <= end; ++pos.x)
  {
    mSite.push_back(pos);
  }

}

void SectorLoader::SetPos(const SPos &pos)
{
  if (mPos == pos)
  {
    //return;
  }
  mPos = pos;
  Reload();
}
