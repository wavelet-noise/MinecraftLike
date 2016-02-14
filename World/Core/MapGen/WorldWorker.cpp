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

static PerlinNoise noise(0);

std::shared_ptr<Sector> WorldWorker::Generate(const SPos &spos)
{
	auto currentTime = glfwGetTime(); //TODO: totally unnecessary
	std::shared_ptr<Sector> psec = std::make_shared<Sector>(spos);
	Sector &s = *psec;

	size_t blocksCount = 0;

  const size_t size = static_cast<size_t>(SECTOR_SIZE);
  for (int i = 0; i < SECTOR_SIZE; ++i)
  {
    for (int j = 0; j < SECTOR_SIZE; ++j)
    {
      float tx = static_cast<float>(i + s.mPos.x*SECTOR_SIZE);
      float ty = static_cast<float>(j + s.mPos.y*SECTOR_SIZE);

      float h = (noise.Noise2(tx / 10.0f, ty / 10.0f) + 1.0f)  * SECTOR_SIZE / 3.f;

      h = glm::min(h, float(SECTOR_SIZE));
      h = glm::max(h, 0.f);

      s.mBlocks[int(h) * SECTOR_SIZE * SECTOR_SIZE + j * SECTOR_SIZE + i] = DB::Get().Create(StringIntern("grass"));
      for (int k = 0; k < int(h); ++k)
      {
        s.mBlocks[k * SECTOR_SIZE * SECTOR_SIZE + j * SECTOR_SIZE + i] = DB::Get().Create(StringIntern("dirt"));
      }
    }
  }
  psec->SayChanged();

	LOG(trace) << "SectorGen: " << glfwGetTime() - currentTime << " blocks count: " << blocksCount;
  return psec;
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
