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
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>
#include <boost/iostreams/stream.hpp>

WorldWorker &WorldWorker::Get(World &w)
{
  static WorldWorker object;
  object.w = &w;
  return object;
}

WorldWorker::WorldWorker()
{
  mGenerator = std::make_unique<WorldGenMountains>();

  mz_zip_archive zip_archive;
  mz_bool status;

  memset(&zip_archive, 0, sizeof(zip_archive));

  status = mz_zip_reader_init_file(&zip_archive, "Save\\worldsave.world", 0);

  if (!status)
  {
	  LOG(error) << "zip file appears invalid...";
	  return;
  }

  for (int i = 0; i < static_cast<int>(mz_zip_reader_get_num_files(&zip_archive)); i++)
  {

	  mz_zip_archive_file_stat file_stat;
	  if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat))
	  {
		  LOG(error) << "zip file read error...";
		  mz_zip_reader_end(&zip_archive);
		  return;
	  }

	  size_t uncompressed_size = file_stat.m_uncomp_size;
	  void* p = mz_zip_reader_extract_file_to_heap(&zip_archive, file_stat.m_filename, &uncompressed_size, 0);
	  if (!p)
	  {
		  LOG(error) << "mz_zip_reader_extract_file_to_heap() failed...";
		  mz_zip_reader_end(&zip_archive);
		  return;
	  }
	  
	  auto s = std::make_shared<Sector>(SPos(0, 0, 0));

	  boost::iostreams::array_source sr(static_cast<char*>(p), uncompressed_size);
	  boost::iostreams::stream<decltype(sr)> source(sr);

	  try 
	  {
		  boost::archive::binary_iarchive ia(source);

		  ia >> *s;

		  mReady[s->GetPos()] = s;
	  } 
	  catch (...)
	  {
		  LOG(error) << "broken sector " << file_stat.m_filename;
	  }

	  mz_free(p);
  }

  mz_zip_reader_end(&zip_archive);
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

			mz_zip_add_mem_to_archive_file_in_place("Save\\worldsave.world", a_name.c_str(), boost::asio::buffer_cast<const char*>(sb.data()), sb.size(), nullptr, 0, 0);
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

