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


WorldWorker &WorldWorker::Get()
{
  static WorldWorker object;
  return object;
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

void WorldWorker::Process()
{
  mQueueMutex.lock();
  if (!mRequested.empty())
  {
    SPos last = mLast;
    auto r = mRequested.find(last);
    if (r == mRequested.end())
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



inline std::shared_ptr<Sector> WorldWorker::Generate(const SPos & spos)
{
  auto currentTime = glfwGetTime(); //TODO: totally unnecessary
  std::shared_ptr<Sector> psec = std::make_shared<Sector>(spos);
  Sector &sector = *psec;

  const size_t size = static_cast<size_t>(SECTOR_SIZE);

  for (size_t i = 0; i < size * size * size; ++i)
  {
    const auto &sbpos = cs::ItoSB(i);
    sector.SetBlock(sbpos, mGenerator.Create(cs::SBtoWB(sbpos, spos)));
  }

  LOG(trace) << "SectorGen: " << glfwGetTime() - currentTime;
  return psec;
}

