



#include "Tessellator.h"
#include "..\Render\RenderSector.h"
#include "DB.h"
#include <utility>
#include <gui\WindowPerfomance.h>

Tessellator::Tessellator(RenderSector &render)
	: mRender(render)
{
}

void Tessellator::Set(const WBPos &pos, PGameObjectTessellator block)
{
	PushFunc([this](const std::tuple<WBPos, PGameObjectTessellator> &tuple)
	{
		auto &pos = std::get<0>(tuple);
		auto &block = std::get<1>(tuple);
		auto spos = cs::WBtoS(pos);
		auto sector = FindSector(spos);
		if (!sector)
		{
			sector = std::make_shared<SectorTessellator>(spos);
			mSectors[spos] = sector;
		}
		sector->SetBlock(cs::WBtoSB(pos, spos), block);
	}, pos, block);
}

void Tessellator::Set(const SPos &spos, std::vector<std::tuple<size_t, StringIntern>> &&blocks)
{
	PushFunc([&, this](const std::tuple<SPos, std::vector<std::tuple<size_t, StringIntern>>> &tuple)
	{
		const auto &spos = std::get<0>(tuple);
		auto &blocks = std::get<1>(tuple);

		auto sector = FindSector(spos);
		if (!sector)
		{
			sector = std::make_shared<SectorTessellator>(spos);
			mSectors[spos] = sector;
		}
		for (size_t i = 0; i < blocks.size(); ++i)
		{
			sector->SetBlock(cs::ItoSB(std::get<0>(blocks[i])), DB::Get().CreateTesselator(std::get<1>(blocks[i])));
		}
	}, spos, std::move(blocks));
}

void Tessellator::SayChanged(const SPos &pos)
{
	PushFunc([this](const std::tuple<SPos> &tuple)
	{
		if (auto sector = FindSector(std::get<0>(tuple)))
		{
			sector->SayChanged();
		}
	}, pos);
}

void Tessellator::SayCamera(Camera * c)
{
	cam = c;
}

void Tessellator::SetSlise(int s)
{
	if (s != slise)
	{
		auto ss1 = cs::WBtoS({ 0,0,s });
		auto ss2 = cs::WBtoS({ 0,0,slise });
		if (ss1.z > ss2.z)
			std::swap(ss1, ss2);
		for (auto it = mSectors.begin(); it != mSectors.end(); ++it)
		{
			if (it->first.z >= ss1.z && it->first.z <= ss2.z)
				it->second->SayChanged();
		}
	}
	slise = s;
}

int Tessellator::GetSlise()
{
	return slise;
}

void Tessellator::Start()
{
	//TODO:???
}

void Tessellator::Process()
{
	auto start = glfwGetTime();
	for (auto it = mSectors.begin(); it != mSectors.end(); ++it)
	{
		if (it->second->IsChanged())
		{
			glm::vec3 min = it->second->GetPos();
			if (cam->BoxWithinFrustum(min * float(SECTOR_SIZE), (min + glm::vec3(1, 1, 1)) * float(SECTOR_SIZE)))
			{
				it->second->Update(this, mRender, slise);
			}
		}
	}
	auto end = glfwGetTime();
	WindowPerfomance::Get().TesselatorDt(static_cast<float>(end - start));
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
}

void Tessellator::Stop()
{

}

PGameObjectTessellator Tessellator::GetBlock(const WBPos &pos)
{
	auto spos = cs::WBtoS(pos);
	if (auto sector = FindSector(spos))
	{
		return sector->GetBlock(cs::WBtoSB(pos, spos));
	}

	return nullptr;
}

void Tessellator::Interrupt()
{
	//mThread.release();
}

std::shared_ptr<SectorTessellator> Tessellator::FindSector(const SPos &pos)
{
	static SPos last{ -99999 };
	static std::shared_ptr<SectorTessellator> lasttess;

	if (last == pos)
		return lasttess;

	auto it = mSectors.find(pos);
	if (it != mSectors.end())
	{
		lasttess = it->second;
		return it->second;
	}

	return nullptr;
}
