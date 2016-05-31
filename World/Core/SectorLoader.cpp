



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
    auto pos = mPos + site;
//    pos.z = 0;
    auto a = mWorld.GetSector(pos);
    //if (!a)
    //{
    //  return;
    //}
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
  //pos.z = 0;
  for (pos.z = end; pos.z >= begin; --pos.z)
  for (pos.y = begin; pos.y <= end; ++pos.y)
  for (pos.x = begin; pos.x <= end; ++pos.x)
  {
    if(glm::length(glm::vec3(pos)) <= mRadius)
    {
      mSite.push_back(pos);
    }
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
