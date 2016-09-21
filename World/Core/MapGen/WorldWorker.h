#pragma once
#include <GL\glew.h>
#include <glm/glm.hpp>
#include <boost\noncopyable.hpp>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include "tools\CoordSystem.h"
#include <mutex>
#include "GenStrategy.h"

class Sector;

class WorldWorker : public boost::noncopyable
{
public:
	WorldWorker();
	~WorldWorker();

	std::shared_ptr<World> w;

	std::shared_ptr<Sector> GetSector(const SPos &v);
	void Process();
	void WorldPass() const;

private:
	std::shared_ptr<Sector> Generate(const SPos &spos);
	SPos mLast; //replace with queue

	std::mutex mQueueMutex;
	std::unordered_map<SPos, std::shared_ptr<Sector>> mReady;
	std::unordered_set<SPos> mRequested;

	std::unique_ptr<GenStrategy> mGenerator;
};