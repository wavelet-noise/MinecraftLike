// ============================================================================
// ==                   Copyright (c) 2015, Smirnov Denis                    ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include "World.h"
#include <tuple>
#include <utility>
#include <GLFW\glfw3.h>
#include "..\tools\CoordSystem.h"
#include "RenderSector.h"
#include "MapGen\WorldWorker.h"
#include "..\Render\Render.h"


World::World()
{
  mPlayer = std::make_unique<Player>(StringIntern(std::string("player")));
}


World::~World()
{
}

void World::Update(float dt)
{
  mPlayer->Update(GameObjectParams{ this, nullptr, {}, dt});
}

void World::UpdateGraphic(Render &render)
{
  for (auto &sector : mSectors)
  {
    sector.second->UpdateGraphic(this, render);
  }
}

std::shared_ptr<Sector> World::GetSector(const SPos &position)
{
  auto it = mSectors.find(position);
  if (it == mSectors.end())
  {
    if (auto psec = WorldWorker::instance().GetSector(position))
    {
      mSectors[position] = psec;
      // Добавлен новый сектор, сообщим соседним секторам, что б перестроились.
      
      /*SPos offset[] = 
      {
        {  1,  0,  0 },
        { -1,  0,  0 },
        {  0,  1,  0 },
        {  0, -1,  0 },
        {  0,  0,  1 },
        {  0,  0, -1 },
      };
      for (const auto &i : offset)
      {
        if (auto &sector = FindSector(position + i))
        {
          sector->SayChanged();
        }
      }*/
      return psec;
    }

    return nullptr;
  }
  
  return it->second;
}

std::shared_ptr<Sector> World::FindSector(const SPos &position)
{
  auto it = mSectors.find(position);
  if (it != mSectors.end())
  {
    return it->second;
  }

  return nullptr;
}

PBlock World::GetBlock(const WBPos &wbpos)
{
  auto spos = cs::WBtoS(wbpos);
  if (auto sector = FindSector(spos))
  {
    return sector->GetBlock(cs::WBtoSB(wbpos, spos));
  }

  return nullptr;
}

void World::SetBlock(const WBPos &wbpos, PBlock block)
{
  auto spos = cs::WBtoS(wbpos);
  if (auto sector = FindSector(spos))
  {
    sector->SetBlock(cs::WBtoSB(wbpos, spos), block);
    sector->SayChanged();
  }
}

Player *World::GetPlayer()
{
  return mPlayer.get();
}
