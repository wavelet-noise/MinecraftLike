



#include <GLFW\glfw3.h>
#include "WorldWorker.h"
#include <thread>
#include "tools\CoordSystem.h"
#include "Core\Sector.h"
#include "Core\MapGen\PerlinNoise.h"
#include "..\DB.h"
#include "..\..\tools\Log.h"
#include <gui\WindowPerfomance.h>
#include "WorldGenFlat.h"
#include "WorldGenMountains.h"

WorldWorker &WorldWorker::Get()
{
  static WorldWorker object;
  return object;
}

WorldWorker::WorldWorker()
{
  mGenerator = std::make_unique<WorldGenMountains>();
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
  auto start = glfwGetTime();
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
  auto end = glfwGetTime();
  WindowPerfomance::Get().GeneratorDt(end - start);
}



inline std::shared_ptr<Sector> WorldWorker::Generate(const SPos & spos)
{
  std::shared_ptr<Sector> psec = std::make_shared<Sector>(spos);
  Sector &sector = *psec;

  mGenerator->Generate(sector);

  return psec;
}

