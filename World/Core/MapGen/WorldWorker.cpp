#define MINIZ_HEADER_FILE_ONLY
#include "miniz.c"
#include "WorldWorker.h"
#include <thread>
#include "tools\CoordSystem.h"
#include "Core\Sector.h"
#include <gui\WindowPerfomance.h>
#include "WorldGenMountains.h"
#include <boost/asio/streambuf.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <fstream>

WorldWorker &WorldWorker::Get(World &w)
{
  static WorldWorker object;
  object.w = &w;
  return object;
}

WorldWorker::WorldWorker()
{
  mGenerator = std::make_unique<WorldGenMountains>();
}

WorldWorker::~WorldWorker()
{
	remove("Save\\worldsave.world");

	for(const auto &s : mReady)
	{
		//boost::asio::streambuf b;
		auto a_name = (boost::format("%1%_%2%_%3%.sec") % s.first.x % s.first.y % s.first.z).str();
		//std::ofstream os(a_name.c_str());
		boost::asio::streambuf sb;
		//if (os.is_open())
		{
			boost::archive::binary_oarchive oa(sb);

			oa << *s.second;

			mz_zip_add_mem_to_archive_file_in_place("Save\\worldsave.world", a_name.c_str(), boost::asio::buffer_cast<const char*>(sb.data()), sb.size(), nullptr, 0, MZ_BEST_COMPRESSION);
		}
	}
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
  {
	  WorldPass();
	  mQueueMutex.unlock();
  }

  auto end = glfwGetTime();
  WindowPerfomance::Get().GeneratorDt(end - start);
}

inline void WorldWorker::WorldPass()
{
	mGenerator->WorldPass(*w);
}

inline std::shared_ptr<Sector> WorldWorker::Generate(const SPos & spos)
{
  std::shared_ptr<Sector> psec = std::make_shared<Sector>(spos);
  Sector &sector = *psec;

  mGenerator->Generate(sector);

  return psec;
}

