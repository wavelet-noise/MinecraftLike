// ============================================================================
// ==         Copyright (c) 2016, Samsonov Andrey and Smirnov Denis          ==
// ==                  See license.txt for more information                  ==
// ============================================================================
#include <GLFW\glfw3.h>
#include "WorldWorker.h"
#include <thread>
#include "tools\CoordSystem.h"
#include "Core\Sector.h"
#include "Core\MapGen\PerlinNoise.h"
#include "Core\RenderSector.h"
#include "..\DB.h"
#include "..\..\tools\Log.h"

WorldWorker::WorldWorker()
{
}

std::shared_ptr<Sector> WorldWorker::GetSector(const SPos &v)
{
  std::lock_guard<std::mutex> scope(mQueueMutex);
  mLast = v;

	auto f = mReady.find(v);
	if (f != mReady.end())
		return f->second;

	if (mRequested.find(v) == mRequested.end())
	  mRequested.insert(v);

	return nullptr;
}

void place_tree(Sector &s, const glm::vec3 &pos)
{

}

void WorldWorker::Process()
{
  mQueueMutex.lock();
	if (!mRequested.empty())
	{
    SPos last = mLast;
    auto r = mRequested.find(last);
    if(r == mRequested.end())
      r = mRequested.begin();
    mQueueMutex.unlock();

    auto gen = Generate(*r);

    mQueueMutex.lock();
    mReady[*r] = gen;
    mRequested.erase(r);
    mQueueMutex.unlock();
	}
  else
    mQueueMutex.unlock();
}

#define GEN_OCT 5
float flatness(float tx, float ty)
{
  return (PerlinNoise2D(tx, ty, 2, 2, GEN_OCT) + 1) / 2.f;
}

float dens(float tx, float ty, float tz)
{
  float flat = 1/5.f;// flatness(tx / 1000.f, ty / 1000.f);
  if (tz < -SECTOR_SIZE)
    return 1;
  else if (tz < 0)
    return PerlinNoise3D(tx / 100.f, ty / 100.f, tz / 100.f, 1+5*flat, 2, GEN_OCT) + ((-tz*(SECTOR_SIZE / 1000.f)));
  else
    return PerlinNoise3D(tx / 100.f, ty / 100.f, tz / 100.f, 1+5*flat, 2, GEN_OCT) / ((tz + float(SECTOR_SIZE)) / float(SECTOR_SIZE));
}

bool solid(float tx, float ty, float tz)
{
  return dens(tx, ty, tz) > 0.1;
}
#undef GEN_OCT

inline std::shared_ptr<Sector> WorldWorker::Generate(const SPos & spos)
{
	auto currentTime = glfwGetTime(); //TODO: totally unnecessary
	std::shared_ptr<Sector> psec = std::make_shared<Sector>(spos);
	Sector &s = *psec;

	size_t blocksCount = 0;
  const size_t size = static_cast<size_t>(SECTOR_SIZE);

  auto bg = DB::Get().Create(StringIntern("grass"));
  auto bd4 = DB::Get().Create(StringIntern("dirt4"));
  auto bd3 = DB::Get().Create(StringIntern("dirt3"));
  auto bd2 = DB::Get().Create(StringIntern("dirt2"));
  auto bd = DB::Get().Create(StringIntern("dirt"));

  if (spos.x != 0) // для того, чтоб смотреть что там в разрезе
  for (int i = 0; i < SECTOR_SIZE; ++i)
  {
    for (int j = 0; j < SECTOR_SIZE; ++j)
    {
      for (int k = 0; k < SECTOR_SIZE; ++k)
      {
        float tx = static_cast<float>(i + s.mPos.x*SECTOR_SIZE);
        float ty = static_cast<float>(j + s.mPos.y*SECTOR_SIZE);
        float tz = static_cast<float>(k + s.mPos.z*SECTOR_SIZE);

        if (solid(tx, ty, tz))
        {
            if (!solid(tx, ty, tz + 1))
              s.mBlocks[k * SECTOR_SIZE * SECTOR_SIZE + j * SECTOR_SIZE + i] = bg;
          else
          {
              if (solid(tx, ty, tz + 15))
                s.mBlocks[k * SECTOR_SIZE * SECTOR_SIZE + j * SECTOR_SIZE + i] = bd4;
            else if (solid(tx, ty, tz + 10))
                s.mBlocks[k * SECTOR_SIZE * SECTOR_SIZE + j * SECTOR_SIZE + i] = bd3;
            else if (solid(tx, ty, tz + 5))
                s.mBlocks[k * SECTOR_SIZE * SECTOR_SIZE + j * SECTOR_SIZE + i] = bd2;
            else
                s.mBlocks[k * SECTOR_SIZE * SECTOR_SIZE + j * SECTOR_SIZE + i] = bd;
          }
        }
      }
    }
  }

  psec->SayChanged();

	LOG(trace) << "SectorGen: " << glfwGetTime() - currentTime << " blocks count: " << blocksCount;
  return psec;
}