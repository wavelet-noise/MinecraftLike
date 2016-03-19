



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

void Tessellator::Start()
{
  //TODO:???
}

void Tessellator::Process()
{
  auto start = glfwGetTime();
  for (auto it = mSectors.begin(); it != mSectors.end(); ++it)
  {
    it->second->Update(this, mRender);
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

std::shared_ptr<SectorTessellator> Tessellator::FindSector(const SPos &pos)
{
  auto it = mSectors.find(pos);
  if (it != mSectors.end())
  {
    return it->second;
  }

  return nullptr;
}
