



#include "World.h"
#include <tuple>
#include <utility>
#include <GLFW\glfw3.h>
#include "..\tools\CoordSystem.h"
#include "MapGen\WorldWorker.h"
#include "..\Render\Render.h"
#include "Tessellator.h"
#include <Core\Chest.h>

World::World()
{
  mPlayer = std::make_unique<Player>(StringIntern(std::string("player")));
  mPlayer->PushAgent(std::make_shared<Chest>());
}


World::~World()
{
}

void World::Update(float dt)
{
  mEventBus.Update(this);

  for (auto &i : mSectors)
  {
    i.second->Update(this, dt);
  }
  mPlayer->Update(GameObjectParams{ this, nullptr, {}, dt});
}



std::shared_ptr<Sector> World::GetSector(const SPos &position)
{
  auto it = mSectors.find(position);
  if (it == mSectors.end())
  {
    if (auto psec = WorldWorker::Get().GetSector(position))
    {
      mSectors[position] = psec;
      psec->Draw(mTesselator);

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

PGameObject World::GetBlock(const WBPos &wbpos)
{
  auto spos = cs::WBtoS(wbpos);
  if (auto sector = FindSector(spos))
  {
    return sector->GetBlock(cs::WBtoSB(wbpos, spos));
  }

  return nullptr;
}

PGameObject World::GetBlock(const WBPos &wbpos, std::shared_ptr<Sector> &contains)
{
	auto spos = cs::WBtoS(wbpos);
	if (auto sector = FindSector(spos))
	{
		contains = sector;
		return sector->GetBlock(cs::WBtoSB(wbpos, spos));
	}

	contains = nullptr;
	return nullptr;
}


static float sdt = 0;
static World *sworld = nullptr;
static glm::ivec3 neib[] = {
	{  1,  0,  0 },
	{  0,  1,  0 },
	{  0,  0,  1 },
	{ -1,  0,  0 },
	{  0, -1,  0 },
	{  0,  0, -1 }
};

void World::SetBlock(const WBPos &wbpos, PGameObject block)
{
  auto spos = cs::WBtoS(wbpos);
  std::shared_ptr<Sector> sec;

  if (auto l = GetBlock(wbpos, sec))
	  l->OnDestroy({ this, sec.get(), wbpos, 0 });

  if (auto sector = FindSector(spos))
  {
	  sector->SetBlock(cs::WBtoSB(wbpos, spos), block);
	  block->OnCreate({ this, sec.get(), wbpos, 0 });
  }

  for (const auto &n : neib)
  {
	  if (auto l = GetBlock(wbpos + n, sec))
		  l->OnAdjacentChanged({ this, sec.get(), wbpos + n, 0 });
  }
}

int World::GetActiveCount()
{
  return mSectors.size();
}

Player *World::GetPlayer()
{
  return mPlayer.get();
}

void World::SetTessellator(Tessellator *tess)
{
  mTesselator = tess;
}

Tessellator * World::GetTessellator()
{
  return mTesselator;
}

void World::PushEvent(std::unique_ptr<GameEvent> event)
{
  mEventBus.Push(std::move(event));
}
